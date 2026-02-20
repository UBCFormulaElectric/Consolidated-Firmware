use tokio::select;
use tokio::io::{AsyncReadExt, AsyncWriteExt, ReadHalf, WriteHalf};
use tokio::sync::{broadcast, mpsc};
use tokio_serial::{SerialPortBuilderExt, SerialStream};
use std::io::{Error, ErrorKind};
use std::time::{Duration, SystemTime, UNIX_EPOCH};
use crc::{Crc, CRC_32_ISO_HDLC};

use crate::config::CONFIG;
use crate::tasks::telem_message::TelemetryOutgoingMessage;
use super::telem_message::{TelemetryIncomingMessage, CanPayload};

/**
 * Handling serial signals from radio. Main task
 */
pub async fn run_serial_task(mut shutdown_rx: broadcast::Receiver<()>, can_queue_sender: broadcast::Sender<CanPayload>) {
    println!("Serial handler task started.");

    let serial_port = tokio_serial::new(
        &CONFIG.serial_port,
        CONFIG.serial_baud_rate
    )
    .timeout(Duration::from_millis(1000)) // i love magic numbers
    .open_native_async()
    .expect("Failed to open serial port");

    // split serial port into respective reads and writes
    let (serial_read, serial_write) = tokio::io::split(serial_port);
    
    // communicate between serial reader and this function
    let (in_packet_tx, mut in_packet_rx) = mpsc::channel::<Vec<u8>>(32);
    // communicate between this function and serial sender
    let (out_packet_tx, out_packet_rx) = mpsc::channel::<TelemetryOutgoingMessage>(32);

    // spawn blocking packet reader
    // the reader thread will allow the handler thread to be async
    let packet_reader = {
        let shutdown_rx = shutdown_rx.resubscribe();
        tokio::spawn(packet_reader_handler(shutdown_rx, serial_read, in_packet_tx))
    };
    let packet_sender = {
        let shutdown_rx = shutdown_rx.resubscribe();
        tokio::spawn(packet_sender_handler(shutdown_rx, serial_write, out_packet_rx))
    };

    // loop select check for shutdown signal
    // if shutdown signal, select block breaks loop early
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                println!("Shutting down serial task.");
                break;
            }
            Some(packet) = in_packet_rx.recv() => {
                // todo should also probably check for closed channels and close thread
                // TODO better handling error
                // println!("Received packet: {:x?}", &packet);
                let telem_message = match parse_incoming_telem_message(packet) {
                    Ok(m)   => m,
                    Err(_)  => {
                        eprintln!("Failed to parse telemetry message.");
                        continue;
                    },
                };

                match telem_message {
                    TelemetryIncomingMessage::Can { body } => {
                        // TODO error handling
                        if !can_queue_sender.send(body).is_ok() {
                            eprintln!("Channel has closed");
                            break;
                        };
                    },
                    TelemetryIncomingMessage::NTP => {
                        let t1 = SystemTime::now()
                            .duration_since(UNIX_EPOCH)
                            .unwrap();
                        if !out_packet_tx.send(TelemetryOutgoingMessage::NTP { t1 }).await.is_ok() {
                            eprintln!("Channel has closed");
                            break;
                        };
                    }
                }
            }
        }
    }
    packet_reader.await.ok();
    packet_sender.await.ok();
    println!("Serial handler task ended.");
}

/**
 * Intakes incoming packet bytes and parses into telemetry incoming message
 */
fn parse_incoming_telem_message(payload: Vec<u8>) -> Result<TelemetryIncomingMessage, ()> {
    let parsed_message: TelemetryIncomingMessage = match payload[0] {
        // TODO magic numbers
        TelemetryIncomingMessage::CAN_BYTE => {
            let can_id = u32::from_le_bytes([payload[1], payload[2], payload[3], payload[4]]);
            // TODO use RTC and NTP time instead of whatever this is
            // this should also probably be u64 for epoch unix time, awaiting confirmation
            let can_time_offset = f32::from_le_bytes([payload[5], payload[6], payload[7], payload[8]]);
            let timestamp = SystemTime::now(); // something with can_time_offset

            let can_payload = payload[9..].to_vec();
            TelemetryIncomingMessage::Can {
                body: CanPayload {
                    can_id: can_id,
                    can_timestamp: timestamp,
                    payload: can_payload,
                }
            }
        },
        TelemetryIncomingMessage::NTP_BYTE => TelemetryIncomingMessage::NTP,
        invalid => {
            eprintln!("Invalid message type: {invalid}");
            return Err(());
        },
    };

    return Ok(parsed_message);
}

//
// Serial reading stuff
//

