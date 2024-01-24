"""
Function and variable names for C coddegen.
"""
from strenum import StrEnum
from ...utils import *


class CFuncsConfig(StrEnum):
    # Io Tx
    IO_TX_INIT = "io_canTx_init"
    IO_TX_ENABLE_MODE = "io_canTx_enableMode"
    IO_TX_ENQUEUE_PERIODIC = "io_canTx_enqueue{freq}Msgs"
    IO_TX_ENQUEUE_OTHER_PERIODIC = "io_canTx_enqueueOtherPeriodicMsgs"
    IO_TX_SEND = "io_canTx_{msg}_send{mode}"

    # App Tx
    APP_TX_INIT = "app_canTx_init"
    APP_TX_SET_SIGNAL = "app_canTx_{signal}_set"
    APP_TX_GET_SIGNAL = "app_canTx_{signal}_get"
    APP_TX_GET_MSG = "app_canTx_{msg}_getData"

    # Io Rx
    IO_RX_UPDATE_TABLE = "io_canRx_updateRxTableWithMessage"
    IO_RX_FILTER_ID = "io_canRx_filterMessageId"

    # App Rx
    APP_RX_INIT = "app_canRx_init"
    APP_RX_SET_SIGNAL = "app_canRx_{signal}_update"
    APP_RX_GET_SIGNAL = "app_canRx_{signal}_get"

    # Utils
    UTILS_PACK = "app_canUtils_{msg}_pack"
    UTILS_UNPACK = "app_canUtils_{msg}_unpack"

    # Alerts
    APP_ALERTS_SET = "app_canAlerts_{alert}_set"
    APP_ALERTS_GET = "app_canAlerts_{alert}_get"


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
    ALERT_ID_ENUM = "{node}_{alert_type}Code"
    CAN_ALERT_BOARD_ENUM = "CanAlertBoard"
    CAN_MSG_STRUCT = "JsonCanMsg"


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
