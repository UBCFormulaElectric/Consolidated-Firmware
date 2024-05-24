from ...can_database import *
from .c_writer import *
from .c_config import *
from ...utils import *

CAPTURE_LENGTH_MACRO = "{name}_{type}_CYCLE_TIME"
NEXT_CAPTURE_TIME_VAR = "{name}_next_{type}_time_ms"


class AppCanDataCaptureModule(CModule):
    def __init__(self, db: CanDatabase):
        self._db = db
        self._init = self._init_func()
        self._log_func = self._data_capture_func(
            name="app_dataCapture_needsLog",
            cycle_time_var="log_cycle_time",
            type="log",
        )
        self._telem_func = self._data_capture_func(
            name="app_dataCapture_needsTelem",
            cycle_time_var="telem_cycle_time",
            type="telem",
        )

    def _init_func(self) -> CFunc:
        func = CFunc(name="app_canDataCapture_init", return_type="void", args=[])

        # Reset log capture times.
        func.body.add_comment("Reset next log capture times.")
        for msg in self._db.msgs.values():
            if msg.log_cycle_time and msg.log_cycle_time != msg.cycle_time:
                func.body.add_line(
                    f"{NEXT_CAPTURE_TIME_VAR.format(name=msg.name, type='log')} = 0U;"
                )

        func.body.add_line()

        # Reset telem capture times.
        func.body.add_comment("Reset next telem capture times.")
        for msg in self._db.msgs.values():
            if msg.telem_cycle_time and msg.telem_cycle_time != msg.cycle_time:
                func.body.add_line(
                    f"{NEXT_CAPTURE_TIME_VAR.format(name=msg.name, type='telem')} = 0U;"
                )

        return func

    def _data_capture_func(self, name: str, cycle_time_var: str, type: str) -> CFunc:
        func = CFunc(
            name=name,
            return_type="bool",
            args=[
                CVar(name="msg_id", type="uint16_t"),
                CVar(name="time_ms", type="uint32_t"),
            ],
        )
        func.body.start_switch(value="msg_id")

        for msg in self._db.msgs.values():
            cycle_time = getattr(msg, cycle_time_var)

            func.body.add_switch_case(case=CMacrosConfig.id(msg.name))
            func.body.start_switch_case()

            if cycle_time is None:
                # If None, we don't capture this message.
                func.body.add_comment("This message is not captured, return false.")
                func.body.add_line("return false;")
            elif cycle_time == msg.cycle_time:
                # This message is captured at its normal cycle time, assume we need to capture it.
                func.body.add_comment(
                    "This message is captured at its normal cycle time. So, just assume the messages are received at that rate and should be captured."
                )
                func.body.add_line("return true;")
            else:
                # Need to keep track via a variable if cycle times don't match.
                capture_time_var = NEXT_CAPTURE_TIME_VAR.format(
                    name=msg.name, type=type
                )

                func.body.start_if(f"time_ms > {capture_time_var}")
                func.body.add_comment("Capture msg and increment variable.")
                func.body.add_line(
                    f"{capture_time_var} = time_ms + {CAPTURE_LENGTH_MACRO.format(name=pascal_to_screaming_snake_case(msg.name), type=type.upper())};"
                )
                func.body.add_line("return true;")

                func.body.start_else()
                func.body.add_comment("Not ready to capture yet.")
                func.body.add_line("return false;")
                func.body.end_if()

            func.body.add_switch_break()

        func.body.end_switch()

        return func

    def header(self) -> str:
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

        # Add function prototypes
        cw.add_line()
        cw.add_header_comment("Function Prototypes")
        cw.add_line()
        cw.add_line()

        cw.add_function_prototype(self._init)
        cw.add_line()
        cw.add_function_prototype(self._log_func)
        cw.add_line()
        cw.add_function_prototype(self._telem_func)
        cw.add_line()

        return str(cw)

    def source(self) -> str:
        cw = CWriter()
        cw.add_preamble()
        cw.add_line()

        # Add includes
        cw.add_line()
        cw.add_header_comment("Includes")
        cw.add_line()
        cw.add_include('"app_canDataCapture.h"')
        cw.add_include('"app_canUtils.h"')

        # Add macros
        cw.add_line()
        cw.add_header_comment("Macros")
        cw.add_line()
        for msg in self._db.msgs.values():
            if msg.log_cycle_time and msg.log_cycle_time != msg.cycle_time:
                cw.add_macro(
                    name=CAPTURE_LENGTH_MACRO.format(
                        name=pascal_to_screaming_snake_case(msg.name), type="LOG"
                    ),
                    value=msg.log_cycle_time,
                )

        for msg in self._db.msgs.values():
            if msg.telem_cycle_time and msg.telem_cycle_time != msg.cycle_time:
                cw.add_macro(
                    name=CAPTURE_LENGTH_MACRO.format(
                        name=pascal_to_screaming_snake_case(msg.name), type="TELEM"
                    ),
                    value=msg.log_cycle_time,
                )

        # Private vars
        cw.add_line()
        cw.add_header_comment("Private Variables")
        cw.add_line()

        # Add capture time vars for log.
        for msg in self._db.msgs.values():
            if msg.log_cycle_time and msg.log_cycle_time != msg.cycle_time:
                cw.add_var_declaration(
                    var=CVar(
                        name=NEXT_CAPTURE_TIME_VAR.format(name=msg.name, type="log"),
                        type="uint32_t",
                    ),
                    qualifier="static",
                )
        cw.add_line()

        # Add capture time vars for telemetry.
        for msg in self._db.msgs.values():
            if msg.telem_cycle_time and msg.telem_cycle_time != msg.cycle_time:
                cw.add_var_declaration(
                    var=CVar(
                        name=NEXT_CAPTURE_TIME_VAR.format(name=msg.name, type="telem"),
                        type="uint32_t",
                    ),
                    qualifier="static",
                )

        # Add function definitions
        cw.add_line()
        cw.add_header_comment("Function Definitions")
        cw.add_line()
        cw.add_line()

        cw.add_function_definition(self._init)
        cw.add_line()
        cw.add_function_definition(self._log_func)
        cw.add_line()
        cw.add_function_definition(self._telem_func)
        cw.add_line()

        return str(cw)
