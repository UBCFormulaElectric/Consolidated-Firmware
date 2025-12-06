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
    AllRxMsgs,
    SomeRxMsgs(std::collections::HashSet<String>),
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

pub struct CanSignal {
    // Name of this CAN signal
    pub name: String,
    // Start bit of this signal in the message payload
    pub start_bit: u32,
    // Number of bits used to represent this signal in the message payload
    pub bits: u32,
    // Scaling factor for encoding/decoding this signal
    pub scale: f64,
    // Offset for encoding/decoding this signal
    pub offset: f64,
    // Minimum allowed value for this signal
    pub min_val: f64,
    // Maximum allowed value for this signal
    pub max_val: f64,
    // Default starting value, None if doesn't specify one
    pub start_val: Option<f64>,
    // enum: Optional[CanEnum]  # Value table, None if doesn't specify one
    pub enum_name: Option<String>,
    // Unit of this signal
    pub unit: String,
    // Whether or not signal is represented as signed or unsigned
    pub signed: bool,
    // Description of this signal
    pub description: String,
    // Whether or not this signal is big-endian
    pub big_endian: bool, // TODO: Add tests for big endianness
}

pub struct CanMessage {
    // Name of this CAN message
    pub name: String,
    // Message ID
    pub id: u32,
    // Message description
    pub description: String,
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

fn bits_to_bytes(bits: u32) -> u32 {
    //  Get number of bytes needed to store bits number of bits.
    (bits + 7) / 8
}

impl CanMessage {
    pub fn dlc(&self) -> u32 {
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

        static ALLOWABLE_MSG_LENGTHS: [u32; 16] =
            [0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64];
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
    name: String,
    alert_type: CanAlertType,
    id: u32,
    description: String,
}

#[derive(Clone)]
pub struct CanEnum {
    name: String,
    description: String,
    // mapping from enum name to value
    values: std::collections::HashMap<String, u32>,
}

pub struct CanDatabase {
    // nodes[node_name] gives metadata for node_name
    pub nodes: std::collections::HashMap<String, CanNode>,
    // buses[bus_name] gives metadata for bus_name
    pub buses: std::collections::HashMap<String, CanBus>,
    // msgs[msg_name] gives metadata for msg_name
    pub msgs: std::collections::HashMap<String, CanMessage>,
    // alerts[node_name] gives a list of alerts on that node
    pub alerts: std::collections::HashMap<String, Vec<CanAlert>>,
    // enums[enum_name] gives metadata for enum_name
    pub enums: std::collections::HashMap<String, CanEnum>,
    // collects_data[node_name] is true if this node collects data
    pub collects_data: std::collections::HashMap<String, bool>,
    // signals_to_msgs[signal_name] gives the message that contains the signal
    pub signals_to_msgs: std::collections::HashMap<String, &CanMessage>,

    // this must be global state rather than local (node) state as the common usecase is navigation
    // which requires global information
    pub forwarding: Vec<BusForwarder>,
}
