use serialport::SerialPort;
use std::io::{Error, ErrorKind};
use std::time::Duration;

use super::thread_signal_handler::should_run;
use super::can_data_handler::CanMessage;
use super::clock_handler::TimeMessage;

/**
 * Handling serial signals from radio.
 */
pub fn run_serial_task() {
    // TODO use tokio to not block a core when reading from serial port
    let serial_port_builder = serialport::new(
            "/dev/ttyUSB0",
            9600
        )
        .timeout(Duration::from_millis(10));

    let mut serial_port = match serial_port_builder.open() {
        Ok(port) => port,
        Err(e) => {
            eprintln!("Failed to open serial port: {}", e);
            return;
        }
    };

    // have a should run check
    loop {
        if !should_run() {
            break;
        }

        // TODO i love unwrapping
        let packet = match read_packet(&mut serial_port) {
            Ok(p)   => p,
            Err(_)  => continue,
        };

        let message = match parse_telem_message(packet) {
            Ok(m)   => m,
            Err(_)  => continue,
        };

        match message {
            CanMessage { 
                can_id, 
                can_time_offset, 
                can_payload 
            } => {
                // TODO handle CAN message
                println!("Received CAN Message: ID: {}, Time Offset: {}, Payload: {:?}", can_id, can_time_offset, can_payload);
            },
            TimeMessage::NTPTimeMessage => {
                // TODO handle NTP time message
            },
            TimeMessage::NTPDateMessage => {
                // TODO handle NTP date message
            },
            TimeMessage::BaseTimeRegMessage { base_time } => {
                // TODO handle base time register message
            },
        }
    }
}

const MAGIC: [u8; 2] = [0xaa, 0x55];
const HEADER_SIZE: usize = 7;
const MAX_PAYLOAD_SIZE: usize = 100;

enum TelemetryMessageType {
    CanMessage,
    NTPTimeMessage,
    NTPDateMessage,
    BaseTimeRegMessage,
}

impl TryFrom<u8> for TelemetryMessageType {
    type Error = ();
    fn try_from(byte: u8) -> Result<Self, Self::Error> {
        match byte {
            0x01 => Ok(TelemetryMessageType::CanMessage),
            0x02 => Ok(TelemetryMessageType::NTPTimeMessage),
            0x03 => Ok(TelemetryMessageType::NTPDateMessage),
            0x04 => Ok(TelemetryMessageType::BaseTimeRegMessage),
            _ => Err(()),
        }
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

    // checksum
    // TODO implement checksum verification

    return Ok(payload_buffer);
}

fn parse_telem_message(payload: Vec<u8>) -> Result<someT, ()> {
    let message_type = TelemetryMessageType::try_from(payload[0])?;
    let parsed_message: someT = match message_type {
        TelemetryMessageType::CanMessage => {
            let can_id = u32::from_le_bytes([payload[1], payload[2], payload[3], payload[4]]);
            let can_time_offset = f32::from_le_bytes([payload[5], payload[6], payload[7], payload[8]]);
            let can_payload = payload[9..].to_vec();
            CanMessage {
                can_id,
                can_time_offset,
                can_payload,
            }
        },
        TelemetryMessageType::NTPTimeMessage => TimeMessage::NTPTimeMessage,
        TelemetryMessageType::NTPDateMessage => TimeMessage::NTPDateMessage,
        TelemetryMessageType::BaseTimeRegMessage => {
            let base_time = u32::from_le_bytes([payload[1], payload[2], payload[3], payload[4]]);
            TimeMessage::BaseTimeRegMessage {
                base_time,
            }
        }
    };

    return Ok(parsed_message);
}
