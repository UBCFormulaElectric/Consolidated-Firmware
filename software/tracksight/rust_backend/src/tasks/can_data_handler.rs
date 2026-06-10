use std::sync::Arc;

use tokio::{select, spawn};
use tokio::sync::{RwLock, broadcast};

use crate::tasks::can_data::influx_handler::run_influx_handler;
use crate::tasks::can_data::live_data_handler::run_live_data_handler;
use crate::tasks::can_data::decoded_item::{DecodedItem, DecodedMarker};
use crate::utils::yellow;
use crate::tasks::{HealthCheckSender, HealthCheckSenderExt, Task};
use crate::tasks::telem_message::CanPayload;
use crate::tasks::client_api::subtable_clients::Clients;
use crate::{error_println, vprintln};

use jsoncan_rust::can_database::CanDatabase;

fn decode_items_from_payload(can_db: &CanDatabase, can_payload: CanPayload) -> Vec<DecodedItem> {
    let decoded_signals = can_db.unpack(
        can_payload.can_id,
        can_payload.payload,
        Some(can_payload.can_timestamp),
    );

    if decoded_signals.is_empty() {
        if let Ok(message) = can_db.get_message_by_id(can_payload.can_id) && message.signals.is_empty() {
            return vec![DecodedItem::Marker(DecodedMarker {
                name: message.name,
                timestamp: can_payload.can_timestamp,
            })];
        }
    }

    decoded_signals
        .into_iter()
        .map(DecodedItem::Signal)
        .collect()
}

/**
 * Consumes from serial handler
 * Uses JsonCan config to parse CAN messages and broadcasts to other tasks
 */
pub async fn run_can_data_handler(
    mut shutdown_rx: broadcast::Receiver<()>, 
    health_check_tx: HealthCheckSender,
    mut can_queue_rx: broadcast::Receiver<CanPayload>,
    clients: Arc<RwLock<Clients>>,
    can_db: Arc<CanDatabase>
) {
    vprintln!("{}", yellow("CAN data task started."));

    let (decoded_signal_tx, _) = broadcast::channel::<DecodedItem>(4096);

    // parsed can signal consumers
    let influx_handler_task: tokio::task::JoinHandle<()> = spawn(run_influx_handler(shutdown_rx.resubscribe(), health_check_tx.clone(), decoded_signal_tx.subscribe()));
    let live_data_handler_task: tokio::task::JoinHandle<()> = spawn(run_live_data_handler(shutdown_rx.resubscribe(), health_check_tx.clone(), decoded_signal_tx.subscribe(), clients));
    
    health_check_tx.send_health_check(Task::CanDataHandler, true).await;
    
    'outer: loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("CAN data task shutting down.");
                break;
            }
            Ok(can_payload) = can_queue_rx.recv() => {
                let decoded_items = decode_items_from_payload(&can_db, can_payload);
                for item in decoded_items {
                    if !decoded_signal_tx.send(item).is_ok() {
                        error_println!("Parsed can data signal consumers are all closed");
                        break 'outer;
                    }
                }
            }
        }
    }
    drop(decoded_signal_tx); // Closing channel signals the other tasks to shutdown
    let _ = influx_handler_task.await;
    let _ = live_data_handler_task.await;
    vprintln!("{}", yellow("CAN data task ended."));
}

#[cfg(test)]
mod tests {
    use crate::tasks::can_data::load_can_database;

    use super::*;

    #[test]
    fn telem_mark_event_becomes_marker_item() {
        let can_db = load_can_database().expect("CAN DB should load for tests");
        let telem_mark_message = can_db
            .get_all_msgs()
            .expect("Messages should load")
            .into_iter()
            .find(|msg| msg.name.ends_with("TelemMarkEvent"))
            .expect("TelemMarkEvent should exist in configured CAN DB");

        let decoded_items = decode_items_from_payload(
            &can_db,
            CanPayload {
                can_id: telem_mark_message.id,
                can_timestamp: 1_000,
                payload: vec![],
            },
        );

        assert_eq!(decoded_items.len(), 1);
        match &decoded_items[0] {
            DecodedItem::Marker(marker) => {
                assert!(marker.name.ends_with("TelemMarkEvent"));
                assert_eq!(marker.timestamp, 1_000);
            }
            DecodedItem::Signal(_) => panic!("Expected zero-signal TelemMarkEvent to decode as Marker"),
        }
    }

    #[test]
    fn non_zero_signal_message_decodes_as_signal_item() {
        let can_db = load_can_database().expect("CAN DB should load for tests");
        let message_with_signal = can_db
            .get_all_msgs()
            .expect("Messages should load")
            .into_iter()
            .find(|msg| !msg.signals.is_empty())
            .expect("At least one message should have signals");

        let decoded_items = decode_items_from_payload(
            &can_db,
            CanPayload {
                can_id: message_with_signal.id,
                can_timestamp: 2_000,
                payload: vec![0; 8],
            },
        );

        assert!(
            decoded_items
                .iter()
                .any(|item| matches!(item, DecodedItem::Signal(_))),
            "Expected at least one decoded signal item",
        );
    }
}

