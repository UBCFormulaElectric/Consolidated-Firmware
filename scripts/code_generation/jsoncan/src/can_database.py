"""
This file contains various classes to fully describes a CAN bus: The nodes, messages, and signals on the bus.
"""
from dataclasses import dataclass
from enum import Enum
from strenum import StrEnum
from typing import List, Union, Dict
from .utils import bits_for_uint, bits_to_bytes, is_int


@dataclass(frozen=True)
class CanEnumItem:
    """
    Dataclass for describing a single value table item.
    """

    name: str
    value: int


@dataclass(frozen=True)
class CanEnum:
    """
    Dataclass for fully describing a CAN value table.
    Note: Minimum value is assumed to always be 0. TODO: Change this?
    """

    name: str
    items: List[CanEnumItem]

    def max_val(self) -> int:
        """
        Maximum value present in this value table's entries.
        """
        return max(entry.value for entry in self.items)

    def min_val(self) -> int:
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
        return self.cycle_time != None


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
    
    
class BoardNumbering (StrEnum):
    
    """
    Enum for the possible boards that can be faulting or sending a warning
    """
    
    BMS = "1" #The numbering is associated with the order of LED on the DIM
    FSM = "2"
    DCM = "3"
    DIM = "4"
    PDM = "5"
    JCT = "6"
    
class BMS_FaultWarningCode (StrEnum):
    
    BMS_Warning_StackWaterMarkHighTask1Hz = "01"
    BMS_Warning_StackWaterMarkHighTask100Hz = "02"
    BMS_Warning_StackWaterMarkHighTask1kHz = "03"
    BMS_Warning_StackWaterMarkHighTaskCanRx = "04"
    BMS_Warning_StackWaterMarkHighTaskCanTx = "05"
    BMS_Warning_WatchdogTimeout = "06"
    BMS_Warning_TxOverflow = "07"
    BMS_Warning_RxOverflow = "08"
    BMS_Fault_MissingHeartbeat = "51"
    BMS_Fault_StateMachine = "52"
    BMS_Fault_CellUndervoltage = "53"
    BMS_Fault_CellOvervoltage = "54",
    BMS_Fault_ModuleCommunicationError = "55"
    BMS_Fault_CellUndertemp = "56"
    BMS_Fault_CellOvertemp = "57"
    BMS_Fault_Charger = "58"
    BMS_Fault_ChargerDisconnectedDuringCharge = "59"
    BMS_Fault_ChargerExternalShutdown = "60"
    BMS_Fault_TractiveSystemOvercurrent = "61"
    BMS_Fault_PrechargeFailure = "62"
    
    
class FSM_FaultWarningCode (StrEnum):
    FSM_Warning_StackWaterMarkHighTask1Hz ="01"
    FSM_Warning_StackWaterMarkHighTask100Hz = "02"
    FSM_Warning_StackWaterMarkHighTask1kHz = "03"
    FSM_Warning_FSM_Warning_StackWaterMarkHighTaskCanRx = "04"
    FSM_Warning_StackWaterMarkHighTaskCanTx = "05"
    FSM_Warning_WatchdogTimeout = "06"
    FSM_Warning_TxOverflow = "07"
    FSM_Warning_RxOverflow = "08"
    FSM_Warning_LeftWheelSpeedOutOfRange = "09"
    FSM_Warning_RightWheelSpeedOutOfRange = "10"
    FSM_Warning_SteeringAngleOCSC = "11"
    FSM_Warning_SteeringAngleOutOfRange = "12"
    FSM_Warning_FrontBrakePressureOutOfRange = "13"
    FSM_Warning_RearBrakePressureOutOfRangeWarning = "14"
    FSM_Warning_FlowRateOutOfRange = "15"
    FSM_Warning_BrakeAppsDisagreement = "16"
    FSM_Warning_AppsDisagreement = "17"
    FSM_Fault_MissingHeartbeat = "51"
    FSM_Fault_StateMachine = "52"
    FSM_Fault_PappsOCSC = "53"
    FSM_Fault_SappsOCSCFault = "54"
    FSM_Fault_FlowMeterUnderflow = "55"
    FSM_Fault_TorquePlausabilityFailed = "56"
    
    FSM_Warning_Warning_Test1 = "57"
    FSM_Warning_Warning_Test2 = "58"
    FSM_Fault_Fault_Test3 = "59"
    
