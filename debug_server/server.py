from Pyro5.api import expose, behavior, serve
from openocd import Interface, Target, OpenOcdServer


# TODO: Improve logging. Print raw OpenOCD output to terminal, since sometimes fails silently. Handle common logs.

SERVER_NAME = "debug_server"
SERVER_PORT = 9000
SERVER_CONFIGS = {
    "BMS": [
        {
            "name": "BMS",
            "target": Target.STM32F4X,
            "interface": Interface.JLINK_SWD,
            "port": 3333,
        }
    ],
    "DCM": [
        {
            "name": "DCM",
            "target": Target.STM32F4X,
            "interface": Interface.JLINK_SWD,
            "port": 3333,
        }
    ],
    "FSM": [
        {
            "name": "FSM",
            "target": Target.STM32F4X,
            "interface": Interface.JLINK_SWD,
            "port": 3333,
        }
    ],
    "PDM": [
        {
            "name": "DIM",
            "target": Target.STM32F4X,
            "interface": Interface.JLINK_SWD,
            "port": 3333,
        }
    ],
    "DIM": [
        {
            "name": "DIM",
            "target": Target.STM32F4X,
            "interface": Interface.JLINK_SWD,
            "port": 3333,
        }
    ],
    "GSM": [
        {
            "name": "DIM",
            "target": Target.STM32F3X,
            "interface": Interface.JLINK_SWD,
            "port": 3333,
        }
    ],
}


@behavior(instance_mode="single")
class DebugServer(object):
    def __init__(self):
        self.servers = []

    @expose
    def run(self, config: str):
        # Ensure all servers are closed before opening a new config.
        if not self._check_closed():
            raise RuntimeError(
                "Attempted to open a new debug server config, while one was already running."
            )

        for server_config in SERVER_CONFIGS[config]:
            server = OpenOcdServer(
                name=server_config["name"],
                interface=server_config["interface"],
                target=server_config["target"],
                port=server_config["port"],
            )
            self.servers.append(server)

        if not self._check_config(config):
            raise RuntimeError(
                f"Servers for config {config} were not started successfully."
            )

    @expose
    def shutdown(self):
        for server in self.servers:
            server.stop()

        self.servers = []

    def _check_config(self, config: str) -> bool:
        # If a requested server config was not created, return false.
        for server_config in SERVER_CONFIGS[config]:
            if server_config["name"] not in {server.name for server in self.servers}:
                return False

        # If a server has died unexpectedly, return false.
        for server in self.servers:
            if not server.alive():
                return False

        # If the number of servers running doesn't match the number requested, return false.
        if len(self.servers) != len(SERVER_CONFIGS[config]):
            return False

        return True

    def _check_closed(self):
        return len(self.servers) == 0


if __name__ == "__main__":
    serve(
        objects={DebugServer: SERVER_NAME},
        host="0.0.0.0",
        port=SERVER_PORT,
        use_ns=False,
    )
