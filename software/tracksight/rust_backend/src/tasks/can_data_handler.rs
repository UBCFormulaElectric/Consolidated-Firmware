use std::sync::mpsc::Receiver;

use crate::tasks::telem_message::CanMessage;
use crate::tasks::thread_signal_handler::should_run;

/**
 * Handling serial signals from radio.
 */
pub fn run_influx_task(can_queue_receiver: Receiver<CanMessage>) {
    loop {
        if !should_run() {
            break;
        }
        match can_queue_receiver.recv() {
            Ok(message) => {
                // TODO Process the message
            },
            Err(_) => break, // Channel has closed, no possible senders left
        }
    }
}

// pub struct CanQueue {
//     sender: Sender<CanMessage>,
//     receiver: Receiver<CanMessage>
// }

// impl CanQueue {
//     pub fn new() -> Self {
//         let (sender, receiver) = std::sync::mpsc::channel::<CanMessage>();
//         Self { sender, receiver }
//     }

//     pub fn enqueue(&self, message: CanMessage) {
//         if let Err(e) = self.sender.send(message) {
//             eprintln!("Failed to send CAN message: {}", e);
//         }
//     }

//     /**
//      * Blocks thread until a message is available or channel is closed.
//      * Returns None if channel is closed.
//      */
//     pub fn dequeue(&self) -> Option<CanMessage> {
//         self.receiver.recv().ok()
//     }

//     /**
//      * Clones the sender to allow multiple producers.
//      */
//     pub fn clone_sender(&self) -> Sender<CanMessage> {
//         return self.sender.clone()
//     }
// }