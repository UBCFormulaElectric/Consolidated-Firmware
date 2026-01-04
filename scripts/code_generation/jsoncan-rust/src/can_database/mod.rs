pub mod error;
mod from_jsonparser;
mod types;

use error::CanDBError;
use rusqlite::Connection;
pub use types::*;

impl CanNode {
    pub fn add_rx_msg(&mut self, rx_msg: &CanMessage) -> Result<(), CanDBError> {
        // This function registers a message which is to be received by a node on the bus

        // Note that this function does not know
        // 1. which bus the sender will tx will send the message on
        // 2. which bus the receiver will rx the message on

        // This function
        // 1. enters the current node into the list of nodes that the message is rx'd by
        // 2. adds the message name to the list of messages received by the given node
        //         # if we are already listening to all, we don't need to register this specific message
        //         # in particular, this is useful for alerts which will blindly make everyone accept them
        //         if type(self._nodes[rx_node_name].rx_msgs_names) == AllRxMsgs:
        //             return

        if rx_msg.tx_node_name == *self.name {
            return Err(CanDBError::RxLoopback {
                rx_node_name: self.name.clone(),
                rx_msg_name: rx_msg.name.clone(),
            });
        }

        match &mut self.rx_msgs_names {
            RxMsgNames::All => {
                // this cannot happen because we checked for it earlier
                panic!(
                    "Node '{}' is already set to receive all messages, cannot add specific message '{}'",
                    self.name, rx_msg.name
                );
            }
            RxMsgNames::RxMsgs(msg_names) => {
                if msg_names.contains(&rx_msg.name) {
                    return Err(CanDBError::RxDuplicate {
                        rx_node_name: self.name.clone(),
                        rx_msg_name: rx_msg.name.clone(),
                    });
                }
                msg_names.push(rx_msg.name.clone());
            }
        }

        Ok(())
    }
}

pub struct CanDatabase {
    conn: rusqlite::Connection,
    pub nodes: Vec<CanNode>,
    pub buses: Vec<CanBus>,
    pub forwarding: Vec<BusForwarder>,
    pub shared_enums: Vec<CanEnum>,
}

impl CanDatabase {
    pub fn new(
        buses: Vec<CanBus>,
        nodes: Vec<CanNode>,
        forwarding: Vec<BusForwarder>,
        shared_enums: Vec<CanEnum>,
    ) -> Result<Self, CanDBError> {
        let conn = Connection::open_in_memory().unwrap();
        match conn.execute(
            "CREATE TABLE IF NOT EXISTS messages (
				name TEXT NOT NULL,
				id INTEGER PRIMARY KEY NOT NULL,
				description TEXT,
				cycle_time INTEGER,
				log_cycle_time INTEGER,
				telem_cycle_time INTEGER,
				tx_node_name TEXT NOT NULL,
				modes TEXT NOT NULL
			)",
            [],
        ) {
            Ok(_) => {}
            Err(e) => {
                return Err(CanDBError::SqlLiteError(e));
            }
        }

