import jinja2 as j2
from typing import List

from .utils import load_template
from ...can_database import CanDatabase
from .cmodule import CModule
from .routing import CanForward


# only VC run this module
class IoCanRerouteModule(CModule):
    def __init__(self, db: CanDatabase, node: str, reroutes: List[CanForward] | None):
        self._db = db
        self._node = node
        self._reroutes = reroutes

    def header_template(self):
        if self._reroutes is None:
            return ""
        j2_env = j2.Environment(loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"])
        template = j2_env.from_string(load_template("io_canReroute.h.j2"))
        return template.render(node=(self._db.nodes[self._node]))

    def source_template(self):
        if self._reroutes is None:
            return ""

        node_obj = self._db.nodes[self._node]

        # dict from bus name to list of messages
        reroutes = {bus: [] for bus in node_obj.bus_names}
        for msg in self._reroutes:
            reroutes[msg.from_bus].append(msg)
        j2_env = j2.Environment(loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"])
        template = j2_env.from_string(load_template("io_canReroute.c.j2"))
        return template.render(
            reroutes=reroutes,
            messages=self._db.msgs,
            node=node_obj,
            nodes=self._db.nodes,
        )
