use colored::Colorize;
use ctrlc;
use std::sync::Arc;
use std::sync::atomic::{AtomicUsize, Ordering};
use tokio::select;
use tokio::sync::RwLock;
use tokio::sync::broadcast;
use tokio::task::JoinSet;

use crate::config::CONFIG;
use crate::health_check::HealthCheckError;
use crate::tasks::can_data::load_can_database;

mod mock;
mod config;
mod tasks;
mod health_check;

use mock::run_mock_task;
use health_check::HealthCheck;
use tasks::api_handler::run_api_handler;
use tasks::can_data::can_data_handler::run_can_data_handler;
use tasks::client_api::clients::Clients;
use tasks::serial_handler::run_serial_task;
use tasks::telem_message::CanPayload;

// macro for printing red hehe
macro_rules! error_println {
    ($($arg:tt)*) => {{
        let s = format!($($arg)*);
        eprintln!("{}", s.red());
    }};
}

#[tokio::main]
async fn main() {
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
    let mut tasks = JoinSet::new();

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
    tasks.spawn(run_api_handler(
        shutdown_rx.resubscribe(),
        health_check.hc_tx.clone(),
        clients.clone(),
        can_db.clone(),
    ));
    tasks.spawn(run_can_data_handler(
        shutdown_rx.resubscribe(),
        health_check.hc_tx.clone(),
        can_queue_rx,
        clients.clone(),
        can_db.clone(),
    ));
    // run mock xor serial radio
    match CONFIG.mock {
        true => tasks.spawn(run_mock_task(
            shutdown_rx.resubscribe(),
            health_check.hc_tx.clone(),
            can_queue_tx.clone(),
            can_db.clone(),
        )),
        _ => tasks.spawn(run_serial_task(
            shutdown_rx.resubscribe(),
            health_check.hc_tx.clone(),
            can_queue_tx,
        )),
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
    // wait for tasks to clean up
    while let Some(res) = tasks.join_next().await {
        if let Err(err) = res {
            eprintln!("Task failed to clean up: {err}");
        }
    }

    println!("Exiting main thread.");
}
