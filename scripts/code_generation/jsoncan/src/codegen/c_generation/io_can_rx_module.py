import jinja2 as j2

from .routing import CanRxConfig
from ...can_database import CanDatabase
from .utils import load_template
from .cmodule import CModule


class IoCanRxModule(CModule):
    def __init__(self, db: CanDatabase, node: str, rx_config: CanRxConfig):
        self._db = db
        self._node = node
        self._node_bus_names = self._db.nodes[self._node].bus_names
        self._rx_config = rx_config

    def header_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("io_canRx.h.j2"))
        return template.render(node=self._node, node_bus_names=self._node_bus_names)

    def source_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("io_canRx.c.j2"))
        return template.render(
            node=self._node,
            node_bus_names=self._node_bus_names,
            rx_config=self._rx_config,
            msg_map=self._db.msgs,
        )
