use std::{collections::VecDeque, sync::{Mutex, OnceLock}};
use std::sync::mpsc::Sender;

use crate::tasks::thread_signal_handler::should_run;

pub struct CanMessage {
    pub can_id: u32,
    pub can_time_offset: f32,
    pub can_payload: Vec<u8>,
}

impl CanMessage {
    fn to_json(&self) -> String {
        format!(
            r#"{{"can_id": {},"can_time_offset": {},"can_payload": {:?}}}"#,
            self.can_id,
            self.can_time_offset,
            self.can_payload
        )
    }
}

pub struct CanQueue {
    sender: Sender<CanMessage>
}

impl CanQueue {
    pub fn new(sender: Sender<CanMessage>) -> Self {
        CanQueue { sender }
    }

    pub fn enqueue(&self, message: CanMessage) {
        if let Err(e) = self.sender.send(message) {
            eprintln!("Failed to send CAN message: {}", e);
        }
    }
}

/**
 * Handling serial signals from radio.
 */
pub fn run_influx_task() {
    QUEUE.set(VecDeque::new());
    loop {
        if !should_run() {
            break;
        }
    }
}

pub fn enqueue_can_data(can_data: CanMessage) {
    match QUEUE.get_mut() {
        Some(queue) => {
            queue.push_back(can_data);
        },
        None => {
            
        }
    }
}

fn dequeue_can_data() -> Option<CanMessage> {
    match QUEUE.lock() {
        Ok(mut queue) => {
            queue.pop_front()
        },
        Err(e) => {
            eprintln!("Failed to dequeue CAN data: {e}");
            None
        }
    }
}