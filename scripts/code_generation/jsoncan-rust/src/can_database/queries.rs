use crate::can_database::{CanDatabase, CanMessage, CanSignal, CanSignalType, DecodedSignal, error::CanDBError};

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
                modes: serde_json::from_str::<Vec<String>>(&row.get::<_, String>(7)?).unwrap(),
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
                signal_type: match row.get::<_, i32>(14)? {
                    0 => CanSignalType::Numerical,
                    1 => CanSignalType::Alert,
                    2 => CanSignalType::Enum,
                    _ => panic!("Invalid signal type in database"),
                },
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

    pub fn get_all_rx_msgs_for(self: &Self, node_name: &str) -> Result<Vec<String>, CanDBError> {
        let binding = self.get_connection()?;
        let mut s = binding
            .prepare("SELECT name FROM messages WHERE tx_node_name != ?1")
            .unwrap();

        let res = s.query_map([node_name], |row| Ok(row.get::<_, String>(0)?))
            .unwrap()
            .map(|res| res.unwrap())
            .collect();

        Ok(res)
    }

    pub fn get_message_by_name(self: &Self, message_name: &str) -> Result<CanMessage, CanDBError> {
        let binding = self.get_connection()?;
        let mut s = binding
            .prepare("SELECT * FROM messages WHERE name = ?1")
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
                modes: serde_json::from_str::<Vec<String>>(&row.get::<_, String>(7)?).unwrap(),
                signals: self.get_signals_for_message(row.get(1)?).unwrap(),
            })
        }) {
            Ok(msg) => Ok(msg),
            Err(e) => Err(CanDBError::SqlLiteError(e)),
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
                modes: serde_json::from_str::<Vec<String>>(&row.get::<_, String>(7)?).unwrap(),
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
            Ok(CanMessage {
                name: row.get(0)?,
                id: row.get(1)?,
                description: row.get(2)?,
                cycle_time: row.get(3)?,
                log_cycle_time: row.get(4).unwrap_or_default(),
                telem_cycle_time: row.get(5).unwrap_or_default(),
                tx_node_name: row.get(6)?,
                modes: serde_json::from_str::<Vec<String>>(&row.get::<_, String>(7)?).unwrap(),
                signals: self.get_signals_for_message(row.get(1)?).unwrap(),
            })
        }) {
            Err(e) => Err(CanDBError::SqlLiteError(e)),
            Ok(k) => Ok(k.map(|res| res.unwrap()).collect()),
        }
    }

    pub fn pack(&self) -> Vec<u8> {
        todo!("Serializing or whatnot")
    }

    pub fn unpack(&self, msg_id: u32, data: Vec<u8>, timestamp: Option<std::time::SystemTime>) -> Vec<DecodedSignal> {
        let msgs = match self.get_message_by_id(msg_id) {
            Ok(m) => m,
            Err(_) => {
                eprintln!("Message ID {} not found in database", msg_id);
                return Vec::new()
            },
        };

        let mut decoded_signals: Vec<DecodedSignal> = Vec::new();
        
        let mut buf = [0u8; 4];
        let len = data.len().min(4);
        buf[..len].copy_from_slice(&data[..len]);

        let data_uint = u32::from_le_bytes(buf);

        for signal in msgs.signals {
            // Extract the bits representing the current signal.
            let data_shifted = data_uint as u64 >> signal.start_bit;

            let bitmask = (1u64 << signal.bits) - 1;
            let raw_value = {
                let val = data_shifted & bitmask;
                // Handle signed values via 2's complement
                if signal.signed && (val & (1 << (signal.bits - 1)) != 0) {
                    val - (1 << signal.bits)
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
                timestamp: timestamp,
                unit: signal.unit,
                label: signal.enum_name.as_deref().and_then(|enum_name| {
                    self.get_enum(enum_name).and_then(|can_enum| {
                        can_enum.values.iter()
                            .find(|(_, val)| **val as f64 == scaled_value)
                            .map(|(k, _)| k.clone())
                    })
                }),
            };
            
            if decoded.label.is_none() {
                continue;
            }

            decoded_signals.push(decoded);
        }

        return decoded_signals;
    }
}
