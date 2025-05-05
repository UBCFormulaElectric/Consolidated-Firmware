import jinja2 as j2

from .cmodule import CModule
from ...can_database import CanDatabase
from .utils import load_template


class AppCanTxModule(CModule):
    def __init__(self, db: CanDatabase, node_name: str):
        self._db = db
        self._node_name = node_name

    def header_template(self):
        template = load_template("app_canTx.h.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            messages=self._db.tx_msgs_for_node(self._node_name), node_name=self._node_name
        )

    def source_template(self):
        template = load_template("app_canTx.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            messages=self._db.tx_msgs_for_node(self._node_name), node=self._node_name
        )
