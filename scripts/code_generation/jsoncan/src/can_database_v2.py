"""
This file contains various classes to fully describes a CAN bus: The nodes, messages, and signals on the bus.
"""

from dataclasses import dataclass
from typing import List, Union, Dict
import logging
from strenum import StrEnum

from .json_parsing.schema_validation import AlertsEntry
from .utils import bits_for_uint, bits_to_bytes, is_int

import pandas as pd

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


class CanSignalDatatype(StrEnum):
    """
    Enum for the possible primitive datatypes of a CAN signal.
    """

    BOOL = "bool"
    INT = "int"
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
    start_val: Union[int, float]  # Default starting value, None if doesn't specify one
    enum: Union[CanEnum, None]  # Value table, None if doesn't specify one
    unit: str  # Signal's unit
    signed: bool  # Whether or not signal is represented as signed or unsigned
    description: str = "N/A"  # Description of signal

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

    def __str__(self):
        return self.name


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
    tx_node: str  # The node that transmits this message
    rx_nodes: List[str]  # All nodes which receive this message
    modes: List[str]  # List of modes which this message should be transmitted in
    log_cycle_time: Union[
        int, None
    ]  # Interval that this message should be logged to disk at (None if don't capture this msg)
    telem_cycle_time: Union[
        int, None
    ]  # Interval that this message should be sent via telem at (None if don't capture this msg)

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


@dataclass(frozen=True)
class CanBusConfig:
    """
    Dataclass for holding bus config.
    """

    default_receiver: str
    bus_speed: int
    modes: List[str]
    default_mode: str


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


@dataclass(frozen=True)
class CanDatabase:
    """
    Dataclass for fully describing a CAN bus, its nodes, and their messages.
    """

    nodes: List[str]  # List of names of the nodes on the bus
    bus_config: List[CanBusConfig]  # List of bus configurations, multiple buses supported
    shared_enums: List[CanEnum]  # Enums used by all nodes
    alerts: Dict[
        str, Dict[CanAlert, AlertsEntry]
    ]  # Dictionary of node to list of alerts set by node
    msgs: Dict[
        int, CanMessage
    ]  # All messages being sent to the bus (dict of (ID to message)

    pd_msgs: pd.DataFrame 


    def canMessage_to_pdR



    

    