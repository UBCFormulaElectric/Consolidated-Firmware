"""
Module for generating a DBC file from a CanDatabase object.
TODO: Adding descriptions of messages to DBC
"""

from ...can_database import *
from ..c_generation.routing import CanRxConfig

DBC_TEMPLATE = """\
VERSION ""
NS_:
BS_:


{nodes}

{messages}

{attribute_definitions}

{value_tables}

"""
DBC_BOARD_LIST = "BU_: {node_names}\n"
DBC_MESSAGE_TEMPLATE = "BO_ {id} {name}: {num_bytes} {tx_node}\n"
DBC_SIGNAL_TEMPLATE = 'SG_ {name} : {bit_start}|{num_bits}{endianness}{signed} ({scale},{offset}) [{min_val}|{max_val}] "{unit}" {rx_node_names}\n'
DBC_ATTRIBUTE_DEFINITONS_TEMPLATE = """\
BA_DEF_  "BusType" STRING ;
BA_DEF_DEF_  "BusType" "CAN";
BA_ "BusType" "CAN FD";
"""
DBC_ATTRIBUTE_TEMPLATE = (
    'BA_ "{attr_name}" {attr_operand} {id} {signal_name} {value};\n'
)
DBC_VALUE_TABLE_TEMPLATE = "VAL_ {id} {signal_name} {entries};\n"

# If a signal isn't recevied by any node then DBC parsing might fail.
# Hacky solution: Every signal receives "DEBUG"
DBC_DEFAULT_RECEIVER = "DEBUG"


class DbcGenerator:
    """
    Class for generating a DBC file from a CanDatabase object.
    """

    def __init__(self, database: CanDatabase, rx_configs: Dict[str, CanRxConfig]):
        self._db = database
        self._rx_configs = rx_configs

    def source(self) -> str:
        """
        Create the DBC file, and write it to the output path.
        """
        # Generate text for list of CAN nodes
        nodes_text = self._dbc_board_list()

        msgs_text = ""
        value_tables_text = ""
        cycle_time_attributes_text = ""
        signal_start_values_text = ""
        for msg in self._db.msgs.values():
            # Generate text for CAN message
            msgs_text += self._dbc_message(msg=msg)

            # If message has cycle time, generate text
            if msg.is_periodic():
                cycle_time_attributes_text += self._dbc_msg_cycle_time_attribute(
                    value=msg.cycle_time,
                    msg_id=msg.id,
                )

            rx_nodes: List[str] = [
                node_name
                for node_name, node_rx_config in self._rx_configs.items()
                if msg.name in node_rx_config.get_all_rx_msgs_names()
            ]
            for signal in msg.signals:
                # Generate text for current CAN signal
                msgs_text += self._dbc_signal(signal=signal, rx_nodes=rx_nodes)

                # Generate text for signal value table, if it has one
                if signal.enum:
                    value_tables_text += self._dbc_value_table(
                        signal=signal, msg_id=msg.id
                    )

                # Generate text for non-default start value
                if signal.has_non_default_start_val():
                    signal_start_values_text += self._dbc_signal_start_val_attribute(
                        msg_id=msg.id,
                        signal=signal,
                    )

            msgs_text += "\n"

        # Write final file output text to file
        return DBC_TEMPLATE.format(
            nodes=nodes_text,
            messages=msgs_text,
            attribute_definitions=self._attribute_definitions(),
            start_value_attributes=signal_start_values_text,
            cycle_time_attributes=cycle_time_attributes_text,
            value_tables=value_tables_text,
        )

    def _dbc_board_list(self) -> str:
        """default_receiver
        Return space-delimitted list of all boards on the bus.
        """
        boards = list(self._db.nodes.keys()) + [DBC_DEFAULT_RECEIVER]

        return DBC_BOARD_LIST.format(node_names=" ".join(boards))

    def _attribute_definitions(self) -> str:
        """
        Format and attribute definitions and defaults.
        """
        return DBC_ATTRIBUTE_DEFINITONS_TEMPLATE

    @staticmethod
    def _process_msg_id(msg_id: int) -> int:
        """
        Process message ID to ensure it is in the correct format for DBC.
        """
        # The 31st bit needs to be set to indicate this message has an extended
        # ID, otherwise CANoe doesn't decode it properly.
        if msg_id >= 2**11:
            msg_id |= 2**31
        return msg_id

    @staticmethod
    def _dbc_message(msg: CanMessage) -> str:
        """
        Format and return DBC message definition.
        """
        return DBC_MESSAGE_TEMPLATE.format(
            id=DbcGenerator._process_msg_id(msg.id), name=msg.name, num_bytes=msg.dlc(), tx_node=msg.tx_node_name
        )

    @staticmethod
    def _dbc_signal(signal: CanSignal, rx_nodes: List[str]) -> str:
        """
        Format and return DBC signal definition.
        """
        rx_nodes = (
            (rx_nodes + [DBC_DEFAULT_RECEIVER])
            if DBC_DEFAULT_RECEIVER not in rx_nodes
            else rx_nodes
        )

        start_bit = signal.start_bit
        if signal.big_endian:
            # If big endian then the start bit is the most significant bit,
            # which is the most significant bit taken up in the least
            # significant byte (because big endian). Wow this is dumb!
            start_bit = min(
                (signal.start_bit // 8 * 8) + 7, signal.start_bit + signal.bits
            )

        return DBC_SIGNAL_TEMPLATE.format(
            name=signal.name,
            bit_start=start_bit,
            num_bits=signal.bits,
            scale=signal.scale,
            offset=signal.offset,
            min_val=signal.min_val,
            max_val=signal.max_val,
            unit=signal.unit,
            rx_node_names=",".join(rx_nodes),
            endianness="@0" if signal.big_endian else "@1",
            signed="-" if signal.signed else "+",
        )

    @staticmethod
    def _dbc_msg_cycle_time_attribute(value: int, msg_id: int) -> str:
        """
        Format and return DBC GenMsgCycleTime message attribute.
        """
        return DBC_ATTRIBUTE_TEMPLATE.format(
            id=DbcGenerator._process_msg_id(msg_id),
            attr_name="GenMsgCycleTime",
            attr_operand="BO_",
            signal_name="",
            value=value,
        )

    @staticmethod
    def _dbc_signal_start_val_attribute(signal: CanSignal, msg_id: int) -> str:
        """
        Format and return DBC GenSigStartValue signal attribute.
        """
        return DBC_ATTRIBUTE_TEMPLATE.format(
            id=DbcGenerator._process_msg_id(msg_id),
            attr_name="GenSigStartValue",
            attr_operand="SG_",
            signal_name=signal.name,
            value=signal.start_val,
        )

    @staticmethod
    def _dbc_value_table(signal: CanSignal, msg_id: int) -> str:
        """
        Format and return DBC value table.
        """
        entries_text = " ".join(
            [
                f'{item_value} "{item_name}"'
                for item_value, item_name in signal.enum.items.items()
            ]
        )

        return DBC_VALUE_TABLE_TEMPLATE.format(
            id=DbcGenerator._process_msg_id(msg_id), signal_name=signal.name, entries=entries_text
        )
