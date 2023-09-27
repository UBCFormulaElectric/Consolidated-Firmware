"""
Module for generating a DBC file from a CanDatabase object.
TODO: Adding descriptions of messages to DBC
"""
from typing import List
from ...can_database import *

DBC_TEMPLATE = """\
VERSION ""
NS_:
BS_:


{nodes}

{messages}

{attribute_definitions}

{start_value_attributes}

{cycle_time_attributes}

{value_tables}

"""
DBC_BOARD_LIST = "BU_: {node_names}\n"
DBC_MESSAGE_TEMPLATE = "BO_ {id} {name}: {num_bytes} {tx_node}\n"
DBC_SIGNAL_TEMPLATE = 'SG_ {name} : {bit_start}|{num_bits}{endianness}{signed} ({scale},{offset}) [{min_val}|{max_val}] "{unit}" {rx_node_names}\n'
DBC_ATTRIBUTE_DEFINITONS_TEMPLATE = """\
BA_DEF_  "BusType" STRING ;
BA_DEF_DEF_  "BusType" "CAN";
BA_ "BusType" "CAN";
"""
DBC_ATTRIBUTE_TEMPLATE = (
    'BA_ "{attr_name}" {attr_operand} {id} {signal_name} {value};\n'
)
DBC_VALUE_TABLE_TEMPLATE = "VAL_ {id} {signal_name} {entries};\n"

# BA_DEF_ BO_  "GenMsgCycleTime" INT {cycle_time_min} {cycle_time_max};
# BA_DEF_ SG_  "GenSigStartValue" INT {start_value_min} {start_value_max};
# BA_DEF_DEF_  "GenMsgCycleTime" {cycle_time_default};
# BA_DEF_DEF_  "GenSigStartValue" {start_value_default};


class DbcGenerator:
    """
    Class for generating a DBC file from a CanDatabase object.
    """

    def __init__(self, database: CanDatabase):
        self._db = database

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
        for msg in self._db.msgs:
            # Generate text for CAN message
            msgs_text += self._dbc_message(msg=msg, tx_node=msg.tx_node)

            # If message has cycle time, generate text
            if msg.is_periodic():
                cycle_time_attributes_text += self._dbc_msg_cycle_time_attribute(
                    value=msg.cycle_time,
                    msg_id=msg.id,
                )

            for signal in msg.signals:
                # Generate text for current CAN signal
                msgs_text += self._dbc_signal(signal=signal, rx_nodes=msg.rx_nodes)

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
        """
        Return space-delimitted list of all boards on the bus.
        """
        return DBC_BOARD_LIST.format(
            node_names=" ".join(self._db.nodes + [self._db.bus_config.default_receiver])
        )

    def _dbc_message(self, msg: CanMessage, tx_node: str) -> str:
        """
        Format and return DBC message definition.
        """
        return DBC_MESSAGE_TEMPLATE.format(
            id=msg.id, name=msg.name, num_bytes=msg.bytes(), tx_node=tx_node
        )

    def _dbc_signal(self, signal: CanSignal, rx_nodes: List[str]) -> str:
        """
        Format and return DBC signal definition.
        """
        return DBC_SIGNAL_TEMPLATE.format(
            name=signal.name,
            bit_start=signal.start_bit,
            num_bits=signal.bits,
            scale=signal.scale,
            offset=signal.offset,
            min_val=signal.min_val,
            max_val=signal.max_val,
            unit=signal.unit,
            rx_node_names=",".join(rx_nodes),
            endianness=f"@1",  # TODO: Big endianness
            signed="-" if signal.signed else "+",
        )

    def _attribute_definitions(self) -> str:
        """
        Format and attribute definitions and defaults.
        """
        bus = self._db.bus_config
        # return DBC_ATTRIBUTE_DEFINITONS_TEMPLATE.format(
        #     cycle_time_min=bus.cycle_time_min,
        #     cycle_time_max=bus.cycle_time_max,
        #     cycle_time_default=bus.cycle_time_default,
        #     start_value_min=bus.start_value_min,
        #     start_value_max=bus.start_value_max,
        #     start_value_default=bus.start_value_default,
        # )
        return DBC_ATTRIBUTE_DEFINITONS_TEMPLATE

    def _dbc_msg_cycle_time_attribute(self, value: int, msg_id: int) -> str:
        """
        Format and return DBC GenMsgCycleTime message attribute.
        """
        return DBC_ATTRIBUTE_TEMPLATE.format(
            id=msg_id,
            attr_name="GenMsgCycleTime",
            attr_operand="BO_",
            signal_name="",
            value=value,
        )

    def _dbc_signal_start_val_attribute(self, signal: CanSignal, msg_id: int) -> str:
        """
        Format and return DBC GenSigStartValue signal attribute.
        """
        return DBC_ATTRIBUTE_TEMPLATE.format(
            id=msg_id,
            attr_name="GenSigStartValue",
            attr_operand="SG_",
            signal_name=signal.name,
            value=signal.start_val,
        )

    def _dbc_value_table(self, signal: CanSignal, msg_id: int) -> str:
        """
        Format and return DBC value table.
        """
        entries_text = " ".join(
            [f'{entry.value} "{entry.name}"' for entry in signal.enum.items]
        )

        return DBC_VALUE_TABLE_TEMPLATE.format(
            id=msg_id, signal_name=signal.name, entries=entries_text
        )
