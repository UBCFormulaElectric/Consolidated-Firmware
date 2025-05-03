import jinja2 as j2

from .c_writer import *
from .utils import load_template

CAPTURE_LENGTH_MACRO = "{name}_{type}_CYCLE_TIME"
NEXT_CAPTURE_TIME_VAR = "{name}_next_{type}_time_ms"


class AppCanDataCaptureModule(CModule):
    def __init__(self, db: CanDatabase):
        self._db = db

    def header_template(self):
        template = load_template("app_canDataCapture.h.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(messages=list(self._db.msgs.values()))

    def source_template(self):
        template = load_template("app_canDataCapture.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(messages=list(self._db.msgs.values()))
