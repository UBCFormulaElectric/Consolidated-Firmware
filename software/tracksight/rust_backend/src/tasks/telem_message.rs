use std::time::SystemTime;

pub enum TelemetryMessage {
    Can {
        body: CanPayload,
    },
    NTPTime {
        body: NTPTimeMessage,
    },
    NTPDate {
        body: NTPDateMessage,
    },
    BaseTimeReg {
        body: BaseTimeRegMessage,
    },
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


pub struct NTPTimeMessage;

pub struct NTPDateMessage;

pub struct BaseTimeRegMessage {
    pub base_time: u32, // TODO some time format
}

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