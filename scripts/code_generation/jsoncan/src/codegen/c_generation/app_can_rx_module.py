import jinja2 as j2

from ...can_database import CanDatabase
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
            self._alert_boards_messages[board] = db.get_board_tx_messages(board=board)

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
