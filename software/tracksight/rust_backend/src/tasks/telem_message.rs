use std::time::{Duration, SystemTime};

/**
 * Incoming messages from DAM to backend
 */
pub enum TelemetryIncomingMessage {
    Can {
        body: CanPayload,
    },
    NTP
}

impl TelemetryIncomingMessage {
    pub const CAN_BYTE: u8 = 0x01;
    pub const NTP_BYTE: u8 = 0x02;
}

// impl TryFrom<u8> for TelemetryMessage {
//     type Error = ();
//     fn try_from(byte: u8) -> Result<Self, Self::Error> {
//         match byte {
//             0x01 => Ok(TelemetryMessage::CanMessage),
//             0x02 => Ok(TelemetryMessage::NTPTimeMessage),
//             0x03 => Ok(TelemetryMessage::NTPDateMessage),
//             0x04 => Ok(TelemetryMessage::BaseTimeRegMessage),
//             _ => Err(()),
//         }
//     }
// }

/**
 * Message passed through serial
 * Define with default cloning behaviour, as fields are more or less primitives
 */
#[derive(Clone, Debug)]
 pub struct CanPayload {
    pub can_id: u32,
    pub can_timestamp: SystemTime, // should deprecate this field when we establish RTC and NTP
    pub payload: Vec<u8>,
}

pub enum TelemetryOutgoingMessage {
    // payload should be t1 and t2, each being 64 bits
    NTP {
        // Only pack t1 in outgoing message
        // let the handler actually send the appropriate t2 when sending packet
        t1: Duration
    }
}

impl TelemetryOutgoingMessage {
    pub const NTP_BYTE: u8 = 0x01;
}