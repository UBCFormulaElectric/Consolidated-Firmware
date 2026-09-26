use tokio::select;
use tokio::io::{AsyncReadExt, AsyncWriteExt, ReadHalf, WriteHalf};
use tokio::sync::{broadcast, mpsc};
use tokio_serial::{SerialPortBuilderExt, SerialStream};
use std::collections::VecDeque;
use std::time::{Duration, SystemTime, UNIX_EPOCH};

const ERROR_RATE_WINDOW_SECS: usize = 60;

use crate::config::CONFIG;
use crate::tasks::{HealthCheckSender, HealthCheckSenderExt, ResultExt, Task};
use crate::tasks::telem_message::{CRC32_CALC, TelemetryOutgoingMessage};
use crate::utils::yellow;
use crate::{dprintln, vprintln};
use super::telem_message::{TelemetryIncomingMessage, CanPayload};

/**
 * Internal enum for communicating packet status from reader handler
 */
enum SerialReaderOutput {
    Message(TelemetryIncomingMessage),
    InvalidData,
}

/**
 * Handling serial signals from radio. Main task
 */
pub async fn run_serial_task(
    mut shutdown_rx: broadcast::Receiver<()>,
    health_check_tx: HealthCheckSender,
    can_queue_tx: broadcast::Sender<CanPayload>,
    diag_tx: broadcast::Sender<f64>,
    client_out_msg_rx: broadcast::Receiver<TelemetryOutgoingMessage>,
) {
    vprintln!("{}", yellow("Serial handler task started."));

    let serial_port = tokio_serial::new(
        &CONFIG.serial_port,
        CONFIG.serial_baud_rate
    )
    .timeout(Duration::from_millis(1000)) // i love magic numbers
    .open_native_async()
    .unwrap_or_fail_health_check(&health_check_tx, Task::SerialHandler).await;

    // split serial port into respective reads and writes
    let (serial_read, serial_write) = tokio::io::split(serial_port);
    
    // communicate between serial reader and this function
    let (in_msg_tx, mut in_msg_rx) = mpsc::channel::<SerialReaderOutput>(32);
    // communicate between this function and serial sender
    let (out_msg_tx, out_msg_rx) = mpsc::channel::<TelemetryOutgoingMessage>(32);

    // spawn blocking packet reader
    // the reader thread will allow the handler thread to be async
    let mut packet_reader = {
        let shutdown_rx = shutdown_rx.resubscribe();
        tokio::spawn(packet_reader_handler(shutdown_rx, serial_read, in_msg_tx))
    };
    let packet_sender = {
        let shutdown_rx = shutdown_rx.resubscribe();
        tokio::spawn(packet_sender_handler(shutdown_rx, serial_write, out_msg_rx, client_out_msg_rx))
    };

    // metrics: per-second counts rolled into a 60s window for error rate
    let mut success_count = 0u64;
    let mut error_count = 0u64;
    let mut error_rate_history: VecDeque<(u64, u64)> =
        VecDeque::with_capacity(ERROR_RATE_WINDOW_SECS);
    let mut diag_interval = tokio::time::interval(Duration::from_secs(1));

    // no set up involved with packet sender and reader thread, send health check
    health_check_tx.send_health_check(Task::SerialHandler, true).await;

    // loop select check for shutdown signal
    // if shutdown signal, select block breaks loop early
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("Shutting down serial task.");
                break;
            }
            res = &mut packet_reader => {
                panic!("packet_reader exited unexpectedly: {res:?}");
            }
            _ = diag_interval.tick() => {
                error_rate_history.push_back((success_count, error_count));
                if error_rate_history.len() > ERROR_RATE_WINDOW_SECS {
                    error_rate_history.pop_front();
                }
                success_count = 0;
                error_count = 0;

                let window_success: u64 = error_rate_history.iter().map(|(s, _)| s).sum();
                let window_error: u64 = error_rate_history.iter().map(|(_, e)| e).sum();
                let window_total = window_success + window_error;
                let error_rate = if window_total > 0 {
                    (window_error as f64 / window_total as f64) * 100.0
                } else {
                    0.0
                };
                diag_tx.send(error_rate).ok();
            }
            Some(out) = in_msg_rx.recv() => {
                match out {
                    SerialReaderOutput::InvalidData => {
                        error_count += 1;
                    },
                    SerialReaderOutput::Message(msg) => {
                        success_count += 1;
                        match msg {
                            TelemetryIncomingMessage::Can { body } => {
                                // TODO error handling
                                dprintln!(
                                    "Backend ingest CAN: id=0x{:03x} ts={} payload_len={} payload={:02x?}",
                                    body.can_id,
                                    body.can_timestamp,
                                    body.payload.len(),
                                    body.payload
                                );
                                if !can_queue_tx.send(body).is_ok() {
                                    eprintln!("Channel has closed");
                                    break;
                                };
                            },
                            TelemetryIncomingMessage::NTP => {
                                vprintln!("Backend ingest NTP request");
                                let t1 = SystemTime::now()
                                    .duration_since(UNIX_EPOCH)
                                    .unwrap();
                                if !out_msg_tx.send(TelemetryOutgoingMessage::NtpResponse { t1 }).await.is_ok() {
                                    eprintln!("Channel has closed");
                                    break;
                                };
                            }
                        }
                    }
                }
            }
        }
    }
    packet_reader.await.ok();
    packet_sender.await.ok();
    vprintln!("{}", yellow("Serial handler task ended."));
}

