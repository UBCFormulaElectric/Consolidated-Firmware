"""
This file contains various classes to fully describes a CAN bus: The nodes, messages, and signals on the bus.
"""

from __future__ import annotations

import logging
from dataclasses import dataclass
from typing import Dict, List, Optional, Set, Union

import pandas as pd
from strenum import StrEnum

from .utils import (bits_for_uint, bits_to_bytes, is_int,
                    pascal_to_screaming_snake_case, pascal_to_snake_case)

logger = logging.getLogger(__name__)


@dataclass(frozen=True)
class CanBus:
    """
    Dataclass for holding bus config.
    """

    name: str
    bus_speed: int
    modes: List[str]
    default_mode: str
    fd: bool  # Whether or not this bus is FD
    # List of nodes on this bus, foreign key into CanDatabase.nodes, although provided in json
    node_names: List[str]

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
    big_endian: bool = False  # TODO: Add tests for big endianness

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
        elif (
            self.min_val == 0
            and self.max_val == 1
            and self.scale == 1
            and self.offset == 0
        ):
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


@dataclass(frozen=True)
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

    # if this is None, then only use the bus default
    modes: Optional[List[str]]

    def dlc(self):
        """
        Length of payload, in bytes.
        """
        if len(self.signals) == 0:
            return 0

        useful_length = bits_to_bytes(
            max([signal.start_bit + signal.bits for signal in self.signals])
        )

        allowable_lengths = [0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64]
        for length in allowable_lengths:
            if length >= useful_length:
                return length

        raise RuntimeError("This message was created with an invalid DLC!")

    def is_periodic(self):
        """
        If this signal is periodic, i.e. should be continuously transmitted at a certain cycle time.
        """
        return self.cycle_time is not None

    def requires_fd(self) -> bool:
        return self.dlc() > 8

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

    def dlc_macro(self):
        return f"CAN_MSG_{self.snake_name().upper()}_DLC"

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


class All:
    pass


@dataclass
class CanNode:
    """
    Dataclass for fully describing a CAN node.
    Each CanNode object should be able to independently generate (notwithstanding foreign keys) all code related to that node
    """

    name: str  # Name of this CAN node
    # busses which the node is attached to, foreign key into CanDatabase.msgs
    bus_names: List[str]
    rx_msgs_names: Set[str] | All  # list of messages that it is listening
    fd: bool

    def __hash__(self):
        return hash(self.name)

    def __str__(self):
        return self.name


@dataclass
class DecodedSignal:
    name: str
    value: float
    unit: Optional[str] = None
    label: Optional[str] = None


@dataclass(frozen=True)
class CanDatabase:
    """
    Dataclass for fully describing a CAN bus, its nodes, and their messages.
    """

    nodes: Dict[str, CanNode]  # nodes[node_name] gives metadata for node_name
    # bus_config[bus_name] gives metadata for bus_name
    buses: Dict[str, CanBus]
    msgs: Dict[str, CanMessage]  # msgs[msg_name] gives metadata for msg_name
    # alerts[node_name] gives a list of alerts on that node
    alerts: Dict[str, list[CanAlert]]
    enums: Dict[str, CanEnum]  # enums[enum_name] gives metadata for enum_name
    # collects_data[node_name] is true if this node collects data
    collects_data: Dict[str, bool]
    # signals_to_msgs[signal_name] gives the message that contains the signal
    signals_to_msgs: Dict[str, CanMessage]

    # this must be global state rather than local (node) state as the common usecase is navigation
    # which requires global information
    forwarding: List[BusForwarder]

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

    def unpack(self, msg_id: int, data: bytes) -> List[DecodedSignal]:
        """
        Unpack a CAN dataframe.
        Returns a list of decoded signals as `DecodedSignal` objects.

        TODO: Also add packing!
        """
        msg = self.get_message_by_id(msg_id)
        if msg is None:
            logger.warning(f"Message ID '{msg_id}' is not defined in the JSON.")
            return []

        decoded_signals: List[DecodedSignal] = []
        data_uint = int.from_bytes(data, byteorder="little", signed=False)

        for signal in msg.signals:
            # Extract the bits representing the current signal.
            data_shifted = data_uint >> signal.start_bit
            bitmask = (1 << signal.bits) - 1
            raw_value = data_shifted & bitmask

            # Handle signed values via 2's complement
            if signal.signed and (raw_value & (1 << (signal.bits - 1))):
                raw_value = raw_value - (1 << signal.bits)

            # Apply scale and offset
            scaled_value = raw_value * signal.scale + signal.offset

            # Initialize decoded signal
            decoded = DecodedSignal(name=signal.name, value=scaled_value)

            if signal.unit:
                decoded.unit = signal.unit

            if signal.enum:
                enum_label = signal.enum.items.get(int(scaled_value))
                if enum_label is None:
                    logger.warning(
                        f"Signal value '{scaled_value}' not found in enum '{signal.enum.name}' for signal '{signal.name}'."
                    )
                    continue
                decoded.label = enum_label

            decoded_signals.append(decoded)

        return decoded_signals

    def get_message_by_id(self, id: int):
        for msg in self.msgs.values():
            if msg.id == id:
                return msg
        return None


@dataclass()
class BusForwarder:
    forwarder: str
    bus1: str
    bus2: str

    def __eq__(self, value):
        if isinstance(value, BusForwarder):
            return (
                self.forwarder == value.forwarder
                and self.bus1 == value.bus1
                and self.bus2 == value.bus2
            )
        return False

    def __lt__(self, value):
        if isinstance(value, BusForwarder):
            return (
                self.forwarder < value.forwarder
                or (self.forwarder == value.forwarder and self.bus1 < value.bus1)
                or (
                    self.forwarder == value.forwarder
                    and self.bus1 == value.bus1
                    and self.bus2 < value.bus2
                )
            )
        return False
