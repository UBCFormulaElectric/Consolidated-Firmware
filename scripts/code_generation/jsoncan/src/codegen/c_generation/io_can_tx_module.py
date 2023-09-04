from lzma import is_check_supported
from statistics import mode
from ...can_database import *
from ...utils import *
from .c_config import *
from .c_writer import *

MSG_CYCLE_TIME_2_FREQ = {
    1000: "1Hz",
    10: "100Hz",
}


class IoCanTxModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node
        self._public_functions, self._static_functions = self._functions()

    def _functions(self) -> List[CFunc]:
        public_funcs = []
        static_funcs = []

        # Init the IO module
        func_init = CFunc(
            CFuncsConfig.IO_TX_INIT,
            "void",
            args=[
                CVar(
                    "(*transmit_tx_msg_func)(const CanMsg*)",
                    "void",
                )
            ],
            comment="Initialzie the IO CAN TX module.",
        )
        func_init.body.add_line("transmit_func = transmit_tx_msg_func;")
        public_funcs.append(func_init)

        # Set the current CAN mode
        func_set_mode = CFunc(
            CFuncsConfig.IO_TX_ENABLE_MODE,
            "void",
            args=[CVar("mode", "CanMode"), CVar("enable", "bool")],
            comment="Enable or disable a mode (only messages allowed for the enabled modes transmitted on the bus).",
        )
        func_set_mode.body.start_if("enable")
        func_set_mode.body.add_line("can_mode |= (uint32_t)mode; // Enable mode")
        func_set_mode.body.add_line()
        func_set_mode.body.start_else()
        func_set_mode.body.add_line("can_mode &= ~((uint32_t)mode); // Disable mode")
        func_set_mode.body.end_if()
        public_funcs.append(func_set_mode)

        # Generate periodic message sending functions
        for cycle_time, freq in MSG_CYCLE_TIME_2_FREQ.items():
            func = CFunc(
                CFuncsConfig.IO_TX_ENQUEUE_PERIODIC.format(freq=freq),
                "void",
                args=[],
                comment=f"Enqeue periodic CAN msgs of period {cycle_time}ms.",
            )

            for msg in self._db.tx_msgs_for_node(self._node):
                if msg.is_periodic() and msg.cycle_time == cycle_time:
                    func.body.add_line(
                        f"{CFuncsConfig.IO_TX_SEND.format(msg=msg.name, mode='Periodic')}();"
                    )

            public_funcs.append(func)

        # Generate 1kHz sending function
        func_1khz = CFunc(
            CFuncsConfig.IO_TX_ENQUEUE_OTHER_PERIODIC,
            "void",
            args=[CVar("time_ms", "uint32_t")],
            comment=f"Enqueue periodic CAN messages whose cycle time does is not suitable for other periodic sending functions.",
        )
        for msg in self._db.tx_msgs_for_node(self._node):
            if msg.is_periodic() and msg.cycle_time not in MSG_CYCLE_TIME_2_FREQ:
                func_1khz.body.start_if(
                    f"time_ms % {CMacrosConfig.cycle_time(msg.name)} == 0"
                )
                func_1khz.body.add_line(
                    f"{CFuncsConfig.IO_TX_SEND.format(msg=msg.name, mode='Periodic')}();"
                )
                func_1khz.body.end_if()
                func_1khz.body.add_line()
        public_funcs.append(func_1khz)

        # Generate msg sending functions
        for msg in self._db.tx_msgs_for_node(self._node):
            mode = "Periodic" if msg.is_periodic() else "Aperiodic"
            func = CFunc(
                CFuncsConfig.IO_TX_SEND.format(msg=msg.name, mode=mode),
                "void",
                args=[],
                comment=f'Pack and send the {"aperiodic" if not msg.is_periodic() else ""} TX msg {msg.name}.',
            )

            # Return early if not in the right mode
            func.body.start_if(
                f"can_mode & ({' | '.join([f'CAN_MODE_{mode.upper()}' for mode in msg.modes])})"
            )

            # Prepare header
            func.body.add_comment("Prepare msg header")
            func.body.add_var_declaration(CVar("tx_msg", "CanMsg"))
            func.body.add_line("memset(&tx_msg, 0, sizeof(CanMsg));")