// These magic bytes are chosen so that the pattern of the bits 
// wouldn't be confused with each other
const MAGIC_INCOMING: [u8; 2] = [0xaa, 0x55];
const MAGIC_OUTGOING: [u8; 2] = [0xcc, 0x33];
// 2 for magic bytes, 1 for length, 4 for checksum
const HEADER_SIZE: usize = 7;
const MAX_PAYLOAD_SIZE: usize = 100;
// calc is short for calculator for those new to the chat
const CRC32_CALC: Crc<u32> = Crc::<u32>::new(&CRC_32_ISO_HDLC);

/**
 * Subhandler that manages packet reading, sends packet bytes to main task
 */
async fn packet_reader_handler(mut shutdown_flag: broadcast::Receiver<()>, mut serial_read: ReadHalf<SerialStream>, packet_tx: mpsc::Sender<Vec<u8>>) {   
    loop {
        select! {
            _ = shutdown_flag.recv() => break,
            result = read_packet(&mut serial_read) => {
                match result {
                    Ok(p) => match packet_tx.send(p).await {
                        Ok(_) => {},
                        Err(_) => break, // packet receiver closed, exit thread
                    },
                    Err(e) => {println!("{e}")}, // failed to read, continue
                }
            }
        }
    }
}

/**
 * Reading bytes until legitimate packet is formed
 */
async fn read_packet(serial_read: &mut ReadHalf<SerialStream>) -> Result<Vec<u8>, Error> {
    let mut header_buffer = [0x0; HEADER_SIZE];
    let mut index = 0;

    // keep reading until magic bytes
    loop {
        while index < 2 {
            match serial_read.read(&mut header_buffer[index..]).await {
                Ok(n) => index += n,
                Err(e) => return Err(e)
            }
        }
        if header_buffer[0..2] == MAGIC_INCOMING {
            break;
        }
        // if magic byte not matched, rotate buffer
        header_buffer.rotate_left(1);
        index -= 1;
    }

    // magic bytes have matched, make sure that the rest of the header has been fully read
    serial_read.read_exact(&mut header_buffer[index..]).await?;

    // header has been read, now read until payload is fully read
    let payload_length = header_buffer[2] as usize;
    // assert payload length is less than max
    if payload_length > MAX_PAYLOAD_SIZE {
        // TODO wow this is horrible, probably just log and return generic error
        return Err(Error::new(
            ErrorKind::InvalidData,
            "Payload length exceeds maximum",
        ));
    }

    // read payload buffer
    let mut payload_buffer = vec![0u8; payload_length];
    serial_read.read_exact(&mut payload_buffer).await?;

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
 * Subhandler that manages packet sending, handles outgoing telemetry messages from main task
 */
async fn packet_sender_handler(mut shutdown_flag: broadcast::Receiver<()>, mut serial_write: WriteHalf<SerialStream>, mut out_packet_rx: mpsc::Receiver<TelemetryOutgoingMessage>) {
    loop {
        select! {
            _ = shutdown_flag.recv() => break,
            Some(outgoing) = out_packet_rx.recv() => {
                let packet = generate_packet(outgoing);
                match serial_write.write_all(&packet).await {
                    Ok(_) => {},
                    Err(e) => {
                        eprintln!("Failed to send packet: {e}");
                    }
                }
            }
        }
    }
}

/**
 * Generate telem packet bytes in the form of
 * [
 *  MAGIC BYTE 1
 *  MAGIC BYTE 2
 *  LENGTH OF PAYLOAD
 *  CHECK SUM 1
 *  CHECK SUM 2
 *  CHECK SUM 3
 *  CHECK SUM 4
 *  PAYLOAD ...
 *  MESSAGE TYPE BYTE (usually first byte of payload)
 * ]
 */
fn generate_packet(message: TelemetryOutgoingMessage) -> Vec<u8> {
    let mut payload = Vec::<u8>::new();

    match message {
        TelemetryOutgoingMessage::NTP{ t1 } => {
            payload.push(TelemetryOutgoingMessage::NTP_BYTE);

            let t1_millis = (t1.as_millis() as u64).to_le_bytes();
            payload.extend_from_slice(&t1_millis);

            // t2 handled here to more accurately reflect time difference between received and sent
            let t2_millis = (SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .unwrap()
                .as_millis() as u64)
                .to_le_bytes();
            payload.extend_from_slice(&t2_millis);
        }
    }

    let length: [u8; 1] = [payload.len() as u8];
    let checksum: [u8; 4] = CRC32_CALC.checksum(&payload).to_le_bytes();

    let mut packet = Vec::new();
    packet.extend_from_slice(&MAGIC_OUTGOING);
    packet.extend_from_slice(&length);
    packet.extend_from_slice(&checksum);
    packet.extend_from_slice(&payload);
    
    return packet;
}