class DCM_FaultWarningCode (StrEnum):
    DCM_Warning_StackWaterMarkHighTask1Hz = "01"
    DCM_Warning_StackWaterMarkHighTask100Hz = "02"
    DCM_Warning_StackWaterMarkHighTask1kHz = "03"
    DCM_Warning_StackWaterMarkHighTaskCanRx = "04"
    DCM_Warning_StackWaterMarkHighTaskCanTx = "05"
    DCM_Warning_WatchdogTimeout = "06"
    DCM_Warning_TxOverflow = "07"
    DCM_Warning_RxOverflow = "08"
    DCM_Fault_MissingHeartbeat = "51"
    DCM_Fault_LeftInverterFault = "52"
    DCM_Fault_RightInverterFault = "53"
    
    
class DIM_FaultWarningCode (StrEnum):
    DIM_Warning_StackWaterMarkHighTask1Hz = "01"
    DIM_Warning_StackWaterMarkHighTask100Hz = "02"
    DIM_Warning_StackWaterMarkHighTask1kHz = "03"
    DIM_Warning_StackWaterMarkHighTaskCanRx = "04"
    DIM_Warning_StackWaterMarkHighTaskCanTx = "05"
    DIM_Warning_WatchdogTimeout = "06"
    DIM_Warning_TxOverflow = "07"
    DIM_Warning_RxOverflow = "08"
    DIM_Fault_MissingHeartbeat = "51"
    
class PDM_FaultWarningCode (StrEnum):
    PDM_Warning_StackWaterMarkHighTask1Hz = "01"
    PDM_Warning_StackWaterMarkHighTask100Hz = "02"
    PDM_Warning_StackWaterMarkHighTask1kHz = "03"
    PDM_Warning_StackWaterMarkHighTaskCanRx = "04"
    PDM_Warning_StackWaterMarkHighTaskCanTx ="05"
    PDM_Warning_WatchdogTimeout = "06"
    PDM_Warning_TxOverflow = "07"
    PDM_Warning_RxOverflow = "08"
    
class JCT(StrEnum):
    JCT_Warning_Warning_Test = "01"


class FaultWarningCodeRetriever:
    def __init__(self):
        self.board_mappings = {
            'BMS': {code.name: code.value for code in BMS_FaultWarningCode},
            'FSM': {code.name: code.value for code in FSM_FaultWarningCode},
            'DCM': {code.name: code.value for code in DCM_FaultWarningCode},
            'DIM': {code.name: code.value for code in DIM_FaultWarningCode},
            'PDM': {code.name: code.value for code in PDM_FaultWarningCode},
            'JCT': {code.name: code.value for code in JCT}
        }
        
    def get_fault_warning_code(self, board_type, alert_signal):
        if board_type in self.board_mappings:
            board_class = self.board_mappings[board_type]
            try:
                return BoardNumbering[board_type] + board_class[alert_signal]+";"
            except KeyError:
                return " 0;"
        else:
            return " 0;"

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
    bus_config: CanBusConfig  # Various bus params
    msgs: List[CanMessage]  # All messages being sent to the bus
    shared_enums: List[CanEnum]  # Enums used by all nodes
    alerts: Dict[
        str, Dict[CanAlert,int]
    ]  # Dictionary of node to list of alerts set by node

    def tx_msgs_for_node(self, tx_node: str) -> List[CanMessage]:
        """
        Return list of all CAN messages transmitted by a specific node.
        """
        return [msg for msg in self.msgs if tx_node == msg.tx_node]

    def rx_msgs_for_node(self, rx_node: str) -> List[CanMessage]:
        """
        Return list of all CAN messages received by a specific node.
        """
        return [msg for msg in self.msgs if rx_node in msg.rx_nodes]

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
        
    def node_IDcodes(self, node: str, alert_type :CanAlert) -> Dict[str,int]:
        
        return(
            {
                alert:
                codeID
                for alert, codeID in self.alerts[node].items()
                if alert.alert_type == alert_type
            }
            
            if node in self.alerts
            else {}
        )

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
                msg for msg in self.msgs if msg.name == f"{tx_node}_{alert_type}s"
            )
            return [
                alert
                for alert in self.node_alerts(tx_node, alert_type)
                if rx_node in alert_msg.rx_nodes
            ]

    def node_has_alert(self, node: str, alert_type: CanAlertType) -> bool:
        """
        Return whether or not a node transmits any alerts.
        """
        return len(self.node_alerts(node, alert_type)) > 0
