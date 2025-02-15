import jinja2 as j2

from .c_writer import *
from .utils import load_template


class AppCanRxModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node

    def header_template(self):
        template = load_template("app_canRx.h.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader, extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            messages=self._db.rx_msgs_for_node(self._node), node=self._node
        )

    def source_template(self):
        template = load_template("app_canRx.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader, extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            messages=self._db.rx_msgs_for_node(self._node), node=self._node
        )