        // create table for signals
        match conn.execute(
            "CREATE TABLE IF NOT EXISTS signals (
				name TEXT NOT NULL,
				message_id INTEGER NOT NULL,
				start_bit INTEGER NOT NULL,
				bits INTEGER NOT NULL,
				scale REAL NOT NULL,
				offset REAL NOT NULL,
				min REAL NOT NULL,
				max REAL NOT NULL,
				start_val REAL NOT NULL,
				enum_name TEXT,
				unit TEXT,
				signed INTEGER NOT NULL,
				description TEXT,
				big_endian INTEGER NOT NULL,
				signal_type INTEGER NOT NULL,
				FOREIGN KEY(message_id) REFERENCES messages(id)
			)",
            [],
        ) {
            Ok(_) => {}
            Err(e) => {
                return Err(CanDBError::SqlLiteError(e));
            }
        }

        Ok(CanDatabase {
            conn,
            buses,
            nodes,
            forwarding,
            shared_enums,
        })
    }

    // TODO perhaps add a version which takes a list of msgs idk tho cuz this is not well parallelized
    pub fn add_tx_msg(self: &mut Self, msg: CanMessage) -> Result<(), CanDBError> {
        // This function registers a new message, transmitted by a node on the bus
        //
        // It
        // 1. adds the msg to the global dump of messages (self._msgs)
        // 2. adds the msg name to the list of messages broadcasted by the given node (self._nodes[node_name].tx_msg_names)
        // 3. adds all the signals into the global dump of signals (self._signals_to_msgs)
        // Note this function expects a valid CanMessage object

        // Check if this message name is a duplicate
        // TODO move this
        // if let Some(dup_msg) = msgs.iter().find(|m| m.name == msg.name) {
        //     return Err(ParseError::DuplicateTxMsgName {
        //         tx_node_name_1: tx_node_name.clone(),
        //         tx_node_name_2: dup_msg.tx_node_name.clone(),
        //         tx_msg_name: msg.name.clone(),
        //     });
        // }

        // TODO move this
        // if let Some(dup_msg) = msgs.iter().find(|m| m.id == msg.id) {
        //     return Err(ParseError::DuplicateTxMsgID {
        //         tx_msg_name: msg.name.clone(),
        //         tx_node_name_1: tx_node_name.clone(),
        //         tx_node_name_2: dup_msg.tx_node_name.clone(),
        //     });
        // }

        // TODO determine if a message is FD compatible when it is routed
        // if msg.requires_fd() && !tx_node.fd {
        //     return Err(ParseError::TxFDUnsupported {
        //         fd_msg_name: msg.name.clone(),
        //         non_fd_node_name: tx_node_name.clone(),
        //     });
        // }

        // register the message with the database of all messages

        // TODO figure out how to find conflicting signal names across messages
        // for signal in msgs.get(&msg.name).unwrap().signals.iter() {
        // register the signal with the database of all signals
        // if let Some(dup_msg) = signal_name_to_msgs.get(&signal.name) {
        //     return Err(ParseError::DuplicateTxSignalName {
        //         signal_name: signal.name.clone(),
        //         msg_name: msg.name.clone(),
        //         msg2_name: dup_msg.name.clone(),
        //     });
        // }
        // TODO learn lifetimes lmao
        // signal_name_to_msgs.insert(signal.name.clone(), msgs.get(&msg.name).unwrap());
        // }

        match self.conn.execute(
            "INSERT INTO messages (name, id, description, cycle_time, log_cycle_time, telem_cycle_time, tx_node_name, modes) VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8)",
            [
                &msg.name,
                &msg.id.to_string(),
                &msg.description.unwrap_or("".to_string()),
                &msg.cycle_time.map_or("".to_string(), |v| v.to_string()),
                &msg.log_cycle_time.map_or("".to_string(), |v| v.to_string()),
                &msg.telem_cycle_time.map_or("".to_string(), |v| v.to_string()),
                &msg.tx_node_name,
                &serde_json::to_string(&msg.modes).unwrap()
            ],
        ) {
            Ok(_) => {}
            Err(e) => {
                return Err(CanDBError::SqlLiteError(e));
            }
        }

        let msg_id = msg.id;
        for signal in msg.signals.iter() {
            self.add_signal(&msg_id, signal)?;
        }

        Ok(())
    }

    pub fn add_signal(self: &mut Self, msg_id: &u32, signal: &CanSignal) -> Result<(), CanDBError> {
        match self.conn.execute(
            "INSERT INTO signals (name, message_id, start_bit, bits, scale, offset, min, max, start_val, enum_name, unit, signed, description, big_endian, signal_type) VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14, ?15)",
            [
                &signal.name,
                &msg_id.to_string(),
                &signal.start_bit.to_string(),
                &signal.bits.to_string(),
                &signal.scale.to_string(),
                &signal.offset.to_string(),
                &signal.min.to_string(),
                &signal.max.to_string(),
                &signal.start_val.to_string(),
                &signal.enum_name.clone().unwrap_or("".to_string()),
                &signal.unit.clone().unwrap_or("".to_string()),
                &(if signal.signed { "1".to_string() } else { "0".to_string() }),
                &signal.description.clone().unwrap_or("".to_string()),
                &(if signal.big_endian { "1".to_string() } else { "0".to_string() }),
                &(signal.signal_type.clone() as u32).to_string(),
            ],
        )
    {
            Ok(_) => Ok(()),
            Err(e) => Err(CanDBError::SqlLiteError(e)),
        }
    }

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
                log_cycle_time: row.get(4)?,
                telem_cycle_time: row.get(5)?,
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

    pub fn get_tx_node_for_message_name(self: &Self, message_name: &str) -> Option<String> {
        let mut s = self
            .conn
            .prepare("SELECT tx_node_name FROM messages WHERE name = ?1")
            .unwrap();

        match s.query_row([message_name], |row| row.get(0)) {
            Ok(tx_node_name) => Some(tx_node_name),
            Err(_) => None,
        }
    }

    pub fn get_all_rx_msgs_for(self: &Self, node_name: &str) -> Vec<String> {
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
            .prepare("SELECT * FROM messages WHERE name = ?1")
            .unwrap();

        match s.query_row([message_name], |row| {
            Ok(CanMessage {
                name: row.get(0)?,
                id: row.get(1)?,
                description: row.get(2)?,
                cycle_time: row.get(3)?,
                log_cycle_time: row.get(4)?,
                telem_cycle_time: row.get(5)?,
                tx_node_name: row.get(6)?,
                modes: serde_json::from_str::<Vec<String>>(&row.get::<_, String>(7)?).unwrap(),
                signals: self.get_signals_for_message(row.get(1)?).unwrap(),
            })
        }) {
            Ok(msg) => Ok(msg),
            Err(e) => Err(CanDBError::SqlLiteError(e)),
        }
    }

    pub fn get_message_id_by_name(self: &Self, message_name: &str) -> Result<u32, CanDBError> {
        let mut s = self
            .conn
            .prepare("SELECT id FROM messages WHERE name = ?1")
            .unwrap();

        match s.query_row([message_name], |row| row.get(0)) {
            Ok(msg_id) => Ok(msg_id),
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
                log_cycle_time: row.get(4)?,
                telem_cycle_time: row.get(5)?,
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
                log_cycle_time: row.get(4)?,
                telem_cycle_time: row.get(5)?,
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