//
// Serial reading stuff
//

/**
 * Subhandler that manages packet reading, sends packet bytes to main task
 */
async fn packet_reader_handler(
    mut shutdown_flag: broadcast::Receiver<()>,
    mut serial_read: ReadHalf<SerialStream>,
    in_msg_tx: mpsc::Sender<SerialReaderOutput>
) {
    let mut decoder = FrameDecoder::default();
    let mut read_buf = [0u8; 1024];
    'outer: loop {
        // hand off every complete frame already buffered before reading more
        loop {
            match decoder.decode() {
                DecodeResult::Frame(packet_bytes) => {
                    if let Ok(parsed) = parse_incoming_telem_message(&packet_bytes) {
                        if in_msg_tx.send(SerialReaderOutput::Message(parsed)).await.is_err() {
                            break 'outer; // packet receiver closed, exit thread
                        }
                    } else {
                        eprintln!("Failed to parse telemetry message: {:?}", packet_bytes);
                        in_msg_tx.send(SerialReaderOutput::InvalidData).await.ok();
                    }
                },
                DecodeResult::Invalid(reason) => {
                    // Recoverable: the decoder already slid past the bad magic byte
                    eprintln!("{reason}");
                    in_msg_tx.send(SerialReaderOutput::InvalidData).await.ok();
                },
                DecodeResult::NeedMore => break,
            }
        }

        // read() is cancel-safe, so a shutdown mid-read loses no buffered bytes
        select! {
            _ = shutdown_flag.recv() => break,
            result = serial_read.read(&mut read_buf) => {
                match result {
                    // port is O_NONBLOCK, so 0 bytes only means the device hung up
                    Ok(0) => panic!("Fatal serial read error: serial port closed"),
                    Ok(n) => decoder.push(&read_buf[..n]),
                    // Real OS-level I/O failure (e.g. ENXIO/errno 6 when the serial
                    // device is yanked); panic so main.rs restarts us.
                    Err(e) => panic!("Fatal serial read error: {e}"),
                }
            }
        }
    }
}


/**
 * Intakes incoming packet bytes and parses into telemetry incoming message
 */