<<<<<<< HEAD
            func.body.add_line(f"tx_msg.std_id = {CMacrosConfig.id(msg.name)};")
            func.body.add_line(f"tx_msg.dlc = {CMacrosConfig.bytes(msg.name)};")
=======
            func.body.add_line(f"tx_msg.msg_id = {CMacrosConfig.id(msg.name)};")
            func.body.add_line(f"tx_msg.length = {CMacrosConfig.bytes(msg.name)};")
>>>>>>> 8507a611 (refactor can driver)
            func.body.add_line()

            # Pack payload
            func.body.add_comment(
                "Prepare CAN msg payload (The packing function isn't thread-safe so we must guard it)"
            )
            func.body.add_line("#ifndef THREAD_SAFE_CAN_PACKING")
            func.body.add_line("vPortEnterCritical();")
            func.body.add_line("#endif")
            func.body.add_line(
                f"{CFuncsConfig.UTILS_PACK.format(msg=msg.name)}({CFuncsConfig.APP_TX_GET_MSG.format(msg=msg.name)}(), tx_msg.data);"
            )
            func.body.add_line("#ifndef THREAD_SAFE_CAN_PACKING")
            func.body.add_line("vPortExitCritical();")
            func.body.add_line("#endif")
            func.body.add_line()

            # Push to TX FIFO
            func.body.add_comment("Append msg to TX FIFO")
<<<<<<< HEAD
            func.body.add_line("transmit_func(&tx_msg);")
=======
            func.body.add_line("hw_can_pushTxMsgToQueue(&tx_msg);")
>>>>>>> 8507a611 (refactor can driver)
            func.body.end_if()

            # If aperiodic, make function public. Otherwise it can be static.
            if not msg.is_periodic():
                public_funcs.append(func)
            else:
                func.qualifier = "static"
                static_funcs.append(func)

        return public_funcs, static_funcs

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

        cw.add_line()
        cw.add_header_comment("Structs")
        cw.add_line()

        can_msg_struct = CStruct(
            name=CTypesConfig.CAN_MSG_STRUCT,
            comment="Standard CAN message type.",
        )
        can_msg_struct.add_member(CVar(name="std_id", type="uint32_t"))
        can_msg_struct.add_member(CVar(name="dlc", type="uint32_t"))
        can_msg_struct.add_member(CVar(name="data[8]", type="uint8_t"))
        cw.add_struct(can_msg_struct)

        cw.add_line()
        cw.add_header_comment("Enums")
        cw.add_line()

        can_mode_enum = CEnum("CanMode")
        for i, mode in enumerate(self._db.bus_config.modes):
            can_mode_enum.add_value(
                CVar(name=f"CAN_MODE_{mode.upper()}", value=f"1 << {i}")
            )

        cw.add_enum(can_mode_enum)

        # Add public function prototypes
        cw.add_line()
        cw.add_header_comment("Function Prototypes")
        cw.add_line()

        for func in self._public_functions:
            cw.add_function_prototype(func)
            cw.add_line()

        return str(cw)

    def source(self):
        cw = CWriter()
        cw.add_preamble()
        cw.add_line()

        # Add includes
        cw.add_header_comment("Includes")
        cw.add_line()
        cw.add_include('"Io_CanTx.h"')
        cw.add_include("<string.h>")
        cw.add_include('"App_CanTx.h"')
        cw.add_include('"App_CanUtils.h"')

        cw.add_line()
        cw.add_line("#ifndef THREAD_SAFE_CAN_PACKING")
        cw.add_include("<FreeRTOS.h>")
        cw.add_include("<portmacro.h>")
        cw.add_line("#endif")
        cw.add_line()

        # Static variables
        cw.add_header_comment("Static Variables")
        cw.add_line()
        cw.add_line("static uint32_t can_mode;")
        cw.add_line("static void (*transmit_func)(const CanMsg* tx_msg);")
        cw.add_line()

        # Add static function definitions
        cw.add_header_comment("Static Function Definitions")
        cw.add_line()

        for func in self._static_functions:
            cw.add_function_definition(func)
            cw.add_line()

        # Add public function definitions
        cw.add_header_comment("Public Function Definitions")
        cw.add_line()

        for func in self._public_functions:
            cw.add_function_definition(func)
            cw.add_line()

        return str(cw)
