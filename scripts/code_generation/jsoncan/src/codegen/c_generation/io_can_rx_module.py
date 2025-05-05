import jinja2 as j2

from ...can_database import CanDatabase
from .utils import load_template
from .cmodule import CModule


class IoCanRxModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node

    def header_template(self):
        template = load_template("io_canRx.h.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            node=self._node,
            node_bus_names=self._db.nodes[self._node].bus_names,
        )

    def source_template(self):
        template = load_template("io_canRx.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            node=self._node,
            node_bus_names=self._db.nodes[self._node].bus_names,
            rx_config=self._db.nodes[self._node].rx_config
            # rx_messages=self._db.rx_msgs[self._node],
            # messages=self._db.msgs,  # this map might need to be precomputed
            # rx_messages_for_node=self._db.rx_msgs_for_node(self._node),
        )
