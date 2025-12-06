pub enum TelemetryMessage {
    Can {
        body: CanMessage,
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
 */
pub struct CanMessage {
    pub can_id: u32,
    pub can_time_offset: f32, // should deprecate this field when we establish RTC and NTP
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

/**
 * Parsed from CanMessage payload, represents a single signal
 */
pub struct CanSignal {
    pub name: String,
    pub value: f64,
    pub timestamp: u64, // todo time format
}