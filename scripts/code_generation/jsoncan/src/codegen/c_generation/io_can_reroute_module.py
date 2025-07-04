from collections import defaultdict
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

    def __init__(
        self, db: CanDatabase, node: str, node_reroutes: Set[CanForward] | None
    ):
        self._node = node
        self._db = db
        self._node_reroutes = node_reroutes
        self._node_bus_names = db.nodes[node].bus_names

    def header_template(self):
        if self._node_reroutes is None:
            return ""
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("io_canReroute.h.j2"))
        return template.render(node_bus_names=self._node_bus_names)

    def source_template(self):
        if self._node_reroutes is None:
            return ""
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("io_canReroute.c.j2"))
        m = {bus_name: defaultdict(list) for bus_name in self._node_bus_names}
        for reroute in self._node_reroutes:
            assert reroute.forwarder_name == self._node
            m[reroute.from_bus_name][reroute.message_name].append(reroute.to_bus_name)


        @dataclass
        class RouteOutMessage:
            msg: CanMessage
            to_busses: list[str]

        @dataclass
        class BusReroutes:
            bus_name: str
            reroutes: list[RouteOutMessage]

        bus_name_and_reroutes = [
            BusReroutes(bus_name, [
                RouteOutMessage(self._db.msgs[msg_name], to_busses) for msg_name, to_busses in m[bus_name].items()
            ]
          ) for bus_name in m]
        return template.render(
            node_bus_names=self._node_bus_names,
            bus_name_and_reroutes=bus_name_and_reroutes
        )
