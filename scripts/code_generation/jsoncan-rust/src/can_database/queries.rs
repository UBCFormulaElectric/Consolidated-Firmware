use std::collections::HashMap;

use crate::can_database::{
    CanBusModes, CanDatabase, CanMessage, CanSignal, CanSignalType, DecodedSignal,
    error::CanDBError,
};

impl CanDatabase {
    pub fn get_message_by_node(
        self: &Self,
        node_name: &str,
    ) -> Result<Vec<CanMessage>, CanDBError> {
        // Returns list of messages transmitted by node_name
        let binding = self.get_connection()?;
        let mut s = binding
            .prepare("SELECT * FROM messages WHERE tx_node_name = ?1")
            .unwrap();

        match s.query_map([node_name], |row| {
            Ok(CanMessage {
                name: row.get(0)?,
                id: row.get(1)?,
                description: row.get(2)?,
                cycle_time: row.get(3)?,
                log_cycle_time: row.get(4).unwrap_or_default(), // INCREDIBLY SUS
                telem_cycle_time: row.get(5).unwrap_or_default(), // INCREDIBLY SUSSY
                tx_node_name: row.get(6)?,
                modes: match row.get::<_, String>(7)?.as_str() {
                    "" => CanBusModes::All,
                    mode_str => CanBusModes::Some(serde_json::from_str(mode_str).unwrap()),
                },
                signals: self.get_signals_for_message(row.get(1)?).unwrap(),
            })
        }) {
            Ok(msg) => {
                let mut messages = Vec::new();
                for message_result in msg {
                    match message_result {
                        Err(e) => return Err(CanDBError::SqlLiteError(e)),
                        Ok(message) => messages.push(message),
                    }
                }
                Ok(messages)
            }
            Err(e) => Err(CanDBError::SqlLiteError(e)),
        }
    }

    pub fn get_signals_for_message(
        self: &Self,
        message_id: u32,
    ) -> Result<Vec<CanSignal>, CanDBError> {
        // Returns list of signals for a given message ID
        let binding = self.get_connection()?;
        let mut s = binding
            .prepare("SELECT * FROM signals WHERE message_id = ?1")
            .unwrap();

        match s.query_map([message_id], |row| {
            Ok(CanSignal {
                name: row.get(0)?,
                start_bit: row.get(2)?,
                bits: row.get(3)?,
                scale: row.get(4)?,
                offset: row.get(5)?,
                min: row.get(6)?,
                max: row.get(7)?,
                start_val: row.get(8)?,
                enum_name: {
                    let en = row.get::<_, String>(9)?;
                    if en.is_empty() { None } else { Some(en) }
                },
                unit: {
                    let u = row.get::<_, String>(10)?;
                    if u.is_empty() { None } else { Some(u) }
                },
                signed: row.get::<_, i32>(11)? != 0,
                description: {
                    let d = row.get::<_, String>(12)?;
                    if d.is_empty() { None } else { Some(d) }
                },
                big_endian: row.get::<_, i32>(13)? != 0,
                signal_type: CanSignalType::from(row.get::<_, i32>(14)? as u32),
            })
        }) {
            Err(e) => Err(CanDBError::SqlLiteError(e)),
            Ok(k) => {
                let mut signals = Vec::new();
                for signal_result in k {
                    match signal_result {
                        Err(e) => return Err(CanDBError::SqlLiteError(e)),
                        Ok(signal) => signals.push(signal),
                    }
                }
                Ok(signals)
            }
        }
    }

    pub fn get_all_rx_msgs_for(self: &Self, node_name: &str) -> Result<Vec<u32>, CanDBError> {
        let binding = self.get_connection()?;
        let mut s = binding
            .prepare("SELECT id FROM messages WHERE tx_node_name != ?1")
            .unwrap();

        let res = s
            .query_map([node_name], |row| Ok(row.get::<_, u32>(0)?))
            .unwrap()
            .map(|res| res.unwrap())
            .collect();

        Ok(res)
    }

