import jinja2 as j2

from ...can_database import CanDatabase
from .utils import load_template
from .cmodule import CModule

from .routing import CanTxConfig


class IoCanTxModule(CModule):
    def __init__(self, db: CanDatabase, node: str, tx_config: CanTxConfig):
        self._messages = [
            db.msgs[msg_name] for msg_name in tx_config.get_all_msg_names()
        ]
        self._node_bus_names = db.nodes[node].bus_names
        self._bus_config = db.buses
        self._tx_config = tx_config
        self.fd = db.nodes[node].fd

    def header_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("io_canTx.h.j2"))
        return template.render(
            bus_config=self._bus_config,
            node_bus_names=self._node_bus_names,
            messages=self._messages,
            fd=self.fd,
        )

    def source_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("io_canTx.c.j2"))
        return template.render(
            bus_config=self._bus_config,
            node_bus_names=self._node_bus_names,
            busses_for_msg=self._tx_config.get_busses_for_msg,
            messages=self._messages,
        )
