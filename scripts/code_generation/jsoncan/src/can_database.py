"""
This file contains various classes to fully describes a CAN bus: The nodes, messages, and signals on the bus.
"""

from __future__ import annotations

import logging
from dataclasses import dataclass
from typing import Dict, List, Set, Union

from strenum import StrEnum

from .json_parsing.schema_validation import AlertsEntry
from .utils import (bits_for_uint, bits_to_bytes, is_int,
                    pascal_to_screaming_snake_case, pascal_to_snake_case)

logger = logging.getLogger(__name__)


@dataclass(frozen=True)
class CanEnum:
    """
    Dataclass for fully describing a CAN value table.
    Note: Minimum value is assumed to always be 0. TODO: Change this?
    """

    name: str
    items: Dict[int, str]  # Dict of enum value to enum item name

    def max_val(self) -> int:
        """
        Maximum value present in this value table's entries.
        """
        return max(self.items.keys())

    @staticmethod
    def min_val() -> int:
        """
        Minimum value present in this value table's entries.
        """
        return 0

    def num_bits(self) -> int:
        """
        Number of bits needed to store this value table.
        """
        return bits_for_uint(self.max_val())

    def snake_name(self):
        return pascal_to_snake_case(self.name)

    def scremming_snake_name(self):
        return pascal_to_screaming_snake_case(self.name)

    def __str__(self):
        return self.name

    def __hash__(self):
        return hash(self.name)


class CanSignalDatatype(StrEnum):
    """
    Enum for the possible primitive datatypes of a CAN signal.
    """

    BOOL = "bool"
    INT = "int"
    UINT = "uint32_t"
    FLOAT = "float"


@dataclass()
class CanBusConfig:
    """
    Dataclass for holding bus config.
    """

    bus_speed: int
    modes: List[str]
    default_mode: str
    name: str
    nodes: List[CanNode]  # List of nodes on this bus

    def __hash__(self):
        return hash(self.name)


@dataclass(frozen=True)
class CanSignal:
    """
    Dataclass for fully describing a CAN signal.
    """

    name: str  # Name of signal
    start_bit: int  # Start bit of signal in payload
    bits: int  # Number of bits to represent signal in payload, in bits
    scale: float  # Scale for encoding/decoding
    offset: float  # Offset for encoding/decoding
    min_val: float  # Min allowed value
    max_val: float  # Max allowed value
    start_val: Union[int, float]  # Default starting value, None if doesn't specify one
    enum: Union[CanEnum, None]  # Value table, None if doesn't specify one
    unit: str  # Signal's unit
    signed: bool  # Whether or not signal is represented as signed or unsigned
    description: str = "N/A"  # Description of signal
    message: CanMessage = None  # Message this signal belongs to

    def represent_as_integer(self):
        """
        If this signal holds integer or unsigned integer values only.
        """
        return is_int(self.scale) and is_int(self.offset)

    def has_unit(self):
        """
        If this signal specifies a unit.
        """
        return self.unit != ""

    def has_non_default_start_val(self):
        """
        If this signal specifies a starting value.
        """
        return self.start_val is not None

    def representation(self):
        """
        How this signal will be represented in memory (specific to C).
        For example, in C, enums and booleans are both stored internally as unsigned ints.
        """
        if self.enum:
            return CanSignalDatatype.UINT
        elif self.represent_as_integer():
            if self.min_val >= 0:
                return CanSignalDatatype.UINT
            else:
                return CanSignalDatatype.INT
        else:
            return CanSignalDatatype.FLOAT

    def datatype(self):
        """
        The name the datatype this signal should be stored as (specific to C).
        """
        if self.enum:
            return self.enum.name
        elif self.min_val == 0 and self.max_val == 1:
            return CanSignalDatatype.BOOL
        elif self.represent_as_integer():
            if self.min_val >= 0:
                return CanSignalDatatype.UINT
            else:
                return CanSignalDatatype.INT
        else:
            return CanSignalDatatype.FLOAT

    def snake_name(self):
        return pascal_to_snake_case(self.name)

    def scremming_snake_name(self):
        return pascal_to_screaming_snake_case(self.name)

    def start_val_macro(self):
        return f"CANSIG_{self.snake_name().upper()}_START_VAL"

    def max_val_macro(self):
        return f"CANSIG_{self.snake_name().upper()}_MAX_VAL"

    def min_val_macro(self):
        return f"CANSIG_{self.snake_name().upper()}_MIN_VAL"

    def scale_macro(self):
        return f"CANSIG_{self.snake_name().upper()}_SCALE"

    def offset_macro(self):
        return f"CANSIG_{self.snake_name().upper()}_OFFSET"

    def __str__(self):
        return self.name

    def __hash__(self):
        return hash(self.name)


