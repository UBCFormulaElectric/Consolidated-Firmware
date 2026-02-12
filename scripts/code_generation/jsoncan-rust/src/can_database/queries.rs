use crate::can_database::{CanDatabase, CanMessage, CanSignal, CanSignalType, error::CanDBError};

impl CanDatabase {
    pub fn get_message_by_node(
        self: &Self,
        node_name: &str,
    ) -> Result<Vec<CanMessage>, CanDBError> {
        // Returns list of messages transmitted by node_name
        let mut s = self
            .conn
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
        let mut s = self
            .conn
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

    pub fn get_all_tx_msgs_for(self: &Self, node_name: &str) -> Vec<String> {
        let mut s = self
            .conn
            .prepare("SELECT name FROM messages WHERE tx_node_name != ?1")
            .unwrap();

        s.query_map([node_name], |row| Ok(row.get::<_, String>(0)?))
            .unwrap()
            .map(|res| res.unwrap())
            .collect()
    }

    pub fn get_message_by_name(self: &Self, message_name: &str) -> Result<CanMessage, CanDBError> {
        let mut s = self
            .conn
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
                modes: serde_json::from_str::<Vec<String>>(&row.get::<_, String>(7)?).unwrap(),
                signals: self.get_signals_for_message(row.get(1)?).unwrap(),
            })
        }) {
            Ok(msg) => Ok(msg),
            Err(e) => Err(CanDBError::SqlLiteError(e)),
        }
    }

    pub fn get_message_name_by_id(self: &Self, message_id: u32) -> Result<String, CanDBError> {
        let mut s = self
            .conn
            .prepare("SELECT name FROM messages WHERE id = ?1")
            .unwrap();

        match s.query_row([message_id], |row| row.get(0)) {
            Ok(msg_name) => Ok(msg_name),
            Err(e) => Err(CanDBError::SqlLiteError(e)),
        }
    }

    pub fn get_message_by_id(self: &Self, message_id: u32) -> Result<CanMessage, CanDBError> {
        let mut s = self
            .conn
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
        let mut s = self.conn.prepare("SELECT * FROM messages").unwrap();

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
}
