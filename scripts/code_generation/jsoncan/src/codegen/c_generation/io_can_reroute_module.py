from dataclasses import dataclass

import jinja2 as j2
from typing import List, Set

from .utils import load_template
from ...can_database import CanDatabase, CanMessage
from .cmodule import CModule
from .routing import CanForward


# only VC run this module
class IoCanRerouteModule(CModule):
    @dataclass()
    class ReRouteInstance:
        to_bus_name: str
        message: CanMessage

    def __init__(self, db: CanDatabase, node: str, node_reroutes: Set[CanForward] | None):
        self._db = db
        self._node_reroutes = node_reroutes
        self._node_bus_names = db.nodes[node].bus_names

    def header_template(self):
        if self._node_reroutes is None:
            return ""
        j2_env = j2.Environment(loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"])
        template = j2_env.from_string(load_template("io_canReroute.h.j2"))
        return template.render(node_bus_names=self._node_bus_names)

    def source_template(self):
        if self._node_reroutes is None:
            return ""
        j2_env = j2.Environment(loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"])
        template = j2_env.from_string(load_template("io_canReroute.c.j2"))
        from_bus_to_reroutes = {bus_name: [] for bus_name in self._node_bus_names}
        for reroute in self._node_reroutes:
            from_bus_to_reroutes[reroute.from_bus_name].append(
                IoCanRerouteModule.ReRouteInstance(
                    to_bus_name=reroute.to_bus_name,
                    message=self._db.msgs[reroute.message_name]
                )
            )
        return template.render(
            node_bus_names=self._node_bus_names,
            bus_name_and_reroutes=from_bus_to_reroutes.items(),
        )
