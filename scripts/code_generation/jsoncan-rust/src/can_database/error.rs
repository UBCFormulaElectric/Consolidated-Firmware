use std::fmt::Debug;

pub enum CanDBError {
    DuplicateTxMsgName {
        tx_node_name_1: String,
        tx_node_name_2: String,
        tx_msg_name: String,
    },
    DuplicateTxMsgID {
        tx_node_name_1: String,
        tx_node_name_2: String,
        tx_msg_name: String,
    },
    DuplicateTxSignalName {
        // duplicate within a single message
        signal_name: String,
        tx_msg_name: String,
    },
    RxMsgNotFound {
        rx_node_name: String,
        rx_msg_name: String,
    },
    RxLoopback {
        rx_node_name: String,
        rx_msg_name: String,
    },
    RxDuplicate {
        rx_node_name: String,
        rx_msg_name: String,
    },
    SqlLiteError(rusqlite::Error),
    NodeCannotForwardFrom {
        node_name: String,
        bus_not_on: String,
    },
    NodeCannotForwardTo {
        node_name: String,
        bus_not_on: String,
    },
    EnumMultipleDefinitions {
        enum_name: String,
        definition_locs: Vec<String>,
    },
    BusReferencesUndefinedNode {
        bus_name: String,
        node_name: String,
    },
    BusDefaultModeNotInModes {
        bus_name: String,
        default_mode: String,
    },
    BusNameCollision {
        bus_name: String,
    },
    ForwarderReferencesUndefinedBus {
        forwarder_name: String,
        bus_name: String,
    },
    ForwarderReferencesSameBus {
        forwarder_name: String,
        bus_name: String,
    },
    PoolConnectionError(r2d2::Error),
}

impl Debug for CanDBError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> Result<(), std::fmt::Error> {
        match self {
            CanDBError::DuplicateTxMsgName {
                tx_msg_name,
                tx_node_name_1,
                tx_node_name_2,
            } => write!(
                f,
                "Duplicate transmitted message name '{}' found in nodes '{}' and '{}'. Message names must be unique across all transmitting nodes.",
                tx_msg_name, tx_node_name_1, tx_node_name_2,
            ),
            CanDBError::DuplicateTxMsgID {
                tx_msg_name,
                tx_node_name_1,
                tx_node_name_2,
            } => write!(
                f,
                "Duplicate transmitted message ID found for message '{}' in nodes '{}' and '{}'. Message IDs must be unique across all transmitting nodes.",
                tx_msg_name, tx_node_name_1, tx_node_name_2,
            ),
            CanDBError::DuplicateTxSignalName {
                signal_name,
                tx_msg_name,
            } => write!(
                f,
                "Duplicate transmitted signal name '{}' found in message '{}'. Signal names must be unique across all transmitted messages.",
                signal_name, tx_msg_name
            ),
            CanDBError::RxMsgNotFound {
                rx_node_name,
                rx_msg_name: msg_name,
            } => write!(
                f,
                "Message '{}' received by '{}' is not defined. Make sure it is correctly defined in the TX JSON.",
                msg_name, rx_node_name
            ),
            CanDBError::RxLoopback {
                rx_node_name,
                rx_msg_name,
            } => write!(
                f,
                "'{}' cannot both transmit and receive '{}'",
                rx_node_name, rx_msg_name
            ),
            CanDBError::RxDuplicate {
                rx_node_name,
                rx_msg_name,
            } => write!(
                f,
                "Message '{}' is already registered to be received by node '{}'",
                rx_msg_name, rx_node_name
            ),
            CanDBError::SqlLiteError(error) => write!(f, "SQLite error: {}", error),
            CanDBError::NodeCannotForwardFrom {
                node_name,
                bus_not_on,
            } => write!(
                f,
                "{} cannot forward from {} as it is not on it",
                node_name, bus_not_on
            ),
            CanDBError::NodeCannotForwardTo {
                node_name,
                bus_not_on,
            } => write!(
                f,
                "{} cannot forward to {} as it is not on it",
                node_name, bus_not_on
            ),
            CanDBError::EnumMultipleDefinitions {
                enum_name,
                definition_locs,
            } => write!(
                f,
                "Enum '{}' is broadcast by multiple nodes: {:?}",
                enum_name, definition_locs
            ),
            CanDBError::BusReferencesUndefinedNode {
                bus_name,
                node_name,
            } => write!(
                f,
                "Bus '{}' references node '{}' which is not defined",
                bus_name, node_name
            ),
            CanDBError::BusDefaultModeNotInModes {
                bus_name,
                default_mode,
            } => write!(
                f,
                "Bus '{}' has default mode '{}' which is not in its list of modes",
                bus_name, default_mode
            ),
            CanDBError::BusNameCollision { bus_name } => write!(
                f,
                "Multiple buses with the name '{}' found. Bus names must be unique.",
                bus_name
            ),
            CanDBError::ForwarderReferencesUndefinedBus {
                forwarder_name,
                bus_name,
            } => write!(
                f,
                "Bus forwarder '{}' references bus '{}' which is not defined",
                forwarder_name, bus_name
            ),
            CanDBError::ForwarderReferencesSameBus {
                forwarder_name,
                bus_name,
            } => write!(
                f,
                "Bus forwarder '{}' references the same bus '{}' for both sides of the forwarder. A forwarder must connect two different buses.",
                forwarder_name, bus_name
            ),
            CanDBError::PoolConnectionError(error) => write!(f, "Pool connection error: {}", error),
        }
    }
}
