from ...can_database import *
from ...utils import *
from .c_config import *
from .c_writer import *

NUM_ALERTS = "NUM_{node}_{alert_type}S"
BOARD_HAS_ALERT_FUNC_NAME = "app_canAlerts_BoardHas{alert_type}"
ANY_ALERT_FUNC_NAME = "app_canAlerts_AnyBoardHas{alert_type}"
ALERT_BOARD_ENUM_NAME = "{node}_ALERT_BOARD"
GET_BOARD_FAULT_CODE = "app_canAlerts_{alert_type}Info"


class AppCanAlertsModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node
        self._functions = self._public_functions()

    def _set_alert_funcs(self, alert_type: CanAlertType):
        funcs = []
        for alert in self._db.node_alerts(self._node, alert_type):
            func = CFunc(
                CFuncsConfig.APP_ALERTS_SET.format(alert=alert),
                "void",
                args=[
                    CVar("set_alert", "bool"),
                ],
                comment=f"Set or clear an alert for this board.",
            )
            func.body.add_comment("Increment alert counter.")
            func.body.start_if(
                f"set_alert && !{CFuncsConfig.APP_TX_GET_SIGNAL.format(signal=alert)}()"
            )
            func.body.add_line(
                f"{CFuncsConfig.APP_TX_SET_SIGNAL.format(signal=f'{alert}Count')}({CFuncsConfig.APP_TX_GET_SIGNAL.format(signal=f'{alert}Count')}() + 1);"
            )
            func.body.end_if()
            func.body.add_line()

            func.body.add_comment("Set alert.")
            func.body.add_line(
                f"{CFuncsConfig.APP_TX_SET_SIGNAL.format(signal=alert)}(set_alert);"
            )
            funcs.append(func)

        return funcs

    def _get_alert_funcs(self, alert_type: CanAlertType):
        funcs = []
        for alert in self._db.node_alerts(self._node, alert_type):
            func = CFunc(
                CFuncsConfig.APP_ALERTS_GET.format(alert=alert),
                "bool",
                args=[],
                comment=f"Return whether or not a specific alert transmitted by this board is set.",
            )
            func.body.add_line(
                f"return {CFuncsConfig.APP_TX_GET_SIGNAL.format(signal=alert)}();"
            )
            funcs.append(func)

        return funcs

    def _board_has_alert_func(self, alert_type: CanAlertType, comment: str):
        has_alert = CFunc(
            BOARD_HAS_ALERT_FUNC_NAME.format(alert_type=alert_type),
            "bool",
            args=[
                CVar("board", CTypesConfig.CAN_ALERT_BOARD_ENUM),
            ],
            comment=f"Return whether or not a board has set a {comment}.",
        )
        has_alert.body.start_switch("board")

        nodes_with_alerts = [
            node for node in self._db.nodes if self._db.node_has_alert(node, alert_type)
        ]
        for node in nodes_with_alerts:
            has_alert.body.add_switch_case(
                ALERT_BOARD_ENUM_NAME.format(node=node.upper())
            )
            has_alert.body.start_switch_case()

            for alert in self._db.node_alerts_with_rx_check(
                node, self._node, alert_type
            ):
                if node == self._node:
                    has_alert.body.start_if(
                        f"{CFuncsConfig.APP_TX_GET_SIGNAL.format(signal=alert)}()"
                    )
                else:
                    has_alert.body.start_if(
                        f"{CFuncsConfig.APP_RX_GET_SIGNAL.format(signal=alert)}()"
                    )

                has_alert.body.add_line("return true;")
                has_alert.body.end_if()
                has_alert.body.add_line()

            has_alert.body.add_switch_break()

        has_alert.body.add_switch_default()
        has_alert.body.start_switch_case()
        has_alert.body.add_comment("Do nothing")
        has_alert.body.add_switch_break()
        has_alert.body.end_switch()

        has_alert.body.add_line()
        has_alert.body.add_line("return false;")
        return has_alert

    def _get_board_alert_code(self, alert_type: CanAlertType, comment: str):
        get_alert = CFunc(
            GET_BOARD_FAULT_CODE.format(alert_type=alert_type),
            "uint8_t",
            args=[
                CVar("*alert_array",CTypesConfig.CAN_ALERT_INFO)
            ],
            comment=f"Return whether or not a board has set a {comment}.",
        )
        get_alert.body.add_line("uint8_t element_num = 0;")
        get_alert.body.add_line()
        nodes_with_alerts = [
            node for node in self._db.nodes if self._db.node_has_alert(node, alert_type)
        ]
        for node in nodes_with_alerts:       

            for alert in self._db.node_alerts_with_rx_check(
                node, self._node, alert_type
            ):
                
                item = self._db.node_name_description(node, alert_type = alert_type)
                
                description = item[alert]

                if node == self._node:
                    get_alert.body.start_if(
                        f"{CFuncsConfig.APP_TX_GET_SIGNAL.format(signal=alert)}()"
                    )
                else:
                    get_alert.body.start_if(
                        f"{CFuncsConfig.APP_RX_GET_SIGNAL.format(signal=alert)}()"
                    )
                    
                get_alert.body.add_line(f'alert_array[element_num].name = "{alert}";')
                get_alert.body.add_line(f'alert_array[element_num].description = "{description}";')
                get_alert.body.add_line("element_num++;")

                get_alert.body.end_if()
                get_alert.body.add_line()

        get_alert.body.add_line("return element_num;")

        get_alert.body.add_line()
        return get_alert

    def _any_alert_set_func(self, alert_type: CanAlertType, comment: str):
        has_alert = CFunc(
            ANY_ALERT_FUNC_NAME.format(alert_type=alert_type),
            "bool",
            args=[],
            comment=f"Return whether or not any board has set a {comment}.",
        )

        nodes_with_alerts = [
            node for node in self._db.nodes if self._db.node_has_alert(node, alert_type)
        ]
        for node in nodes_with_alerts:
            has_alert.body.start_if(
                f"{BOARD_HAS_ALERT_FUNC_NAME.format(alert_type=alert_type)}({ALERT_BOARD_ENUM_NAME.format(node=node.upper())})"
            )
            has_alert.body.add_line("return true;")
            has_alert.body.end_if()
            has_alert.body.add_line()

        has_alert.body.add_line("return false;")
        return has_alert

    def _public_functions(self) -> List[CFunc]:
        funcs = []

        # Alert setters
        funcs.extend(self._set_alert_funcs(CanAlertType.WARNING))
        funcs.extend(self._set_alert_funcs(CanAlertType.FAULT))
                
        # Alert getters
        funcs.extend(self._get_alert_funcs(CanAlertType.WARNING))
        funcs.extend(self._get_alert_funcs(CanAlertType.FAULT))

        # Board-specific alert set checkers
        funcs.append(self._board_has_alert_func(CanAlertType.WARNING, "warning"))
        funcs.append(self._board_has_alert_func(CanAlertType.FAULT, "fault"))

        # All board alert set checkers
        funcs.append(self._any_alert_set_func(CanAlertType.WARNING, "warning"))
        funcs.append(self._any_alert_set_func(CanAlertType.FAULT, "fault"))
        
        # Fault and Warning code getters
        funcs.append(self._get_board_alert_code (CanAlertType.WARNING, "warning"))
        funcs.append(self._get_board_alert_code(CanAlertType.FAULT, "fault"))

        return funcs
    

    def header(self):
        cw = CWriter()
        cw.add_preamble()
        cw.add_line()
        cw.add_line("#pragma once")

        # Add includes
        cw.add_line()
        cw.add_header_comment("Includes")
        cw.add_line()
        cw.add_include("<stdbool.h>")
        cw.add_include('"app_canUtils.h"')

        # Add enums
        cw.add_line()
        cw.add_header_comment("Enums")
        cw.add_line()

        # Enum of CAN boards
        boards_enum = CEnum(CTypesConfig.CAN_ALERT_BOARD_ENUM)
        nodes_with_alerts = [
            node
            for node in self._db.nodes
            if any(
                [
                    self._db.node_has_alert(node, alert_type)
                    for alert_type in CanAlertType
                ]
            )
        ]
        for i, node in enumerate(nodes_with_alerts):
            boards_enum.add_value(
                CVar(ALERT_BOARD_ENUM_NAME.format(node=node.upper()), value=i)
            )
        cw.add_enum(boards_enum)   
        cw.add_line()
                    
        fault_warining_struct = CStruct(CTypesConfig.CAN_ALERT_INFO)
        fault_warining_struct.add_member(CVar("description","char*"))
        fault_warining_struct.add_member(CVar("name", "char*"))
        
        cw.add_struct(fault_warining_struct)
        
        
        # Add function prototypes
        cw.add_line()
        cw.add_header_comment("Function Prototypes")
        cw.add_line()

        for func in self._functions:
            cw.add_function_prototype(func)
            cw.add_line()

        return str(cw)

    def source(self):
        cw = CWriter()
        cw.add_preamble()
        cw.add_line()

        # Add includes
        cw.add_line()
        cw.add_header_comment("Includes")
        cw.add_line()
        cw.add_include('"app_canAlerts.h"')
        cw.add_include('"app_canTx.h"')
        cw.add_include('"app_canRx.h"')
        
        # Add function definitions
        cw.add_line()
        cw.add_header_comment("Function Definitions")
        cw.add_line()
        cw.add_line()

        for func in self._functions:
            cw.add_function_definition(func)
            cw.add_line()

        return str(cw)