@dataclass()
class CanMessage:
    """
    Dataclass for fully describing a CAN message.
    """

    name: str  # Name of this CAN message
    id: int  # Message ID
    description: str  # Message description
    cycle_time: Union[
        int, None
    ]  # Interval that this message should be transmitted at, if periodic. None if aperiodic.
    signals: List[CanSignal]  # All signals that make up this message
    log_cycle_time: Union[
        int, None
    ]  # Interval that this message should be logged to disk at (None if don't capture this msg)
    telem_cycle_time: Union[
        int, None
    ]  # Interval that this message should be sent via telem at (None if don't capture this msg)

    # forgein key
    bus: List[str]  # List of buses this message is transmitted on
    tx_node: str  # Node which transmits this message
    rx_nodes: List[str]  # List of nodes which receive this message
    def bytes(self):
        """
        Length of payload, in bytes.
        """
        if len(self.signals) == 0:
            return 0

        return bits_to_bytes(
            max([signal.start_bit + signal.bits for signal in self.signals])
        )

    def is_periodic(self):
        """
        If this signal is periodic, i.e. should be continuously transmitted at a certain cycle time.
        """
        return self.cycle_time is not None

    def snake_name(self):
        return pascal_to_snake_case(self.name)

    def scremming_snake_name(self):
        return pascal_to_screaming_snake_case(self.name)

    # type of the message
    def c_type(self):
        return self.name + "_Signals"

    def id_macro(self):
        return f"CANMSG_{self.snake_name().upper()}_ID"

    def cycle_time_macro(self):
        return f"CANMSG_{self.snake_name().upper()}_CYCLE_TIME"

    def bytes_macro(self):
        return f"CANMSG_{self.snake_name().upper()}_BYTES"

    def __str__(self):
        return self.name

    def __hash__(self):
        return hash(self.id)


class CanAlertType(StrEnum):
    """
    Enum for the possible types of CAN alerts.
    """

    WARNING = "Warning"  # Warnings sent periodically, for notifying driver
    FAULT = "Fault"  # Faults sent periodically, contactors open if a fault is set


@dataclass(frozen=True)
class CanAlert:
    """
    Dataclass for a CAN alert.
    """

    name: str
    alert_type: CanAlertType


@dataclass()
class CanNode:
    """
    Dataclass for fully describing a CAN node.
    """

    name: str  # Name of this CAN node
    
    # forgein key
    tx_msgs: List[str]
    rx_msgs: List[str]
    alerts: List[AlertsEntry]
    buses: List[str]

    def get_all_messages(self):
        rx = list(self.rx_msgs.values())
        tx = list(self.tx_msgs.values())
        return rx + tx

    def __hash__(self):
        return hash(self.name)

    def __str__(self):
        return self.name


