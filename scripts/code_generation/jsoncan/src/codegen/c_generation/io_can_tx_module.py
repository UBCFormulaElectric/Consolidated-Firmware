import jinja2 as j2

from .c_writer import *
from .utils import load_template


class IoCanTxModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node

    def header_template(self):
        node_obj = self._db.nodes[self._node]
        template = load_template("io_canTx.h.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            node=node_obj,
            messages=self._db.tx_msgs_for_node(self._node),
            bus_config=self._db.bus_config,
        )

    def source_template(self):
        node_obj = self._db.nodes[self._node]
        template = load_template("io_canTx.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            messages=self._db.tx_msgs_for_node(self._node),
            node=node_obj,
            bus_config=self._db.bus_config,
        )
