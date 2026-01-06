use serialport::SerialPort;
use tokio::select;
use tokio::sync::broadcast;
use tokio::sync::mpsc;
use std::io::{Error, ErrorKind};
use std::time::Duration;
use crc::{Crc, CRC_32_ISCSI};

use crate::config::CONFIG;
use super::telem_message::{TelemetryMessage, CanMessage, NTPTimeMessage, NTPDateMessage, BaseTimeRegMessage};

/**
 * Handling serial signals from radio.
 */
pub async fn run_serial_task(mut shutdown_rx: broadcast::Receiver<()>, can_queue_sender: broadcast::Sender<CanMessage>) {
    let (packet_tx, mut packet_rx) = mpsc::channel::<Vec<u8>>(32);

    println!("Serial task started.");

    // spawn blocking packet reader
    // the reader thread will allow the handler thread to be async
    let packet_reader = tokio::task::spawn_blocking(move || packet_reader_handler(packet_tx));

    // loop select check for shutdown signal
    // if shutdown signal, select block breaks loop early
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                println!("Shutting down serial task.");
                break;
            }
            Some(packet) = packet_rx.recv() => {
                // todo should also probably check for closed channels and close thread
                // TODO better handling error
                let telem_message = match parse_telem_message(packet) {
                    Ok(m)   => m,
                    Err(_)  => continue,
                };

                match telem_message {
                    TelemetryMessage::Can { body } => {
                        let CanMessage { can_id, can_time_offset, can_payload } = &body;
                        println!("Received CAN Message: ID: {}, Time Offset: {}, Payload: {:?}", can_id, can_time_offset, can_payload);
                        // TODO error handling
                        can_queue_sender.send(body).unwrap();
                    },
                    TelemetryMessage::NTPTime { body } => {
                        // TODO handle NTP time message
                    },
                    TelemetryMessage::NTPDate { body } => {
                        // TODO handle NTP date message
                    },
                    TelemetryMessage::BaseTimeReg { body } => {
                        // TODO handle base time register message
                    },
                }
            }
        }
    }

    packet_reader.await.ok();
}

const MAGIC: [u8; 2] = [0xaa, 0x55];
const HEADER_SIZE: usize = 7;
const MAX_PAYLOAD_SIZE: usize = 100;
// calc is short for calculator
// for those new to the chat
const CRC32_CALC: Crc<u32> = Crc::<u32>::new(&CRC_32_ISCSI);

/**
 * Blocking handler that reads packets and sends it to the handler
 * This separate blocking thread is needed for optimal multithreading with tokio
 */
fn packet_reader_handler(packet_tx: mpsc::Sender<Vec<u8>>) {   
    let mut serial_port = serialport::new(
        &CONFIG.serial_port,
        CONFIG.serial_baud_rate
    )
    .timeout(Duration::from_millis(100)) // i love magic numbers
    .open()
    .expect("Failed to open serial port");

    loop {
        match read_packet(&mut serial_port) {
            // I don't think blocking send is an issue here.. probably...
            Ok(p) => {
                match packet_tx.blocking_send(p) {
                    Ok(_) => {},
                    Err(_) => break, // receiver has closed, exit thread
                }
            },
            Err(_)  => continue,
        };
    }
}

fn read_packet(serial_port: &mut Box<dyn SerialPort>) -> Result<Vec<u8>, Error> {
    let mut header_buffer = [0x0; HEADER_SIZE];
    let mut index = 0;
    // identify start of header
    // keep reading until we find the magic bytes

    loop {
        while index < 2 {
            // TODO timeout retry/error handling
            index += serial_port.read(&mut header_buffer[index..])?
        }
        if header_buffer[0..2] == MAGIC {
            break;
        }
        header_buffer.rotate_left(1);
        index -= 1;
    }
    // TODO should these be infinite loop?
    while index < HEADER_SIZE {
        // TODO error handling
        index += serial_port.read(&mut header_buffer[index..])?
    }

    let payload_length = header_buffer[2] as usize;
    // assert payload length is less than max
    if payload_length > MAX_PAYLOAD_SIZE {
        // TODO wow this is horrible, probably just log and return generic error
        return Err(Error::new(
            ErrorKind::InvalidData,
            "Payload length exceeds maximum",
        ));
    }

    index = 0;
    let mut payload_buffer = vec![0u8; payload_length];
    while index < payload_length {
        // TODO error handling
        index += serial_port.read(&mut payload_buffer[index..])?
    }

    // checksum, probably works
    let expected_crc = u32::from_le_bytes(header_buffer[3..7].try_into().unwrap_or_default());
    let calculated_crc: u32 = CRC32_CALC.checksum(&payload_buffer);

    if expected_crc != calculated_crc {
        return Err(Error::new(
            ErrorKind::InvalidData,
            "CRC mismatch",
        ));
    }

    return Ok(payload_buffer);
}

/**
 * 
 */
fn parse_telem_message(payload: Vec<u8>) -> Result<TelemetryMessage, ()> {
    let parsed_message: TelemetryMessage = match payload[0] {
        // TODO magic numbers
        0x01 => {
            let can_id = u32::from_le_bytes([payload[1], payload[2], payload[3], payload[4]]);
            // TODO use RTC and NTP time instead of whatever this is
            let can_time_offset = f32::from_le_bytes([payload[5], payload[6], payload[7], payload[8]]);
            let can_payload = payload[9..].to_vec();
            TelemetryMessage::Can {
                body: CanMessage {
                    can_id,
                    can_time_offset,
                    can_payload,
                }
            }
        },
        0x02 => TelemetryMessage::NTPTime {
            body: NTPTimeMessage,
        },
        0x03 => TelemetryMessage::NTPDate {
            body: NTPDateMessage,
        },
        0x04 => {
            let base_time = u32::from_le_bytes([payload[1], payload[2], payload[3], payload[4]]);
            TelemetryMessage::BaseTimeReg {
                body: BaseTimeRegMessage {
                    base_time,
                }
            }
        },
        _ => return Err(()),
    };

    return Ok(parsed_message);
}
