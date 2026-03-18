use colored::Colorize;
use ctrlc;
use std::sync::Arc;
use std::sync::atomic::{AtomicUsize, Ordering};
use tokio::select;
use tokio::sync::RwLock;
use tokio::sync::broadcast;
use tokio::task::{JoinError, JoinSet};

use crate::config::CONFIG;
use crate::tasks::{HealthCheckError, Task};
use crate::tasks::can_data::load_can_database;

mod mock;
mod config;
mod tasks;

use mock::run_mock_task;
use tasks::HealthCheck;
use tasks::api_handler::run_api_handler;
use tasks::can_data::can_data_handler::run_can_data_handler;
use tasks::client_api::clients::Clients;
use tasks::serial_handler::run_serial_task;
use tasks::telem_message::CanPayload;


// macro for printing red hehe
#[macro_export]
macro_rules! error_println {
    ($($arg:tt)*) => {{
        let s = format!($($arg)*);
        eprintln!("{}", s.red());
    }};
}

// enable verbose print with `--features verbose`
#[macro_export]
macro_rules! vprintln {
    ($($arg:tt)*) => {{
        #[cfg(feature = "verbose")] {
            println!("{}", format!($($arg)*));
        }
    }};
}

// enable even more verbose print with `--features debug`
#[macro_export]
macro_rules! dprintln {
    ($($arg:tt)*) => {{
        #[cfg(feature = "debug")] {
            println!("{}", format!($($arg)*));
        }
    }};
}

#[tokio::main]
async fn main() {
    vprintln!("Verbose print enabled");
    dprintln!("Debug print enabled");

    // shutdown signal for threads
    let (shutdown_tx, mut shutdown_rx) = broadcast::channel(1);

    // handle termination signal
    let interrupt_count = AtomicUsize::new(0);
    ctrlc::set_handler(move || {
        let count = interrupt_count.fetch_add(1, Ordering::SeqCst);
        if count >= 1 {
            eprintln!("Force exiting...");
            std::process::exit(1);
        }
        eprintln!("Interrupt received! Cleaning up...");
        shutdown_tx.send(()).ok();
    })
    .expect("Error setting Ctrl-C handler");

    // setup task manager
    let mut tasks: JoinSet<(Task, Result<(), JoinError>)> = JoinSet::new();

    // this is equivalent to queue in old backend
    // use broadcast instead of mpsc, probably only one serial source but multiple consumers
    // TODO figure out buffer size
    let (can_queue_tx, can_queue_rx) = broadcast::channel::<CanPayload>(32);

    // track which clients subscribe to which signals
    // maps signal name to client ids
    let clients = Arc::new(RwLock::new(Clients::new()));

    // load CAN database
    let can_db = Arc::new(load_can_database().expect("Could not init Can db"));

    let health_check = HealthCheck::new();
    
    // start tasks
    spawn_task(
        &mut tasks,
        Task::ApiHandler,
        run_api_handler(
            shutdown_rx.resubscribe(),
            health_check.hc_tx.clone(),
            clients.clone(),
            can_db.clone(),
        )
    );
    spawn_task(
        &mut tasks, 
        Task::CanDataHandler,
        run_can_data_handler(
            shutdown_rx.resubscribe(),
            health_check.hc_tx.clone(),
            can_queue_rx,
            clients.clone(),
            can_db.clone(),
        )
    );
    // run mock xor serial radio
    match CONFIG.mock {
        true => spawn_task(
            &mut tasks,
            Task::SerialHandler,
            run_mock_task(
                shutdown_rx.resubscribe(),
                health_check.hc_tx.clone(),
                can_queue_tx.clone(),
                can_db.clone(),
            )
        ),
        _ => spawn_task(
            &mut tasks,
            Task::SerialHandler,
            run_serial_task(
                shutdown_rx.resubscribe(),
                health_check.hc_tx.clone(),
                can_queue_tx,
            )
        ),
    };

    select! {
        hc_res = health_check.wait_for_health_checks() => {
            match hc_res {
                Ok(_) => {
                    println!("{}", "Health checks passed, all tasks successfully started!".green());
                },
                Err(e) => {
                    match e {
                        HealthCheckError::TaskFailure(task) => {
                            error_println!("{task:?} failed health check!");
                        },
                        HealthCheckError::Timeout => {
                            error_println!("Health checks are taking a while, are you sure tasks have started successfully?");
                        },
                        HealthCheckError::ChannelsClosed => {
                            error_println!("Health check failed by prematurely closing channels");
                        }
                    }
                }
            }
        },
        _ = shutdown_rx.recv() => {}
    }

    // tasks are running
    // stop here for tasks to run
    while let Some(res) = tasks.join_next().await {
        // res is the result of the spawn_task async move, probably a way to make it cleaner
        let (task, task_res) = res.unwrap();
        match task_res {
            // todo robust restart
            Ok(_) => {
                vprintln!(format!("{task:?} ended successfuly").yellow());
            },
            Err(e) => {
                error_println!("{task:?} panicked with error: {e:?}");
            }
        }
    }

    println!("Exiting main thread.");
}

fn spawn_task<F>(
    task_set: &mut JoinSet<(Task, Result<(), JoinError>)>,
    task: Task,
    runner_fn: F
) where F: std::future::Future<Output = ()> + Send + 'static {
    task_set.spawn(async move {
        let res = tokio::spawn(runner_fn).await;
        (task, res)
    });
}