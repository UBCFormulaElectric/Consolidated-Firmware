use std::thread::{self, JoinHandle};

mod tasks;
use tasks::serial::run_serial_tasks;

fn main() {
    let mut handles: Vec<JoinHandle<()>> = vec![];

    // start serial
    handles.push(thread::spawn(run_serial_tasks));

    // prevent main from exiting
    for handle in handles {
        handle.join().unwrap(); // i love unwrapping
    }
}
