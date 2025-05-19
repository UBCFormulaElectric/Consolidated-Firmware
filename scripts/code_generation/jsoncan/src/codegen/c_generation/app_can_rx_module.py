import jinja2 as j2

from .routing import CanRxConfig
from ...can_database import CanDatabase
from .utils import load_template
from .cmodule import CModule


class AppCanRxModule(CModule):
    def __init__(self, db: CanDatabase, node: str, rx_config: CanRxConfig):
        self._db = db
        self._node = node
        self._rx_msgs = [
            self._db.msgs[msg_name] for msg_name in rx_config.get_all_rx_msgs_names()
        ]

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
        return template.render(messages=self._rx_msgs, node=self._node)
