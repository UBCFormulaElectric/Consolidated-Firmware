use std::sync::mpsc::Receiver;
use influxdb2::Client;
use influxdb2::models::DataPoint;

use crate::config::CONFIG;
use crate::tasks::telem_message::CanMessage;
use crate::tasks::thread_signal_handler::should_run;

/**
 * After serial_handler parses the can messages,
 * this task will handle "distributing" the data to
 * both the socket clients and influxdb
 */
pub fn run_broadcaster_task(can_queue_receiver: Receiver<CanMessage>) {
    // TODO consider splitting this task for client handling and influxdb into two tasks

    let influx_client = Client::new(
        &CONFIG.influxdb_url,
        &CONFIG.influxdb_org,
        &CONFIG.influxdb_token
    );
    
    loop {
        if !should_run() {
            break;
        }
        match can_queue_receiver.recv() {
            Ok(CanMessage {can_id, can_time_offset, can_payload }) => {
                // TODO Process the message using jsoncan

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