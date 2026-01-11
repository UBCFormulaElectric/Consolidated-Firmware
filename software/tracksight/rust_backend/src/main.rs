use ctrlc;
use std::sync::atomic::{AtomicUsize, Ordering};
use tokio::sync::broadcast::channel;
use tokio::task::{JoinSet};


mod config;
mod tasks;
use tasks::telem_message::CanMessage;

use tasks::serial_handler::run_serial_task;
use tasks::can_data_handler::run_can_data_handler;

#[tokio::main]
async fn main() {
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


    /**
     * Setup channels and tasks
     */
    let mut tasks = JoinSet::new();
    // shutdown signal for threads
    let (shutdown_tx, _) = channel(1);

    // this is equivalent to queue in old backend
    // use broadcast instead of mpsc, probably only one serial source but multiple consumers
    // TODO figure out buffer size
    let (can_queue_tx, can_queue_rx) = channel::<CanMessage>(32);

    // start tasks
    tasks.spawn(run_can_data_handler(shutdown_tx.subscribe(), can_queue_rx));
    tasks.spawn(run_serial_task(shutdown_tx.subscribe(), can_queue_tx));

    // wait for tasks to clean up
    while let Some(res) = tasks.join_next().await {
        if let Err(err) = res {
            eprintln!("Task failed to clean up: {err}");
        }
    }

    println!("Exiting main thread.");
}