    pub fn get_message_by_name(self: &Self, message_name: &str) -> Result<CanMessage, CanDBError> {
        let binding = self.get_connection()?;
        let mut s = binding
            .prepare("SELECT name, id, description, cycle_time, log_cycle_time, telem_cycle_time, tx_node_name, modes FROM messages WHERE name = ?1")
            .unwrap();

        match s.query_row([message_name], |row| {
            Ok(CanMessage {
                name: row.get(0)?,
                id: row.get(1)?,
                description: row.get(2)?,
                cycle_time: row.get(3)?,
                log_cycle_time: row.get(4).unwrap_or_default(),
                telem_cycle_time: row.get(5).unwrap_or_default(),
                tx_node_name: row.get(6)?,
                modes: match row.get::<_, String>(7)?.as_str() {
                    "" => CanBusModes::All,
                    mode_str => CanBusModes::Some(serde_json::from_str(mode_str).unwrap()),
                },
                signals: self.get_signals_for_message(row.get(1)?).unwrap(),
            })
        }) {
            Ok(msg) => Ok(msg),
            Err(e) => Err(CanDBError::SqlLiteError(e)),
        }
    }

    pub fn get_message_id_by_name(self: &Self, message_name: &str) -> Result<u32, CanDBError> {
        let binding = self.get_connection()?;

        let mut s = binding
            .prepare("SELECT id FROM messages WHERE name = ?1")
            .unwrap();

        match s.query_row([message_name], |row| row.get(0)) {
            Ok(msg_id) => Ok(msg_id),
            Err(e) => {
                match e {
                    rusqlite::Error::QueryReturnedNoRows => {
                        Err(CanDBError::MessageNotFound {msg_name: message_name.into()})
                    },
                    _ => Err(CanDBError::SqlLiteError(e))
                }
            },
        }
    }

    pub fn get_message_name_by_id(self: &Self, message_id: u32) -> Result<String, CanDBError> {
        let binding = self.get_connection()?;
        let mut s = binding
            .prepare("SELECT name FROM messages WHERE id = ?1")
            .unwrap();

        match s.query_row([message_id], |row| row.get(0)) {
            Ok(msg_name) => Ok(msg_name),
            Err(e) => Err(CanDBError::SqlLiteError(e)),
        }
    }

    pub fn get_message_by_id(self: &Self, message_id: u32) -> Result<CanMessage, CanDBError> {
        let binding = self.get_connection()?;
        let mut s = binding
            .prepare("SELECT * FROM messages WHERE id = ?1")
            .unwrap();

        match s.query_row([message_id], |row| {
            Ok(CanMessage {
                name: row.get(0)?,
                id: row.get(1)?,
                description: row.get(2)?,
                cycle_time: row.get(3)?,
                log_cycle_time: row.get(4).unwrap_or_default(),
                telem_cycle_time: row.get(5).unwrap_or_default(),
                tx_node_name: row.get(6)?,
                modes: match row.get::<_, String>(7)?.as_str() {
                    "" => CanBusModes::All,
                    mode_str => CanBusModes::Some(serde_json::from_str(mode_str).unwrap()),
                },
                signals: self.get_signals_for_message(row.get(1)?).unwrap(),
            })
        }) {
            Ok(msg) => Ok(msg),
            Err(e) => Err(CanDBError::SqlLiteError(e)),
        }
    }

    pub fn get_all_msgs(self: &Self) -> Result<Vec<CanMessage>, CanDBError> {
        let binding = self.get_connection()?;
        let mut s = binding.prepare("SELECT * FROM messages").unwrap();

        match s.query_map([], |row| {
            let mut signals = self.get_signals_for_message(row.get(1)?).unwrap();
            signals.sort_by(|a, b| a.start_bit.cmp(&b.start_bit));
            Ok(CanMessage {
                name: row.get(0)?,
                id: row.get(1)?,
                description: row.get(2)?,
                cycle_time: row.get(3)?,
                log_cycle_time: row.get(4).unwrap_or_default(),
                telem_cycle_time: row.get(5).unwrap_or_default(),
                tx_node_name: row.get(6)?,
                modes: match row.get::<_, String>(7)?.as_str() {
                    "" => CanBusModes::All,
                    mode_str => CanBusModes::Some(serde_json::from_str(mode_str).unwrap()),
                },
                signals,
            })
        }) {
            Err(e) => Err(CanDBError::SqlLiteError(e)),
            Ok(k) => Ok(k.map(|res| res.unwrap()).collect()),
        }
    }