fn parse_incoming_telem_message(payload: &Vec<u8>) -> Result<TelemetryIncomingMessage, ()> {
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

enum DecodeResult {
    Frame(Vec<u8>),
    Invalid(&'static str),
    NeedMore,
}

/**
 * Incremental frame decoder over a byte buffer.
 * On a bad header or CRC mismatch only the leading magic byte is dropped and the
 * rest of the buffer is rescanned, so a truncated or corrupted frame can't swallow
 * the frames behind it (same resync strategy as the DAM's app_telemRx).
 */
#[derive(Default)]
struct FrameDecoder {
    buf: Vec<u8>,
}

impl FrameDecoder {
    fn push(&mut self, bytes: &[u8]) {
        self.buf.extend_from_slice(bytes);
    }

    fn decode(&mut self) -> DecodeResult {
        const MAGIC: [u8; 2] = TelemetryIncomingMessage::MAGIC;
        const HEADER_SIZE: usize = TelemetryIncomingMessage::HEADER_SIZE;

        // hunt for magic, dropping everything before it
        match self.buf.windows(2).position(|w| w == MAGIC) {
            Some(start) => {
                self.buf.drain(..start);
            },
            None => {
                // keep a trailing first magic byte, its partner may be in the next read
                let keep = usize::from(self.buf.last() == Some(&MAGIC[0]));
                self.buf.drain(..self.buf.len() - keep);
                return DecodeResult::NeedMore;
            },
        }

        if self.buf.len() < HEADER_SIZE {
            return DecodeResult::NeedMore;
        }

        let payload_length = self.buf[2] as usize;
        if payload_length > TelemetryIncomingMessage::MAX_PAYLOAD_SIZE {
            self.buf.drain(..1);
            return DecodeResult::Invalid("Payload length exceeds maximum");
        }
        if payload_length == 0 {
            self.buf.drain(..1);
            return DecodeResult::Invalid("Payload length is 0");
        }

        let frame_size = HEADER_SIZE + payload_length;
        if self.buf.len() < frame_size {
            return DecodeResult::NeedMore;
        }

        let expected_crc = u32::from_le_bytes([self.buf[3], self.buf[4], self.buf[5], self.buf[6]]);
        let payload = &self.buf[HEADER_SIZE..frame_size];
        if CRC32_CALC.checksum(payload) != expected_crc {
            self.buf.drain(..1);
            return DecodeResult::Invalid("CRC mismatch");
        }

        let payload = payload.to_vec();
        self.buf.drain(..frame_size);
        DecodeResult::Frame(payload)
    }
}

/**
 * Subhandler that manages packet sending, handles outgoing telemetry messages from main task
 */
async fn packet_sender_handler(
    mut shutdown_flag: broadcast::Receiver<()>, 
    mut serial_write: WriteHalf<SerialStream>, 
    mut out_msg_rx: mpsc::Receiver<TelemetryOutgoingMessage>,
    mut client_out_msg_rx: broadcast::Receiver<TelemetryOutgoingMessage>
) {
    let mut write_msg_to_serial = async |msg: TelemetryOutgoingMessage| {
        let packet = generate_packet(msg);
        match serial_write.write_all(&packet).await {
            Ok(_) => {},
            Err(e) => {
                eprintln!("Failed to send packet: {e}");
            }
        }
    };
    loop {
        select! {
            // TODO select! is pseudo random, so in absolute saturated worst case, one may be starved over another
            _ = shutdown_flag.recv() => break,
            Ok(client_outgoing) = client_out_msg_rx.recv() => {
                write_msg_to_serial(client_outgoing).await;
            },
            Some(outgoing) = out_msg_rx.recv() => {
                write_msg_to_serial(outgoing).await;
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
        TelemetryOutgoingMessage::NtpTrigger => {
            payload.push(TelemetryOutgoingMessage::NTP_TRIGGER_BYTE);
        },
        TelemetryOutgoingMessage::NtpResponse{ t1 } => {
            payload.push(TelemetryOutgoingMessage::NTP_RESPONSE_BYTE);

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
    packet.extend_from_slice(&TelemetryOutgoingMessage::MAGIC);
    packet.extend_from_slice(&length);
    packet.extend_from_slice(&checksum);
    packet.extend_from_slice(&payload);
    
    return packet;
}

#[cfg(test)]
mod tests {
    use super::*;

    fn encode_frame(payload: &[u8]) -> Vec<u8> {
        let mut frame = TelemetryIncomingMessage::MAGIC.to_vec();
        frame.push(payload.len() as u8);
        frame.extend_from_slice(&CRC32_CALC.checksum(payload).to_le_bytes());
        frame.extend_from_slice(payload);
        frame
    }

    fn can_payload(id: u8) -> Vec<u8> {
        let mut payload = vec![TelemetryIncomingMessage::CAN_BYTE, id, 0, 0, 0];
        payload.extend_from_slice(&1_000u64.to_le_bytes());
        payload.extend_from_slice(&[0xDE, 0xAD, 0xBE, 0xEF]);
        payload
    }

    /// Drain the decoder, returning (frames, invalid_count)
    fn decode_all(decoder: &mut FrameDecoder) -> (Vec<Vec<u8>>, usize) {
        let mut frames = Vec::new();
        let mut invalid = 0;
        loop {
            match decoder.decode() {
                DecodeResult::Frame(f) => frames.push(f),
                DecodeResult::Invalid(_) => invalid += 1,
                DecodeResult::NeedMore => return (frames, invalid),
            }
        }
    }

    #[test]
    fn decodes_back_to_back_frames() {
        let mut decoder = FrameDecoder::default();
        decoder.push(&[encode_frame(&can_payload(1)), encode_frame(&can_payload(2))].concat());
        assert_eq!(decode_all(&mut decoder), (vec![can_payload(1), can_payload(2)], 0));
    }

    #[test]
    fn decodes_frame_split_byte_by_byte() {
        let mut decoder = FrameDecoder::default();
        let mut frames = Vec::new();
        for byte in encode_frame(&can_payload(1)) {
            decoder.push(&[byte]);
            frames.extend(decode_all(&mut decoder).0);
        }
        assert_eq!(frames, vec![can_payload(1)]);
    }

    #[test]
    fn skips_leading_garbage() {
        let mut decoder = FrameDecoder::default();
        decoder.push(&[0x00, 0xAA, 0x12, 0x55, 0xFF]);
        decoder.push(&encode_frame(&can_payload(1)));
        assert_eq!(decode_all(&mut decoder), (vec![can_payload(1)], 0));
    }

    #[test]
    fn truncated_frame_does_not_swallow_next_frame() {
        // radio drops the tail of frame 1; frame 2 must still decode
        let mut truncated = encode_frame(&can_payload(1));
        truncated.truncate(truncated.len() - 3);
        let mut decoder = FrameDecoder::default();
        decoder.push(&[truncated, encode_frame(&can_payload(2)), encode_frame(&can_payload(3))].concat());
        let (frames, invalid) = decode_all(&mut decoder);
        assert_eq!(frames, vec![can_payload(2), can_payload(3)]);
        assert!(invalid >= 1);
    }

    #[test]
    fn corrupted_crc_does_not_swallow_next_frame() {
        let mut corrupted = encode_frame(&can_payload(1));
        let last = corrupted.len() - 1;
        corrupted[last] ^= 0xFF;
        let mut decoder = FrameDecoder::default();
        decoder.push(&[corrupted, encode_frame(&can_payload(2))].concat());
        assert_eq!(decode_all(&mut decoder), (vec![can_payload(2)], 1));
    }

    #[test]
    fn false_magic_with_max_length_does_not_swallow_next_frames() {
        // false magic claiming a 100 byte payload in front of several real frames
        let false_header = [0xAA, 0x55, 100, 0, 0, 0, 0];
        let real = [encode_frame(&can_payload(1)), encode_frame(&can_payload(2)), encode_frame(&can_payload(3)), encode_frame(&can_payload(4))].concat();
        let mut decoder = FrameDecoder::default();
        decoder.push(&[&false_header[..], &real[..]].concat());
        // 103 bytes buffered < 107 claimed: frames are held (not lost) until the false header can be CRC-rejected
        assert_eq!(decode_all(&mut decoder), (vec![], 0));
        decoder.push(&encode_frame(&can_payload(5)));
        let (frames, invalid) = decode_all(&mut decoder);
        assert_eq!(frames, (1..=5).map(can_payload).collect::<Vec<_>>());
        assert_eq!(invalid, 1);
    }

    #[test]
    fn rejects_bad_length_and_resyncs() {
        let mut decoder = FrameDecoder::default();
        decoder.push(&[0xAA, 0x55, 0, 0, 0, 0, 0]);
        decoder.push(&[0xAA, 0x55, 200, 0, 0, 0, 0]);
        decoder.push(&encode_frame(&can_payload(1)));
        assert_eq!(decode_all(&mut decoder), (vec![can_payload(1)], 2));
    }

    #[test]
    fn buffer_stays_bounded_on_garbage() {
        let mut decoder = FrameDecoder::default();
        for _ in 0..100 {
            decoder.push(&[0x11; 1024]);
            decode_all(&mut decoder);
        }
        assert!(decoder.buf.len() <= 1);
    }
}
