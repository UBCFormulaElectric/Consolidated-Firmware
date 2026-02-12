use std::collections::HashMap;

#[derive(PartialEq)]
pub struct CanBus {
    pub name: String,
    pub bus_speed: u32,
    pub modes: Vec<String>,
    pub default_mode: String,
    pub fd: bool,

    // List of nodes on this bus, foreign key into CanDatabase.nodes, although provided in json
    pub node_names: Vec<String>,
}

#[derive(Clone)]
pub enum RxMsgNames {
    All,
    RxMsgs(Vec<String>),
}

pub struct GroupedAlerts {
    pub infos: Vec<CanAlert>,
    pub warnings: Vec<CanAlert>,
    pub faults: Vec<CanAlert>,
    pub infos_id: u32,
    pub infos_count_id: u32,
    pub warnings_id: u32,
    pub warnings_count_id: u32,
    pub faults_id: u32,
    pub faults_count_id: u32,
}

//     struct for fully describing a CAN node.
//     Each CanNode object should be able to independently generate (notwithstanding foreign keys) all code related to that node
pub struct CanNode {
    // Name of this CAN node
    pub name: String,
    pub rx_msgs_names: RxMsgNames, // list of messages that it is listening
    pub collects_data: bool,
    pub alerts: Option<GroupedAlerts>,
    pub enums: Vec<CanEnum>,
}

#[derive(Clone)]
pub enum CanSignalType {
    Numerical,
    Enum,
    Alert,
}
pub struct CanSignal {
    // Name of this CAN signal
    pub name: String,
    // Start bit of this signal in the message payload
    pub start_bit: u16,
    // Number of bits used to represent this signal in the message payload
    pub bits: u16,
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
    pub modes: Vec<String>,
}

const ALLOWABLE_MSG_LENGTHS: [u16; 16] = [0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64];
fn bits_to_bytes(bits: u16) -> u16 {
    //  Get number of bytes needed to store bits number of bits.
    (bits + 7) / 8
}
impl CanMessage {
    pub fn dlc(&self) -> u16 {
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

    pub fn dbc_id(&self) -> u32 {
        if self.id >= 1 << 11 {
            self.id | (1 << 31)
        } else {
            self.id
        }
    }
}

#[derive(Clone)]
pub enum CanAlertType {
    Warning,
    Fault,
    Info,
}

#[derive(Clone)]
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
    pub fn bits(&self) -> u16 {
        // Calculate number of bits needed to represent this enum
        let max_value = self.max_value();
        for bits in 1u16..=32 {
            if max_value < (1 << bits) {
                return bits;
            }
        }
        panic!("Enum has a value that is too large to represent in 32 bits");
    }

    pub fn max_value(&self) -> u32 {
        return self
            .values
            .values()
            .cloned()
            .max()
            .expect("Enum has at least one value");
    }

    pub fn min_value(&self) -> u32 {
        0
    }
}

pub struct BusForwarder {
    pub bus1_name: String,
    pub bus2_name: String,
    pub forwarder_name: String,
}
