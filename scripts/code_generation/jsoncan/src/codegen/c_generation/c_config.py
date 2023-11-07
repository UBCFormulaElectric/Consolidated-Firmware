"""
Function and variable names for C coddegen.
"""
from strenum import StrEnum
from ...utils import *


class CFuncsConfig(StrEnum):
    # Io Tx
    IO_TX_INIT = "Io_CanTx_Init"
    IO_TX_ENABLE_MODE = "Io_CanTx_EnableMode"
    IO_TX_ENQUEUE_PERIODIC = "Io_CanTx_Enqueue{freq}Msgs"
    IO_TX_ENQUEUE_OTHER_PERIODIC = "Io_CanTx_EnqueueOtherPeriodicMsgs"
    IO_TX_SEND = "Io_CanTx_{msg}_Send{mode}"

    # App Tx
    APP_TX_INIT = "App_CanTx_Init"
    APP_TX_SET_SIGNAL = "App_CanTx_{signal}_Set"
    APP_TX_GET_SIGNAL = "App_CanTx_{signal}_Get"
    APP_TX_GET_MSG = "App_CanTx_{msg}_GetData"

    # Io Rx
    IO_RX_UPDATE_TABLE = "Io_CanRx_UpdateRxTableWithMessage"
    IO_RX_FILTER_ID = "Io_CanRx_FilterMessageId"

    # App Rx
    APP_RX_INIT = "App_CanRx_Init"
    APP_RX_SET_SIGNAL = "App_CanRx_{signal}_Update"
    APP_RX_GET_SIGNAL = "App_CanRx_{signal}_Get"

    # Utils
    UTILS_PACK = "App_CanUtils_{msg}_Pack"
    UTILS_UNPACK = "App_CanUtils_{msg}_Unpack"

    # Alerts
    APP_ALERTS_INIT = "App_CanAlerts_Init"
    APP_ALERTS_SET = "App_CanAlerts_{alert}_Set"
    APP_ALERTS_GET = "App_CanAlerts_{alert}_Get"


class CVarsConfig(StrEnum):
    TX_TABLE = "tx_table"
    RX_TABLE = "rx_table"
    MSG_STRUCT = "{msg}_signals"
    SIGNAL_VALUE = "{signal}_value"
    ALERTS_TABLE = "alerts_table"


class CTypesConfig(StrEnum):
    TX_TABLE_STRUCT = "{node}_TxMsgs"
    RX_TABLE_STRUCT = "{node}_RxMsgs"
    MSG_STRUCT = "{msg}_Signals"
    ALERT_ENUM = "{node}_{alert_type}Id"
    CAN_ALERT_BOARD_ENUM = "CanAlertBoard"
    CAN_MSG_STRUCT = "CanMsg"


class CMacrosConfig(StrEnum):
    @classmethod
    def id(cls, msg):
        return f"CAN_MSG_{pascal_to_screaming_snake_case(msg)}_ID"

    @classmethod
    def cycle_time(cls, msg):
        return f"CAN_MSG_{pascal_to_screaming_snake_case(msg)}_CYCLE_TIME_MS"

    @classmethod
    def bytes(cls, msg):
        return f"CAN_MSG_{pascal_to_screaming_snake_case(msg)}_BYTES"

    @classmethod
    def start_val(cls, msg, signal):
        return f"CANSIG_{pascal_to_screaming_snake_case(msg)}_{pascal_to_screaming_snake_case(signal)}_START_VAL"

    @classmethod
    def scale(cls, msg, signal):
        return f"CANSIG_{pascal_to_screaming_snake_case(msg)}_{pascal_to_screaming_snake_case(signal)}_SCALE"

    @classmethod
    def offset(cls, msg, signal):
        return f"CANSIG_{pascal_to_screaming_snake_case(msg)}_{pascal_to_screaming_snake_case(signal)}_OFFSET"

    @classmethod
    def min(cls, msg, signal):
        return f"CANSIG_{pascal_to_screaming_snake_case(msg)}_{pascal_to_screaming_snake_case(signal)}_MIN"

    @classmethod
    def max(cls, msg, signal):
        return f"CANSIG_{pascal_to_screaming_snake_case(msg)}_{pascal_to_screaming_snake_case(signal)}_MAX"
