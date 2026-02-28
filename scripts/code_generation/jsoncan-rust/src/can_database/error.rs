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
        signal_name: String,
        tx_msg_name_1: String,
        tx_msg_name_2: String,
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
    }
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
                tx_msg_name_1,
                tx_msg_name_2,
            } => write!(
                f,
                "Duplicate transmitted signal name '{}' found in messages '{}' and '{}'. Signal names must be unique across all transmitted messages.",
                signal_name, tx_msg_name_1, tx_msg_name_2
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
            CanDBError::NodeCannotForwardFrom { node_name, bus_not_on } => write!(f,  "{} cannot forward from {} as it is not on it", node_name, bus_not_on),
            CanDBError::NodeCannotForwardTo { node_name, bus_not_on } => write!(f,  "{} cannot forward to {} as it is not on it", node_name, bus_not_on),
        }
    }
}
