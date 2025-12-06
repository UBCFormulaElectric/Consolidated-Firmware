use std::thread::{self, JoinHandle};
use ctrlc;
use std::sync::mpsc::channel;

mod config;

mod tasks;
use tasks::telem_message::CanMessage;
use tasks::thread_signal_handler::stop_threads;
use tasks::serial_handler::run_serial_task;
use tasks::can_data_handler::run_broadcaster_task;


fn main() {
    let mut handles: Vec<JoinHandle<()>> = vec![];

    // this is equivalent to queue in old backend
    let (sender, receiver) = channel::<CanMessage>();

    // start tasks
    // TODO use tokio to not block a core when reading from serial port
    handles.push(thread::spawn(move || { run_serial_task(sender.clone()) }));
    handles.push(thread::spawn(move || { run_broadcaster_task(receiver) }));


    // handle termination signal
    let interrupt_count = std::sync::atomic::AtomicUsize::new(0);
    ctrlc::set_handler(move || {
        let count = interrupt_count.fetch_add(1, std::sync::atomic::Ordering::SeqCst);
        if count >= 1 {
            eprintln!("Force exiting...");
            std::process::exit(1);
        }
        eprintln!("Interrupt received! Cleaning up...");
        stop_threads();
    }).expect("Error setting Ctrl-C handler");

    // prevent main from exiting
    for handle in handles {
        if handle.join().is_err() {
            eprintln!("A thread has panicked!");
        }
    }

    println!("Exiting main thread.");
}
