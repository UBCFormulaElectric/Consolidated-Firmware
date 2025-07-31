"""
This file contains various classes to fully describes a CAN bus: The nodes, messages, and signals on the bus.
"""

from __future__ import annotations

import logging
from dataclasses import dataclass
from typing import Dict, List, Optional, Set, Union, Any

import pandas as pd
from strenum import StrEnum

from cantools.database.can.message import Message as CanToolsMessage
from cantools.database.can.signal import Signal as CanToolSignal
from cantools.typechecking import EncodeInputType

from .utils import bits_to_bytes, pascal_to_screaming_snake_case, pascal_to_snake_case
from .can_signals import CanSignal

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

    def screaming_snake_name(self):
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
    
    def build_cantools_message(self) -> CanToolsMessage:
        return CanToolsMessage(
            frame_id=self.id, name=self.name, length=self.dlc(),
            signals=[CanToolSignal(
                name=signal.name, start=signal.start_bit, length=signal.bits,
                is_signed=signal.signed, minimum=signal.min_val, maximum=signal.max_val,
                unit=signal.unit
            ) for signal in self.signals]
        )
    
    def unpack(self, data: bytes) -> List[DecodedSignal]:
        """
        Unpack a CAN dataframe.
        Returns a list of decoded signals as `DecodedSignal` objects.

        TODO: Also add packing!
        """
        decoded_signals: List[DecodedSignal] = []
        data_uint = int.from_bytes(data, byteorder="little", signed=False)

        for signal in self.signals:
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
                    logger.warning( f"Signal value '{scaled_value}' not found in enum '{signal.enum.name}' for signal '{signal.name}'.")
                    continue
                decoded.label = enum_label
            decoded_signals.append(decoded)

        return decoded_signals
    
    def pack(self, data: EncodeInputType) -> bytes:
        ctm = self.build_cantools_message()
        return ctm.encode(data)

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


class AllRxMsgs:
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
    rx_msgs_names: Set[str] | AllRxMsgs  # list of messages that it is listening
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
