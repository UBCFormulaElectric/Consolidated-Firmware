use tokio::select;
use tokio::io::{AsyncReadExt, AsyncWriteExt, ReadHalf, WriteHalf};
use tokio::sync::{broadcast, mpsc};
use tokio::time::sleep;
use tokio_serial::{Error as SerialOpenError, ErrorKind as SerialOpenErrorKind, SerialPortBuilderExt, SerialStream};
use std::io::{Error, ErrorKind, ErrorKind as IoErrorKind};
use std::time::{Duration, SystemTime, UNIX_EPOCH};

use crate::config::CONFIG;
use crate::tasks::{HealthCheckSender, HealthCheckSenderExt, ResultExt, Task};
use crate::tasks::telem_message::{CRC32_CALC, TelemetryOutgoingMessage};
#[allow(unused_imports)]
use crate::utils::yellow;
use crate::utils::green;
use crate::vprintln;
use super::telem_message::{TelemetryIncomingMessage, CanPayload};

const ENXIO_OS_ERROR_CODE: i32 = 6;
const SERIAL_RECONNECT_DELAY: Duration = Duration::from_millis(1000);

#[derive(Clone, Copy)]
enum SerialFatalEvent {
    Disconnect,
}

/**
 * Handling serial signals from radio. Main task
 */
pub async fn run_serial_task(
    mut shutdown_rx: broadcast::Receiver<()>, 
    health_check_tx: HealthCheckSender,
    can_queue_tx: broadcast::Sender<CanPayload>
) {
    vprintln!("{}", yellow("Serial handler task started."));

    let mut health_check_sent = false;
    let mut should_shutdown = false;
    let mut reconnect_pending = false;

    while !should_shutdown {
        let serial_open_result = tokio_serial::new(
            &CONFIG.serial_port,
            CONFIG.serial_baud_rate
        )
        .timeout(Duration::from_millis(1000)) // i love magic numbers
        .open_native_async();

        let serial_port = match serial_open_result {
            Ok(port) => {
                if reconnect_pending {
                    println!("{}", green("Serial reconnected successfully."));
                    reconnect_pending = false;
                }
                port
            }
            Err(e) if is_enxio_open_error(&e) => {
                eprintln!("Serial port disconnected while opening ({e}); retrying...");
                reconnect_pending = true;
                should_shutdown = wait_for_reconnect_delay_or_shutdown(&mut shutdown_rx).await;
                continue;
            }
            Err(e) => {
                Err::<SerialStream, _>(e)
                    .unwrap_or_fail_health_check(&health_check_tx, Task::SerialHandler)
                    .await
            }
        };

        if !health_check_sent {
            // no set up involved with packet sender and reader thread, send health check once.
            health_check_tx.send_health_check(Task::SerialHandler, true).await;
            health_check_sent = true;
        }

        // split serial port into respective reads and writes
        let (serial_read, serial_write) = tokio::io::split(serial_port);

        // communicate between serial reader and this function
        let (in_packet_tx, mut in_packet_rx) = mpsc::channel::<Vec<u8>>(32);
        // communicate between this function and serial sender
        let (out_packet_tx, out_packet_rx) = mpsc::channel::<TelemetryOutgoingMessage>(32);
        // notify main session loop of fatal serial errors
        let (fatal_event_tx, mut fatal_event_rx) = mpsc::channel::<SerialFatalEvent>(4);
        // local shutdown for a single serial session
        let (session_shutdown_tx, _) = broadcast::channel::<()>(1);

        // spawn packet reader and sender
        let packet_reader = {
            let shutdown_rx = shutdown_rx.resubscribe();
            let session_shutdown_rx = session_shutdown_tx.subscribe();
            let fatal_event_tx = fatal_event_tx.clone();
            tokio::spawn(packet_reader_handler(
                shutdown_rx,
                session_shutdown_rx,
                serial_read,
                in_packet_tx,
                fatal_event_tx
            ))
        };
        let packet_sender = {
            let shutdown_rx = shutdown_rx.resubscribe();
            let session_shutdown_rx = session_shutdown_tx.subscribe();
            tokio::spawn(packet_sender_handler(
                shutdown_rx,
                session_shutdown_rx,
                serial_write,
                out_packet_rx,
                fatal_event_tx
            ))
        };

        let mut should_reconnect = false;
        // loop select check for shutdown signal
        // if shutdown signal, select block breaks loop early
        loop {
            select! {
                _ = shutdown_rx.recv() => {
                    vprintln!("Shutting down serial task.");
                    should_shutdown = true;
                    break;
                }
                Some(fatal_event) = fatal_event_rx.recv() => {
                    match fatal_event {
                        SerialFatalEvent::Disconnect => {
                            eprintln!("Serial disconnect detected; restarting serial session.");
                            should_reconnect = true;
                            break;
                        }
                    }
                }
                maybe_packet = in_packet_rx.recv() => {
                    let packet = match maybe_packet {
                        Some(packet) => packet,
                        None => {
                            // Reader ended unexpectedly; restart the serial session.
                            eprintln!("Serial reader stopped; restarting serial session.");
                            should_reconnect = true;
                            break;
                        }
                    };

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
                            println!(
                                "Backend ingest CAN: id=0x{:03x} ts={} payload_len={} payload={:02x?}",
                                body.can_id,
                                body.can_timestamp,
                                body.payload.len(),
                                body.payload
                            );
                            // TODO error handling
                            if !can_queue_tx.send(body).is_ok() {
                                eprintln!("CAN queue channel has closed.");
                                should_shutdown = true;
                                break;
                            };
                        },
                        TelemetryIncomingMessage::NTP => {
                            println!("Backend ingest NTP request");
                            let t1 = SystemTime::now()
                                .duration_since(UNIX_EPOCH)
                                .unwrap();
                            // DEBUG: show the host system clock at the moment of t1
                            // capture. If this is small (< 1e12 ms), the backend
                            // machine's wall clock is wrong, not the DAM's parser.
                            // println!(
                            //     "NTP request: t1_capture={} ms ({})",
                            //     t1.as_millis(),
                            //     format_unix_ms(t1.as_millis() as u64),
                            // );
                            if !out_packet_tx.send(TelemetryOutgoingMessage::NTP { t1 }).await.is_ok() {
                                eprintln!("Serial sender channel has closed.");
                                should_reconnect = true;
                                break;
                            };
                        }
                    }
                }
                else => {
                    should_reconnect = true;
                    break;
                }
            }
        }

        let _ = session_shutdown_tx.send(());
        drop(out_packet_tx);
        packet_reader.await.ok();
        packet_sender.await.ok();

        if should_shutdown {
            break;
        }

        if should_reconnect {
            reconnect_pending = true;
            should_shutdown = wait_for_reconnect_delay_or_shutdown(&mut shutdown_rx).await;
        }
    }

    vprintln!("{}", yellow("Serial handler task ended."));
}

