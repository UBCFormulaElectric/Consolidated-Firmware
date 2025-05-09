import jinja2 as j2
from typing import List

from .utils import load_template
from ...can_database import CanDatabase
from .cmodule import CModule
from .routing import CanForward


# only VC run this module
class IoCanRerouteModule(CModule):
    def __init__(self, db: CanDatabase, node: str, reroutes: List[CanForward]):
        self._db = db
        self._node = node
        self._reroutes = reroutes

    def header_template(self):
        if len(self._reroutes) == 0:
            return ""
        template = load_template("io_canReroute.h.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        node_obj = self._db.nodes[self._node]
        return template.render(node=node_obj)

    def source_template(self):
        if len(self._reroutes) == 0:
            return ""

        node_obj = self._db.nodes[self._node]

        # dict from bus name to list of messages
        reroutes = {bus: [] for bus in node_obj.bus_names}
        for msg in self._reroutes:
            reroutes[msg.from_bus].append(msg)

        template = load_template("io_canReroute.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            reroutes=reroutes,
            messages=self._db.msgs,
            node=node_obj,
            nodes=self._db.nodes,
        )
