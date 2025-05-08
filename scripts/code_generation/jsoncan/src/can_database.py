"""
This file contains various classes to fully describes a CAN bus: The nodes, messages, and signals on the bus.
"""

from __future__ import annotations

import logging
from dataclasses import dataclass
from typing import Dict, List, Optional, Set, Union, DefaultDict

import pandas as pd
from strenum import StrEnum

from .utils import (
    bits_for_uint,
    bits_to_bytes,
    is_int,
    pascal_to_screaming_snake_case,
    pascal_to_snake_case,
)

logger = logging.getLogger(__name__)


@dataclass()
class CanBus:
    """
    Dataclass for holding bus config.
    """

    name: str
    bus_speed: int
    modes: List[str]
    default_mode: str
    fd: bool  # Whether or not this bus is FD

    # foreign key into CanDatabase.nodes
    node_names: List[str]  # List of nodes on this bus

    def __hash__(self):
        return hash(self.name)


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
    INT = "int32_t"
    UINT = "uint32_t"
    FLOAT = "float"


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
    # Default starting value, None if doesn't specify one
    start_val: Union[int, float]
    enum: Optional[CanEnum]  # Value table, None if doesn't specify one
    unit: str  # Signal's unit
    signed: bool  # Whether or not signal is represented as signed or unsigned
    description: str = "N/A"  # Description of signal
    message: Optional[CanMessage] = None  # Message this signal belongs to

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

    # back references, hence are foreign keys
    # note that these simply list sources and destinations of messages, and not how to get between them
    # we store them to find how to travel between them, and they are used in dbcs
    tx_node_name: str  # Node which transmits this message

    modes: Optional[List[str]]  # if this is None, then only use the bus default

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
        return f"CAN_MSG_{self.snake_name().upper()}_ID"

    def cycle_time_macro(self):
        return f"CAN_MSG_{self.snake_name().upper()}_CYCLE_TIME_MS"

    def bytes_macro(self):
        return f"CAN_MSG_{self.snake_name().upper()}_BYTES"

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
    INFO = "Info"  # Informational alerts, not critical


@dataclass(frozen=True)
class CanAlert:
    """
    Dataclass for a CAN alert.
    """
    name: str
    alert_type: CanAlertType
    id: int
    description: str


class CanTxConfigs:
    _map_by_msg_name: Dict[str, Set[str]]  # each message can be sent on many busses

    def __init__(self):
        self._map_by_msg_name = {}

    def add_tx_msg(self, msg_name: str):
        self._map_by_msg_name[msg_name] = set()

    def add_bus_to_tx_msg(self, msg_name: str, tx_bus: str):
        self._map_by_msg_name[msg_name].add(tx_bus)

    def get_busses_for_msg(self, msg_name: str) -> List[str]:
        return list(self._map_by_msg_name[msg_name])

    def list_msg_names(self):
        return self._map_by_msg_name.keys()


class CanRxConfigs:
    _map_by_bus: Dict[str, Set[str]]  # each bus can receive many messages
    _map_by_msg_name: Dict[str, str]  # each message can only be received by one bus

    def __init__(self):
        self._map_by_bus = {}
        self._map_by_msg_name = {}

    def add_rx_bus(self, bus_name: str):
        assert bus_name not in self._map_by_bus, "Bus already exists"
        self._map_by_bus[bus_name] = set()

    def add_rx_msg(self, msg_name: str, rx_bus: str):
        assert msg_name not in self._map_by_msg_name  # do this instead of figuring out how to remove them
        self._map_by_bus[rx_bus].add(msg_name)
        self._map_by_msg_name[msg_name] = rx_bus

    def get_msgs_on_bus(self, bus_name: str) -> List[str]:
        return list(self._map_by_bus[bus_name])

    def get_bus_of_msg(self, msg_name: str) -> str:
        return self._map_by_msg_name[msg_name]

    def get_all_rx_msgs_names(self) -> List[str]:
        return list(self._map_by_msg_name.keys())

    def contains_rx_msg(self, msg_name: str) -> bool:
        return msg_name in self._map_by_msg_name

    def empty(self):
        return len(self._map_by_msg_name) == 0


class CanNode:
    """
    Dataclass for fully describing a CAN node.
    Each CanNode object should be able to independently generate (notwithstanding foreign keys) all code related to that node
    """
    name: str  # Name of this CAN node
    bus_names: List[str]  # busses which the node is attached to, foreign key into CanDatabase.msgs

    # CALCULATED VALUES
    rx_config: CanRxConfigs  # rx_config[msg_name] gives a rx config for that message
    tx_config: CanTxConfigs  # tx_config[msg_name] gives a tx config for that message
    reroute_config: Optional[List[CanForward]] = None  # forwarding rule table

    def __init__(self, name: str):
        self.name = name
        self.bus_names = []
        self.rx_config = CanRxConfigs()
        self.tx_config = CanTxConfigs()
        self.reroute_config = None

    def add_bus(self, bus_name: str):
        """
        Add a bus to this node.
        """
        assert bus_name not in self.bus_names, "Bus already exists"  # do this instead of figuring out how to remove them
        self.bus_names.append(bus_name)
        self.rx_config.add_rx_bus(bus_name)

    def __hash__(self):
        return hash(self.name)

    def __str__(self):
        return self.name