@dataclass(frozen=True)
class CanDatabase:
    """
    Dataclass for fully describing a CAN bus, its nodes, and their messages.
    """

    nodes: Dict[str, CanNode]  # List of names of the nodes on the bus
    bus_config: Dict[str, CanBusConfig]  # Various bus params
    msgs: Dict[
        int, CanMessage
    ]  # All messages being sent to the bus (dict of (ID to message)
    shared_enums: Dict[str, CanEnum]  # Enums used by all nodes
    alerts: Dict[
        str, Dict[CanAlert, AlertsEntry]
    ]  # Dictionary of node to list of alerts set by node
    reroute_msgs: List[CanForward]  # List of messages to be forwarded to another bus
    forwarder: CanNode  # Node which forwards this message

    def tx_msgs_for_node(self, tx_node: str) -> List[CanMessage]:
        """
        Return list of all CAN messages transmitted by a specific node.
        """
        node = self.nodes[tx_node]
        
        return [self.msgs[msg] for msg in node.tx_msgs]

    def rx_msgs_for_node(self, rx_node: str) -> List[CanMessage]:
        """
        Return list of all CAN messages received by a specific node.
        """
        node = self.nodes[rx_node]
        return [self.msgs[msg] for msg in node.rx_msgs]

    def msgs_for_node(self, node: str) -> List[CanMessage]:
        """
        Return list of all CAN messages either transmitted or received by a specific node.
        """
        return self.tx_msgs_for_node(tx_node=node) + self.rx_msgs_for_node(rx_node=node)

    def node_has_tx_msgs(self, node: str) -> bool:
        """
        Return whether or not a node transmits any messages.
        """
        return len(self.tx_msgs_for_node(node)) > 0

    def node_has_rx_msgs(self, node: str) -> bool:
        """
        Return whether or not a node receives any messages.
        """
        return len(self.rx_msgs_for_node(node)) > 0

    def node_alerts(self, node: str, alert_type: CanAlertType) -> List[str]:
        """
        Return list of alerts transmitted by a node, of a specific type.
        """
        return (
            [
                alert.name
                for alert in self.alerts[node]
                if alert.alert_type == alert_type
            ]
            if node in self.alerts
            else []
        )

    def node_name_description(
        self, node: str, alert_type: CanAlert
    ) -> Dict[str, tuple]:
        """Returns a dictionary containing a the alert names as the key and a description and as the item"""

        new_dict = {}
        if node not in self.alerts:
            return {}
        for alert, info in self.alerts[node].items():
            if alert.alert_type == alert_type and info != {}:
                new_dict[alert.name] = (info["id"], info["description"])

            elif info == {}:
                new_dict[alert.name] = {}
        return new_dict

    def get_board_node(self):
        nodes_with_alerts = [
            node
            for node in self.nodes
            if any(
                [self.node_has_alert(node, alert_type) for alert_type in CanAlertType]
            )
        ]

        return nodes_with_alerts

    def node_alerts_all_description(self):
        """Returns a dictionary containing a the alert names as the key and a description and as the item"""

        new_dict = {}
        for node, alerts in self.alerts.items():
            for alert, info in alerts.items():
                if info != {}:
                    new_dict[alert.name] = (info["id"], info["description"])
        return new_dict

    def node_alerts_with_rx_check(
        self, tx_node: str, rx_node, alert_type: CanAlertType
    ) -> List[str]:
        """
        Return list of alerts transmitted by tx_node, and received by rx_node, of a specific type.
        """
        if tx_node == rx_node:
            # A node "receives" its own alerts
            return self.node_alerts(tx_node, alert_type)
        else:
            alert_msg = next(
                msg
                for msg in self.msgs.values()
                if msg.name == f"{tx_node}_{alert_type}s"
            )
            return [
                alert
                for alert in self.node_alerts(tx_node, alert_type)
                if rx_node in alert_msg.rx_nodes
            ]

    def node_rx_alerts(self, node: str, type) -> List[str]:
        """
        Return list of alerts received by a node, of a specific type.
        """
        rte = []
        for tx_node in self.get_board_node():
            if tx_node == node:
                continue  # Skip self-transmitted alerts
            for alert, alert_entry in self.alerts[tx_node].items():
                rte.append(alert.name)
        return rte

    def node_has_alert(self, node: str, alert_type: CanAlertType) -> bool:
        """
        Return whether or not a node transmits any alerts.
        """
        return len(self.node_alerts(node, alert_type)) > 0

    def unpack(self, id: int, data: bytes) -> Dict:
        """
        Unpack a CAN dataframe.
        Returns a dict with the signal name, value, and unit.

        TODO: Also add packing!
        """
        if id not in self.msgs:
            logger.warning(f"Message ID '{id}' is not defined in the JSON.")
            return []

        signals = []
        for signal in self.msgs[id].signals:
            # Interpret raw bytes as an int.
            data_uint = int.from_bytes(data, byteorder="little", signed=False)

            # Extract the bits representing the current signal.
            data_shifted = data_uint >> signal.start_bit
            bitmask = (1 << signal.bits) - 1
            signal_bits = data_shifted & bitmask

            # Interpret value as signed number via 2s complement.
            if signal.signed:
                if signal_bits & (1 << (signal.bits - 1)):
                    signal_bits = ~signal_bits & ((1 << signal.bits) - 1)
                    signal_bits += 1
                    signal_bits *= -1

            # Decode the signal value using the scale/offset.
            signal_value = signal_bits * signal.scale + signal.offset
            signal_data = {"name": signal.name, "value": signal_value}

            # Insert unit, if it exists.
            if signal.unit is not None:
                signal_data["unit"] = signal.unit

            # If the signal is an enum, insert its label.
            if signal.enum is not None:
                if signal_value not in signal.enum.items:
                    logger.warning(
                        f"Signal value '{signal_value}' does not have a matching label in enum '{signal.enum.name}' for signal '{signal.name}'."
                    )
                    continue

                signal_data["label"] = signal.enum.items[signal_value]

            # Append decoded signal's data.
            signals.append(signal_data)

        return signals


@dataclass()
class CanForward:
    message: CanMessage  # Message needed to be forwarded
    receive_node: CanNode
    # bus: List[CanBusConfig]  # List of buses will be forwarded to
    forwarder: CanNode  # Node which forwards the message
