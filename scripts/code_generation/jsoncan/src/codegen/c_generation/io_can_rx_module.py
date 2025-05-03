import jinja2 as j2

from .c_writer import *
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
            rx_messages=self._db.rx_msgs[self._node],
            node=self._node,
            messages=self._db.msgs,
        )

    def source_template(self):
        template = load_template("io_canRx.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        for a, b in self._db.rx_msgs[self._node].messages.items():
            print(a)
            print(b)

        template = j2_env.from_string(template)
        return template.render(
            rx_messages=self._db.rx_msgs[self._node],
            node=self._node,
            messages=self._db.msgs,
            db=self._db
        )
