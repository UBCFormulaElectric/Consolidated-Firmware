from ...can_database import *
from ...utils import *
from .c_config import *
from .c_writer import *


class IoCanRxModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node
        self._functions = self._filter_id_func()

    def _filter_id_func(self) -> CFunc:
        func = CFunc(
            CFuncsConfig.IO_RX_FILTER_ID,
            "bool",
            args=[
                CVar("std_id", "uint32_t"),
            ],
            comment=f"Returns true if {self._node} receives the specified message ID.",
        )

        # Add cases
        func.body.add_line("bool is_found = false;")
        func.body.start_switch("std_id")

        if len(self._db.rx_msgs_for_node(self._node)) > 0:
            for msg in self._db.rx_msgs_for_node(self._node):
                func.body.add_switch_case(CMacrosConfig.id(msg.name))

            # If found msg, set is_found to true
            func.body.start_switch_case()
            func.body.add_line("is_found = true;")
            func.body.add_switch_break()

        # Otherwise, do nothing
        func.body.add_switch_default()
        func.body.start_switch_case()
        func.body.add_comment("Do nothing")
        func.body.add_switch_break()
        func.body.end_switch()

        # Return result
        func.body.add_line()
        func.body.add_line("return is_found;")
        return func

    def _update_rx_table_func(self) -> CFunc:
        func = CFunc(
            CFuncsConfig.IO_RX_UPDATE_TABLE,
            "void",
            args=[
                CVar("msg", "CanMsg*"),
            ],
            comment=f"Unpack a received message and update the CAN RX table.",
        )

        func.body.start_switch("msg->std_id")
        for msg in self._db.rx_msgs_for_node(self._node):
            func.body.add_switch_case(CMacrosConfig.id(msg.name))
            func.body.start_switch_case()

            # Unpack RXed message
            func.body.add_var_declaration(
                CVar("out_msg", CTypesConfig.MSG_STRUCT.format(msg=msg.name))
            )
            func.body.add_line(
                f"{CFuncsConfig.UTILS_UNPACK.format(msg=msg.name)}(msg->data, &out_msg);"
            )
            func.body.add_line()

            # Update RX table signals
            for signal in msg.signals:
                func.body.add_line(
                    f"{CFuncsConfig.APP_RX_SET_SIGNAL.format(msg=msg.name, signal=signal.name)}(out_msg.{CVarsConfig.SIGNAL_VALUE.format(signal=signal.name)});"
                )
            func.body.add_switch_break()

        # Msg id not received by this node, do nothing
        func.body.add_switch_default()
        func.body.start_switch_case()
        func.body.add_comment("Do nothing")
        func.body.add_switch_break()
        func.body.end_switch()
        return func

    def header(self):
        cw = CWriter()
        cw.add_preamble()
        cw.add_line()
        cw.add_line("#pragma once")

        # Add includes
        cw.add_line()
        cw.add_header_comment("Includes")
        cw.add_line()
        cw.add_include("<stdint.h>")
        cw.add_include("<stdbool.h>")
        cw.add_include('"Io_CanTx.h"')

        # Add function prototypes
        cw.add_line()
        cw.add_header_comment("Function Prototypes")
        cw.add_line()

        cw.add_function_prototype(self._filter_id_func())
        cw.add_line()
        cw.add_function_prototype(self._update_rx_table_func())
        cw.add_line()

        return str(cw)

    def source(self):
        cw = CWriter()
        cw.add_preamble()
        cw.add_line()

        # Add includes
        cw.add_header_comment("Includes")
        cw.add_line()
        cw.add_include('"Io_CanRx.h"')
        cw.add_include('"App_CanRx.h"')
        cw.add_include('"App_CanUtils.h"')

        # Add function definitions
        cw.add_line()
        cw.add_header_comment("Function Definitions")
        cw.add_line()

        cw.add_function_definition(self._filter_id_func())
        cw.add_line()
        cw.add_function_definition(self._update_rx_table_func())
        cw.add_line()

        return str(cw)
