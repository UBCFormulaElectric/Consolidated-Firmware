import jinja2 as j2

from ...can_database import *
from .utils import load_template


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
            rx_messages=self._db.rx_msgs[self._node],
            # TODO come back to this, as all busses should generate a io_canRx_filterMessageId function
            messages=self._db.msgs,  # this map might need to be precomputed
            # db=self._db,
        )
