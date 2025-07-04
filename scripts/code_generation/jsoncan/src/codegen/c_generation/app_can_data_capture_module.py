import jinja2 as j2

from ...can_database import *
from .utils import load_template
from .cmodule import CModule


class AppCanDataCaptureModule(CModule):
    def __init__(self, db: CanDatabase):
        self._messages = list(db.msgs.values())

    def header_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canDataCapture.h.j2"))
        return template.render(messages=self._messages)

    def source_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canDataCapture.c.j2"))
        return template.render(messages=self._messages)
