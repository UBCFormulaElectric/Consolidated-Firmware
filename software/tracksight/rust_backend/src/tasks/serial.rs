use serialport::SerialPort;
use std::io::{Error, ErrorKind};
use std::time::Duration;

/**
 * Handling serial signals from radio.
 */
pub fn run_serial_tasks() {
    let mut serialport = serialport::new("/dev/ttyUSB0", 9600)
        .timeout(Duration::from_millis(10))
        .open()
        .expect("Failed to open serial port");

    loop {
        // TODO i love unwrapping
        let packet = match read_packet(&mut serialport) {
            Ok(p)   => p,
            Err(_)  => continue,
        };

        let message = match parse_telem_message(packet) {
            Ok(m)   => m,
            Err(_)  => continue,
        };
    }
}

const MAGIC: [u8; 2] = [0xaa, 0x55];
const HEADER_SIZE: usize = 7;
const MAX_PAYLOAD_SIZE: usize = 100;

enum TelemetryMessage {
    CanMessage {
        can_id: u32,
        can_time_offset: f32,
        can_payload: Vec<u8>,
    },
    NTPTimeMessage,
    NTPDateMessage,
    BaseTimeRegMessage {
        base_time: u32, // TODO some time format
    }
}

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
            // TODO error handling
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

fn parse_telem_message(payload: Vec<u8>) -> Result<TelemetryMessage, ()> {
    let message_type = TelemetryMessageType::try_from(payload[0])?;
    let parsed_message: TelemetryMessage = match message_type {
        TelemetryMessageType::CanMessage => {
            let can_id = u32::from_le_bytes([payload[1], payload[2], payload[3], payload[4]]);
            let can_time_offset = f32::from_le_bytes([payload[5], payload[6], payload[7], payload[8]]);
            let can_payload = payload[9..].to_vec();
            TelemetryMessage::CanMessage {
                can_id,
                can_time_offset,
                can_payload,
            }
        },
        TelemetryMessageType::NTPTimeMessage => TelemetryMessage::NTPTimeMessage,
        TelemetryMessageType::NTPDateMessage => TelemetryMessage::NTPDateMessage,
        TelemetryMessageType::BaseTimeRegMessage => {
            let base_time = u32::from_le_bytes([payload[1], payload[2], payload[3], payload[4]]);
            TelemetryMessage::BaseTimeRegMessage {
                base_time,
            }
        }
    };

    return Ok(parsed_message);
}