fn is_enxio_disconnect(error: &Error) -> bool {
    error.raw_os_error() == Some(ENXIO_OS_ERROR_CODE)
}

fn is_enxio_open_error(error: &SerialOpenError) -> bool {
    match error.kind() {
        SerialOpenErrorKind::NoDevice => error.to_string().contains("os error 6"),
        SerialOpenErrorKind::Io(IoErrorKind::NotFound) => true,
        _ => false,
    }
}

async fn wait_for_reconnect_delay_or_shutdown(
    shutdown_rx: &mut broadcast::Receiver<()>
) -> bool {
    select! {
        _ = shutdown_rx.recv() => true,
        _ = sleep(SERIAL_RECONNECT_DELAY) => false,
    }
}

/**
 * Intakes incoming packet bytes and parses into telemetry incoming message
 */
fn parse_incoming_telem_message(payload: Vec<u8>) -> Result<TelemetryIncomingMessage, ()> {
    let parsed_message: TelemetryIncomingMessage = match payload[0] {
        TelemetryIncomingMessage::CAN_BYTE => {
            let can_id = u32::from_le_bytes([payload[1], payload[2], payload[3], payload[4]]);
            let can_timestamp = u64::from_le_bytes(
                [payload[5], payload[6], payload[7], payload[8],
                payload[9], payload[10], payload[11], payload[12]]
            );

            let can_payload = payload[13..].to_vec();
            TelemetryIncomingMessage::Can {
                body: CanPayload {
                    can_id: can_id,
                    can_timestamp: can_timestamp,
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

/**
 * Subhandler that manages packet reading, sends packet bytes to main task
 */
async fn packet_reader_handler(
    mut shutdown_flag: broadcast::Receiver<()>, 
    mut session_shutdown_flag: broadcast::Receiver<()>,
    mut serial_read: ReadHalf<SerialStream>, 
    packet_tx: mpsc::Sender<Vec<u8>>,
    fatal_event_tx: mpsc::Sender<SerialFatalEvent>
) {   
    loop {
        select! {
            _ = shutdown_flag.recv() => break,
            _ = session_shutdown_flag.recv() => break,
            result = read_packet(&mut serial_read) => {
                match result {
                    Ok(p) => match packet_tx.send(p).await {
                        Ok(_) => {},
                        Err(_) => break, // packet receiver closed, exit thread
                    },
                    Err(e) if is_enxio_disconnect(&e) => {
                        let _ = fatal_event_tx.send(SerialFatalEvent::Disconnect).await;
                        break;
                    }
                    Err(e) => {eprintln!("{e}")}, // failed to read, continue
                }
            }
        }
    }
}

/**
 * Reading bytes until legitimate packet is formed
 */
async fn read_packet(serial_read: &mut ReadHalf<SerialStream>) -> Result<Vec<u8>, Error> {
    let mut header_buffer = [0x0; TelemetryIncomingMessage::HEADER_SIZE];
    let mut index = 0;

    // keep reading until magic bytes
    loop {
        while index < 2 {
            match serial_read.read(&mut header_buffer[index..]).await {
                Ok(0) => {
                    return Err(Error::new(
                        ErrorKind::UnexpectedEof,
                        "Serial stream returned EOF",
                    ));
                }
                Ok(n) => index += n,
                Err(e) => return Err(e)
            }
        }
        if header_buffer[0..2] == TelemetryIncomingMessage::MAGIC {
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
    if payload_length > TelemetryIncomingMessage::MAX_PAYLOAD_SIZE {
        // TODO wow this is horrible, probably just log and return generic error
        return Err(Error::new(
            ErrorKind::InvalidData,
            "Payload length exceeds maximum",
        ));
    }

    if payload_length == 0 {
        return Err(Error::new(
            ErrorKind::InvalidData,
            "Payload length is 0",
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
async fn packet_sender_handler(
    mut shutdown_flag: broadcast::Receiver<()>, 
    mut session_shutdown_flag: broadcast::Receiver<()>,
    mut serial_write: WriteHalf<SerialStream>, 
    mut out_packet_rx: mpsc::Receiver<TelemetryOutgoingMessage>,
    fatal_event_tx: mpsc::Sender<SerialFatalEvent>
) {
    loop {
        select! {
            _ = shutdown_flag.recv() => break,
            _ = session_shutdown_flag.recv() => break,
            maybe_outgoing = out_packet_rx.recv() => {
                let outgoing = match maybe_outgoing {
                    Some(outgoing) => outgoing,
                    None => break,
                };
                let packet = generate_packet(outgoing);
                match serial_write.write_all(&packet).await {
                    Ok(_) => {},
                    Err(e) if is_enxio_disconnect(&e) => {
                        let _ = fatal_event_tx.send(SerialFatalEvent::Disconnect).await;
                        break;
                    }
                    Err(e) => {
                        eprintln!("Failed to send packet: {e}");
                    }
                }
            }
        }
    }
}

/**
 * See `TelemetryOutgoingMessage` to see how packets are generated
 */
fn generate_packet(message: TelemetryOutgoingMessage) -> Vec<u8> {
    let mut payload = Vec::<u8>::new();

    match message {
        TelemetryOutgoingMessage::NTP{ t1 } => {
            payload.push(TelemetryOutgoingMessage::NTP_BYTE);

            let t1_u64 = t1.as_millis() as u64;
            let t1_millis = t1_u64.to_le_bytes();
            payload.extend_from_slice(&t1_millis);

            // t2 handled here to more accurately reflect time difference between received and sent
            let t2_u64 = SystemTime::now()
                .duration_since(UNIX_EPOCH)
                .unwrap()
                .as_millis() as u64;
            let t2_millis = t2_u64.to_le_bytes();
            payload.extend_from_slice(&t2_millis);

            // DEBUG: log what we're actually sending so we can compare against
            // what the firmware reads. Expect both to be ~1.747e12 in 2026; if
            // they look small, the host system clock is wrong.
            println!(
                "NTP reply: t1={} ms ({}) t2={} ms ({}) | t1_bytes_le={:02x?} t2_bytes_le={:02x?}",
                t1_u64,
                format_unix_ms(t1_u64),
                t2_u64,
                format_unix_ms(t2_u64),
                t1_millis,
                t2_millis,
            );
        }
    }

    let length: [u8; 1] = [payload.len() as u8];
    let checksum: [u8; 4] = CRC32_CALC.checksum(&payload).to_le_bytes();

    let mut packet = Vec::new();
    packet.extend_from_slice(&TelemetryOutgoingMessage::MAGIC);
    packet.extend_from_slice(&length);
    packet.extend_from_slice(&checksum);
    packet.extend_from_slice(&payload);

    return packet;
}

/// Format a Unix-epoch milliseconds value as a rough "Y-M-D h:m:s UTC" string
/// using day/time arithmetic only — no chrono dependency required. Good enough
/// for sanity-checking that a timestamp is in the right ballpark.
fn format_unix_ms(ms: u64) -> String {
    let secs = ms / 1000;
    let days = secs / 86_400;
    let sod  = secs % 86_400;
    let h    = sod / 3600;
    let m    = (sod % 3600) / 60;
    let s    = sod % 60;

    // Howard Hinnant civil_from_days, inlined.
    let z = days as i64 + 719_468;
    let era = if z >= 0 { z / 146_097 } else { (z - 146_096) / 146_097 };
    let doe = (z - era * 146_097) as u64;
    let yoe = (doe - doe / 1460 + doe / 36524 - doe / 146_096) / 365;
    let y_civil = yoe as i64 + era * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let d = doy - (153 * mp + 2) / 5 + 1;
    let mo = if mp < 10 { mp + 3 } else { mp - 9 };
    let y = if mo <= 2 { y_civil + 1 } else { y_civil };

    format!("{:04}-{:02}-{:02} {:02}:{:02}:{:02} UTC", y, mo, d, h, m, s)
}