@dataclass(frozen=True)
class CanDatabase:
    """
    Dataclass for fully describing a CAN bus, its nodes, and their messages.
    """

    nodes: Dict[str, CanNode]  # nodes[node_name] gives metadata for node_name
    busses: Dict[str, CanBus]  # bus_config[bus_name] gives metadata for bus_name
    msgs: Dict[str, CanMessage]  # msgs[msg_name] gives metadata for msg_name
    alerts: Dict[str, list[CanAlert]]  # alerts[node_name] gives a list of alerts on that node

    def make_pandas_dataframe(self):
        # Create a pandas dataframe from the messages
        data = []
        for msg in self.msgs.values():
            for signal in msg.signals:
                data.append(
                    {
                        "message": msg.name,
                        "signal": signal.name,
                        "start_bit": signal.start_bit,
                        "bits": signal.bits,
                        "scale": signal.scale,
                        "offset": signal.offset,
                        "min_val": signal.min_val,
                        "max_val": signal.max_val,
                        "start_val": signal.start_val,
                        "unit": signal.unit,
                        "signed": signal.signed,
                        "description": signal.description,
                        "tx_node": msg.tx_node_name,
                        "signal_obj": signal,
                        "message_obj": msg,
                    }
                )
        pandas_data = pd.DataFrame(data)
        return pandas_data

    def tx_msgs_for_node(self, tx_node: str) -> List[CanMessage]:
        """
        Return list of all CAN messages transmitted by a specific node.
        """
        if tx_node not in self.nodes:
            raise KeyError(f"Node '{tx_node}' is not defined in the JSON.")
        return [self.msgs[msg] for msg in self.nodes[tx_node].tx_config.list_msg_names()]

    def rx_msgs_for_node(self, rx_node: str) -> List[CanMessage]:
        """
        Return list of all CAN messages received by a specific node.
        """
        if rx_node not in self.nodes:
            raise KeyError(f"Node '{rx_node}' is not defined in the JSON.")
        return [self.msgs[msg_name] for msg_name in self.nodes[rx_node].rx_config.get_all_rx_msgs_names()]

    def msgs_for_node(self, node: str) -> List[CanMessage]:
        """
        Return list of all CAN messages either transmitted or received by a specific node.
        """
        return self.tx_msgs_for_node(tx_node=node) + self.rx_msgs_for_node(rx_node=node)

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

    def node_rx_alerts(self, node: str) -> List[str]:
        """
        Return list of alerts received by a node, of a specific type.
        """
        rte = []
        for tx_node in [
            node1
            for node1 in self.nodes
            if any(
                [len(self.node_alerts(node1, alert_type)) > 0 for alert_type in CanAlertType]
            )
        ]:
            if tx_node == node:
                continue  # Skip self-transmitted alerts
            for alert in self.alerts[tx_node]:
                rte.append(alert.name)
        return rte

    def unpack(self, msg_id: int, data: bytes) -> list[Dict]:
        """
        Unpack a CAN dataframe.
        Returns a dict with the signal name, value, and unit.

        TODO: Also add packing!
        """
        if str(msg_id) not in self.msgs:
            logger.warning(f"Message ID '{msg_id}' is not defined in the JSON.")
            return []

        signals = []
        for signal in self.msgs[str(msg_id)].signals:
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

                signal_data["label"] = signal.enum.items[
                    int(signal_value)
                ]  # this is legal as enums must be ints

            # Append decoded signal's data.
            signals.append(signal_data)

        return signals

    # def node_name_description(
    #         self, node: str, alert_type: CanAlert
    # ) -> Dict[str, tuple]:
    #     """Returns a dictionary containing a the alert names as the key and a description and as the item"""
    #
    #     new_dict = {}
    #     if node not in self.alerts:
    #         return {}
    #     for alert, info in self.alerts[node].items():
    #         if alert.alert_type == alert_type and info != {}:
    #             new_dict[alert.name] = (info["id"], info["description"])
    #
    #         elif info == {}:
    #             new_dict[alert.name] = {}
    #     return new_dict
    # def node_alerts_with_rx_check(
    #         self, tx_node: str, rx_node, alert_type: CanAlertType
    # ) -> List[str]:
    #     """
    #     Return list of alerts transmitted by tx_node, and received by rx_node, of a specific type.
    #     """
    #     if tx_node == rx_node:
    #         # A node "receives" its own alerts
    #         return self.node_alerts(tx_node, alert_type)
    #     else:
    #         alert_msg = next(
    #             msg
    #             for msg in self.msgs.values()
    #             if msg.name == f"{tx_node}_{alert_type}s"
    #         )
    #         return [
    #             alert
    #             for alert in self.node_alerts(tx_node, alert_type)
    #             if rx_node in alert_msg.rx_node_names
    #         ]
    # def node_has_tx_msgs(self, node: str) -> bool:
    #     """
    #     Return whether or not a node transmits any messages.
    #     """
    #     return len(self.tx_msgs_for_node(node)) > 0
    # def node_has_rx_msgs(self, node: str) -> bool:
    #     """
    #     Return whether or not a node receives any messages.
    #     """
    #     return len(self.rx_msgs_for_node(node)) > 0


@dataclass()
class CanForward:
    message: str  # name of the message
    forwarder: str
    from_bus: str  # name of the bus the message is forwarded from
    to_bus: str  # bus the message is forwarded to

    def __eq__(self, other):
        return (
                self.message == other.message
                and self.forwarder == other.forwarder
                and self.from_bus == other.from_bus
                and self.to_bus == other.to_bus
        )

    def __hash__(self):
        return hash((self.message, self.forwarder, self.from_bus, self.to_bus))

    def __str__(self):
        return f"Forwarding {self.message} from {self.from_bus} to {self.to_bus} via {self.forwarder}"
