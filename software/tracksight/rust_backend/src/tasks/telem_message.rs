use std::time::Duration;

use crc::{Crc, CRC_32_MPEG_2};

// calc is short for calculator for those new to the chat
pub const CRC32_CALC: Crc<u32> = Crc::<u32>::new(&CRC_32_MPEG_2);

/*
 * Telem packet bytes in the form of
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
 * This is the same for incoming and outgoing messages
 */

/**
 * Incoming messages from DAM to backend
 */
pub enum TelemetryIncomingMessage {
    Can { body: CanPayload },
    NTP,
}

impl TelemetryIncomingMessage {
    pub const MAGIC: [u8; 2] = [0xaa, 0x55];
    // 2 for magic bytes, 1 for length, 4 for checksum
    pub const HEADER_SIZE: usize = 7;
    pub const MAX_PAYLOAD_SIZE: usize = 100;

    pub const CAN_BYTE: u8 = 0x01;
    pub const NTP_BYTE: u8 = 0x02;
}

/**
 * Message passed through serial
 * Define with default cloning behaviour, as fields are more or less primitives
 */
#[derive(Clone, Debug)]
pub struct CanPayload {
    pub can_id: u32,
    pub can_timestamp: u64,
    pub payload: Vec<u8>,
}

pub enum TelemetryOutgoingMessage {
    // payload should be t1 and t2, each being 64 bits
    NTP {
        // Only pack t1 in outgoing message
        // let the handler actually send the appropriate t2 when sending packet
        t1: Duration,
    },
}

impl TelemetryOutgoingMessage {
    pub const MAGIC: [u8; 2] = [0xcc, 0x33];

    pub const NTP_BYTE: u8 = 0x01;
}
