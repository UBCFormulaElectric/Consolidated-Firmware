import jinja2 as j2

from ...can_database import CanDatabase
from .utils import load_template
from .cmodule import CModule


class IoCanTxModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node

    def header_template(self):
        template = load_template("io_canTx.h.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            bus_names=self._db.nodes[self._node].bus_names,  # for bus boilerplate
            messages=[self._db.msgs[msg_name] for msg_name in self._db.nodes[self._node].tx_config.list_msg_names()],
            bus_config=self._db.busses,  # template uses modes and default modes
        )

    def source_template(self):
        template = load_template("io_canTx.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            bus_names=self._db.nodes[self._node].bus_names,
            messages=[self._db.msgs[msg_name] for msg_name in self._db.nodes[self._node].tx_config.list_msg_names()],
            busses_for_msg=self._db.nodes[self._node].tx_config.get_busses_for_msg,
            bus_config=self._db.busses,
        )
