import jinja2 as j2

from ...can_database import *
from .utils import load_template

NUM_ALERTS = "NUM_{node}_{alert_type}S"
BOARD_HAS_ALERT_FUNC_NAME = "app_canAlerts_BoardHas{alert_type}"
ANY_ALERT_FUNC_NAME = "app_canAlerts_AnyBoardHas{alert_type}"
ALERT_BOARD_ENUM_NAME = "{node}_ALERT_BOARD"
GET_BOARD_FAULT_CODE = "app_canAlerts_{alert_type}Info"


class AppCanAlertsModule(CModule):
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node

    def get_board_node(self) -> list[str]:
        nodes_with_alerts: list[str] = [
            node
            for node in self._db.nodes.keys()
            if self._db.node_alerts(node, CanAlertType.FAULT)
               or self._db.node_alerts(node, CanAlertType.WARNING)
               or self._db.node_alerts(node, CanAlertType.INFO)
        ]
        return nodes_with_alerts

    def get_rx_warning(self):
        return self._db.node_rx_alerts(self._node)

    def get_rx_fault(self):
        return self._db.node_rx_alerts(self._node)

    def get_rx_info(self):
        return self._db.node_rx_alerts(self._node)

    def source_template(self):
        template = load_template("app_canAlerts.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)

        """Returns a dictionary containing a the alert names as the key and a description and as the item"""
        alert_description = {}
        for alerts in self._db.alerts.values():
            for alert in alerts:
                alert_description[alert.name] = (alert.id, alert.description)

        return template.render(
            alerts=self._db.alerts, # TODO probably a fat refactor under the JINJA
            CanAlertType=CanAlertType,
            alert_description=alert_description,
            boards=self.get_board_node(),
            node=self._node,
        )

    def header_template(self):
        template = load_template("app_canAlerts.h.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            tx_faults=self._db.node_alerts(self._node, CanAlertType.FAULT),
            tx_warnings=self._db.node_alerts(self._node, CanAlertType.WARNING),
            tx_info=self._db.node_alerts(self._node, CanAlertType.INFO),
            boards=self.get_board_node(),
            node=self._node,
        )
