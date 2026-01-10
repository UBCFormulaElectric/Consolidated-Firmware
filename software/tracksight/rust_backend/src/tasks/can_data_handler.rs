use tokio::sync::broadcast::Receiver;
use influxdb2::Client;
use tokio::select;

use crate::config::CONFIG;
use crate::tasks::telem_message::CanMessage;

/**
 * After serial_handler parses the can messages,
 * this task will handle "distributing" the data to
 * both the socket clients and influxdb
 */
pub async fn run_broadcaster_task(mut shutdown_signal: Receiver<()>, mut can_queue_receiver: Receiver<CanMessage>) {
    println!("Broadcaster task started.");

    // TODO consider splitting this task for client handling and influxdb into two tasks

    let influx_client = Client::new(
        &CONFIG.influxdb_url,
        &CONFIG.influxdb_org,
        &CONFIG.influxdb_token
    );

    
    // loop select check for shutdown signal
    // if shutdown signal, select block breaks loop early
    loop {
        select! {
            _ = shutdown_signal.recv() => {
                println!("Shutting down broadcaster task.");
                break;
            }
            Ok(can) = can_queue_receiver.recv() => {
                // todo should also probably check for closed channels and close thread
                let CanMessage { can_id, can_time_offset, can_payload } = can;
                // todo handle
            }
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