    pub fn pack(
        self: &Self,
        msg_name: &str,
        signals: &[DecodedSignal],
    ) -> Result<(u32, Vec<u8>), CanDBError> {
        let msg = match self.get_message_by_name(msg_name) {
            Ok(m) => m,
            Err(_) => {
                eprintln!(
                    "Message named '{}' is not defined in the database.",
                    msg_name
                );
                return Err(CanDBError::SqlLiteError(rusqlite::Error::InvalidQuery));
            }
        };

        let mut signal_map = HashMap::new();
        for signal in &msg.signals {
            signal_map.insert(signal.name.clone(), signal);
        }

        let mut max_bit = 0;
        for signal in &msg.signals {
            let end_bit = signal.start_bit + signal.bits;
            if end_bit > max_bit {
                max_bit = end_bit;
            }
        }
        let useful_length = ((max_bit + 7) / 8) as usize;
        let allowable_lengths = [0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64];
        let mut dlc = 0;
        for &length in &allowable_lengths {
            if length >= useful_length {
                dlc = length;
                break;
            }
        }
        if dlc == 0 {
            return Err(CanDBError::SqlLiteError(rusqlite::Error::InvalidQuery));
        }

        let mut data_bytes = vec![0u8; dlc];

        for decoded_signal in signals {
            let signal = match signal_map.get(&decoded_signal.name) {
                Some(s) => s,
                None => {
                    eprintln!(
                        "Signal named '{}' is not defined for message '{}'.",
                        decoded_signal.name, msg.name
                    );
                    continue;
                }
            };

            let raw_value = ((decoded_signal.value - signal.offset) / signal.scale).floor() as i64;
            let nbits = signal.bits as usize;
            let bitmask: u64 = if nbits >= 64 { u64::MAX } else { (1u64 << nbits) - 1 };
            let val = (raw_value as u64) & bitmask;

            for i in 0..nbits {
                let bit = ((val >> i) & 1) as u8;
                let bit_idx = signal.start_bit as usize + i;
                let byte_idx = bit_idx / 8;
                let bit_in_byte = bit_idx % 8;
                if byte_idx < data_bytes.len() {
                    data_bytes[byte_idx] |= bit << bit_in_byte;
                }
            }
        }

        Ok((msg.id, data_bytes))
    }

    pub fn unpack(&self, msg_id: u32, data: Vec<u8>, timestamp: Option<u64>) -> Vec<DecodedSignal> {
        let msgs = match self.get_message_by_id(msg_id) {
            Ok(m) => m,
            Err(_) => {
                eprintln!("Message ID {} not found in database", msg_id);
                return Vec::new();
            }
        };

        let mut decoded_signals: Vec<DecodedSignal> = Vec::new();

        for signal in msgs.signals {
            // Extract the bits representing the current signal from the raw byte payload.
            // Supports payloads up to 64 bytes (CAN-FD) and signal widths up to 64 bits.
            let nbits = signal.bits as usize;
            let start = signal.start_bit as usize;
            let mut val: u64 = 0;
            for i in 0..nbits.min(64) {
                let bit_idx = start + i;
                let byte_idx = bit_idx / 8;
                let bit_in_byte = bit_idx % 8;
                if byte_idx < data.len() {
                    let bit = ((data[byte_idx] >> bit_in_byte) & 1) as u64;
                    val |= bit << i;
                }
            }

            let raw_value = {
                // Handle signed values via 2's complement
                if signal.signed && nbits > 0 && nbits < 64 && (val & (1u64 << (nbits - 1)) != 0) {
                    val.wrapping_sub(1u64 << nbits)
                } else {
                    val
                }
            };

            // Apply scaling and offset
            let scaled_value = raw_value as f64 * signal.scale + signal.offset;

            // Initialize decoded signal

            let decoded = DecodedSignal {
                name: signal.name,
                value: scaled_value,
                timestamp,
                unit: signal.unit,
                label: signal.enum_name.as_deref().and_then(|enum_name| {
                    self.get_enum(enum_name).and_then(|can_enum| {
                        can_enum
                            .values
                            .iter()
                            .find(|(_, val)| **val as f64 == scaled_value)
                            .map(|(k, _)| k.clone())
                    })
                }),
                signal_type: signal.signal_type,
            };

            if signal.enum_name.is_some() {
                if decoded.label.is_none() {
                    continue;
                }
            }

            decoded_signals.push(decoded);
        }

        return decoded_signals;
    }

    pub fn is_signal_valid(self: &Self, signal_name: &str) -> bool {
        let binding = match self.get_connection() {
            Ok(b) => b,
            Err(_) => return false,
        };

        let mut s = binding
            .prepare("SELECT COUNT(*) FROM signals WHERE name = ?1")
            .unwrap();

        return match s.query_row([signal_name], |row| row.get::<_, i32>(0)) {
            Ok(count) => count > 0,
            Err(_) => false,
        };
    }
}
