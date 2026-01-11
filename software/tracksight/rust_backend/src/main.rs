use ctrlc;
use std::sync::atomic::{AtomicUsize, Ordering};
use tokio::sync::broadcast::channel;
use tokio::task::{JoinSet};


mod config;
mod tasks;
use tasks::telem_message::CanMessage;

/**
 * Handlers
 */
use tasks::serial_handler::run_serial_task;
use tasks::live_data_handler::run_live_data_handler;
use tasks::influx_handler::run_influx_handler;

#[tokio::main]
async fn main() {
    let mut tasks = JoinSet::new();
    // shutdown signal for threads
    let (shutdown_tx, _) = channel(1);

    // this is equivalent to queue in old backend
    // use broadcast instead of mpsc, probably only one serial source but multiple consumers
    // TODO figure out buffer size
    let (can_queue_tx, _) = channel::<CanMessage>(32);

    // start tasks
    tasks.spawn(run_live_data_handler(shutdown_tx.subscribe(), can_queue_tx.subscribe()));
    tasks.spawn(run_influx_handler(shutdown_tx.subscribe(), can_queue_tx.subscribe()));
    tasks.spawn(run_serial_task(shutdown_tx.subscribe(), can_queue_tx));

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
    }).expect("Error setting Ctrl-C handler");

    // wait for tasks to clean up
    while let Some(res) = tasks.join_next().await {
        if let Err(err) = res {
            eprintln!("Task failed to clean up: {err}");
        }
    }

    println!("Exiting main thread.");
}
