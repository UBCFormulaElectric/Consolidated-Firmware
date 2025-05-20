import jinja2 as j2

from ...can_database import *
from .utils import load_template
from .cmodule import CModule
from typing import List, Tuple


class AppCanAlertsModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node_name = node
        self._node_name_and_alerts: List[Tuple[str, List[CanAlert]]] = [
            (k, v) for k, v in db.alerts.items()
        ]

    def source_template(self) -> str:
        if self._node_name not in self._db.alerts.keys():
            return ""
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canAlerts.c.j2"))

        return template.render(
            node_tx_alerts=self._db.alerts[self._node_name],  # List[CanAlert]
            node_name_and_alerts=self._node_name_and_alerts,
            node_name=self._node_name,  # metadata :)
            CanAlertType=CanAlertType,  # to access enums
        )

    def header_template(self) -> str:
        if self._node_name not in self._db.alerts.keys():
            return ""
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canAlerts.h.j2"))

        return template.render(
            node_name=self._node_name,  # just for metadata purposes
            node_name_and_alerts=self._node_name_and_alerts,
            # List[CanAlert] x 3
            tx_faults=[
                alert
                for alert in self._db.alerts[self._node_name]
                if alert.alert_type == CanAlertType.FAULT
            ],
            tx_warnings=[
                alert
                for alert in self._db.alerts[self._node_name]
                if alert.alert_type == CanAlertType.WARNING
            ],
            tx_info=[
                alert
                for alert in self._db.alerts[self._node_name]
                if alert.alert_type == CanAlertType.INFO
            ],
        )
