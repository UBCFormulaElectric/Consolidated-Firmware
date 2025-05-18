import jinja2 as j2

from ...can_database import All, CanDatabase
from .cmodule import CModule
from .routing import CanRxConfig
from .utils import load_template


class AppCanRxModule(CModule):
    def __init__(self, db: CanDatabase, node: str, rx_config: CanRxConfig):
        self._db = db
        self._node = node
        self._rx_msgs = [
            self._db.msgs[msg_name] for msg_name in rx_config.get_all_rx_msgs_names()
        ]
        self._alert_boards_messages = {}
        alerts_boards = db.alerts.keys()

        for board in alerts_boards:
            rx_msgs = []
            if isinstance(db.nodes[node].rx_msgs_names, All): # if all then talk all the tx message for that board
                rx_msgs = db.get_board_tx_messages(board)
            else:
                for msg in db.get_board_tx_messages(board): # else need to filter the message that is rx by the board
                    if msg.name in db.nodes[node].rx_msgs_names:
                        rx_msgs.append(msg)
            self._alert_boards_messages[board] = rx_msgs

    def header_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canRx.h.j2"))
        return template.render(messages=self._rx_msgs, node=self._node)

    def source_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canRx.c.j2"))
        return template.render(
            messages=self._rx_msgs,
            node=self._node,
            alert_boards_messages=self._alert_boards_messages,
        )
