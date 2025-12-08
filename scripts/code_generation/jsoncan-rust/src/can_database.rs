use std::collections::{HashMap, HashSet};

pub struct CanBus {
    pub name: String,
    pub bus_speed: u32,
    pub modes: Vec<String>,
    pub default_mode: String,
    pub fd: bool,

    // List of nodes on this bus, foreign key into CanDatabase.nodes, although provided in json
    pub node_names: Vec<String>,
}

pub enum RxMsgNames {
    All,
    RxMesgs(HashSet<String>), // TODO check if we need this to be a set? not a vector
}

//     struct for fully describing a CAN node.
//     Each CanNode object should be able to independently generate (notwithstanding foreign keys) all code related to that node
pub struct CanNode {
    // Name of this CAN node
    pub name: String,
    // busses which the node is attached to, foreign key into CanDatabase.msgs
    pub bus_names: Vec<String>,
    pub rx_msgs_names: RxMsgNames, // list of messages that it is listening
    pub fd: bool,
}

pub enum CanSignalType {
    Numerical,
    Enum,
    Alert,
}

pub struct CanSignal {
    // Name of this CAN signal
    pub name: String,
    // Start bit of this signal in the message payload
    pub start_bit: u8,
    // Number of bits used to represent this signal in the message payload
    pub bits: u8,
    // Scaling factor for encoding/decoding this signal
    pub scale: f64,
    // Offset for encoding/decoding this signal
    pub offset: f64,
    // Minimum allowed value for this signal
    pub min: f64,
    // Maximum allowed value for this signal
    pub max: f64,
    // Default starting value, None if doesn't specify one
    pub start_val: f64,
    // enum: Optional[CanEnum]  # Value table, None if doesn't specify one
    pub enum_name: Option<String>,
    // Unit of this signal
    pub unit: Option<String>,
    // Whether or not signal is represented as signed or unsigned
    pub signed: bool,
    // Description of this signal
    pub description: Option<String>,
    // Whether or not this signal is big-endian
    pub big_endian: bool, // TODO: Add tests for big endianness

    pub signal_type: CanSignalType,
}

pub struct CanMessage {
    // Name of this CAN message
    pub name: String,
    // Message ID
    pub id: u32,
    // Message description
    pub description: Option<String>,
    // Interval that this message should be transmitted at, if periodic. None if aperiodic.
    pub cycle_time: Option<u32>,
    // All signals that make up this message
    pub signals: Vec<CanSignal>,
    // Interval that this message should be logged to disk at (None if don't capture this msg)
    pub log_cycle_time: Option<u32>,
    // Interval that this message should be sent via telem at (None if don't capture this msg)
    pub telem_cycle_time: Option<u32>,

    // back references, hence are foreign keys
    // note that these simply list sources and destinations of messages, and not how to get between them
    // we store them to find how to travel between them, and they are used in dbcs
    pub tx_node_name: String,

    // if this is None, then only use the bus default
    pub modes: Option<Vec<String>>,
}

fn bits_to_bytes(bits: u8) -> u8 {
    //  Get number of bytes needed to store bits number of bits.
    (bits + 7) / 8
}

const ALLOWABLE_MSG_LENGTHS: [u8; 16] = [0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64];
impl CanMessage {
    pub fn dlc(&self) -> u8 {
        // Length of payload, in bytes.
        if self.signals.len() == 0 {
            return 0;
        }

        let useful_length = bits_to_bytes(
            self.signals
                .iter()
                .map(|signal| signal.start_bit + signal.bits)
                .max()
                .expect("Message has signals (already checked length > 0)"),
        );

        for length in ALLOWABLE_MSG_LENGTHS.iter() {
            if *length >= useful_length {
                return *length;
            }
        }

        panic!("This message was created with an invalid DLC!");
    }

    pub fn requires_fd(&self) -> bool {
        self.dlc() > 8
    }
}

pub enum CanAlertType {
    Warning,
    Fault,
    Info,
}

pub struct CanAlert {
    pub name: String,
    pub alert_type: CanAlertType,
    pub id: u32,
    pub description: String,
}

#[derive(Clone)]
pub struct CanEnum {
    pub name: String,
    // mapping from enum name to value
    pub values: HashMap<String, u32>,
}

impl CanEnum {
    pub fn bits(&self) -> u8 {
        // Calculate number of bits needed to represent this enum
        todo!()
    }

    pub fn max_value(&self) -> u32 {
        // Calculate maximum value of this enum
        todo!()
    }

    pub fn min_value(&self) -> u32 {
        // Calculate minimum value of this enum
        todo!()
    }
}

pub struct BusForwarder {
    pub bus1: String,
    pub bus2: String,
    pub forwarder_name: String,
}

pub struct CanDatabase {
    // nodes[node_name] gives metadata for node_name
    pub nodes: HashMap<String, CanNode>,
    // buses[bus_name] gives metadata for bus_name
    pub buses: HashMap<String, CanBus>,
    // msgs[msg_name] gives metadata for msg_name
    pub msgs: HashMap<String, CanMessage>,
    // alerts[node_name] gives a list of alerts on that node
    pub alerts: HashMap<String, Vec<CanAlert>>,
    // enums[enum_name] gives metadata for enum_name
    pub enums: HashMap<String, CanEnum>,
    // collects_data[node_name] is true if this node collects data
    pub collects_data: HashMap<String, bool>,
    // signals_to_msgs[signal_name] gives the message that contains the signal
    // pub signals_to_msgs: HashMap<String, &CanMessage>,

    // this must be global state rather than local (node) state as the common usecase is navigation
    // which requires global information
    pub forwarding: Vec<BusForwarder>,
}
