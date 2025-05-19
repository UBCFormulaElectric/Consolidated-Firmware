import jinja2 as j2

from .cmodule import CModule
from ...can_database import CanDatabase
from .utils import load_template
from .routing import CanTxConfig


class AppCanTxModule(CModule):
    def __init__(self, db: CanDatabase, tx_config: CanTxConfig):
        self._tx_messages = [
            db.msgs[msg_name] for msg_name in tx_config.get_all_msg_names()
        ]

    def header_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canTx.h.j2"))
        return template.render(tx_messages=self._tx_messages)

    def source_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canTx.c.j2"))
        return template.render(tx_messages=self._tx_messages)
