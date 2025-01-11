import jinja2 as j2

from ...can_database import *
from .utils import load_template


# only VC run this module
class IoCanRerouteModule:
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node

    def header_template(self):
        if self._node != "VC":
            return ""
        
        template = load_template("io_canReroute.h.j2")
        j2_env = j2.Environment(loader=j2.BaseLoader, extensions=['jinja2.ext.loopcontrols'])
        template = j2_env.from_string(template)
        node_obj = self._db.nodes[self._node]
        return template.render(node=node_obj)
        
    def source_template(self):
        if self._node != "VC":
            return ""
        messages = []
        for reroute in self._db.reroute_msgs:
            messages.append(reroute.message)
        
        template = load_template("io_canReroute.c.j2")
        j2_env = j2.Environment(loader=j2.BaseLoader, extensions=['jinja2.ext.loopcontrols'])
        template = j2_env.from_string(template)
        node_obj = self._db.nodes[self._node]
        return template.render(messages= messages, node=node_obj)
