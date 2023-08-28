import rpyc
from typing import Any
from openocd import OpenOcdServer, Target, Interface


SERVICE_PORT = 9000
CONFIGS = {
    "DIM": [
        {
            "name": "DIM",
            "target": Target.STM32F4X,
            "interface": Interface.JLINK_SWD,
            "port": 3333,
        }
    ]
}


servers = []


@rpyc.service
class DebugService(rpyc.Service):
    def __init__(self, *args: Any, **kwargs: Any):
        super().__init__(*args, **kwargs)
        self.num_connections = 0

    def on_connect(self, conn: rpyc.Connection):
        self.num_connections += 1

        if self.num_connections > 1:
            # Only allow a single connection at once.
            raise RuntimeError("")

    def on_disconnect(self, conn: rpyc.Connection):
        self.num_connections -= 1

    @rpyc.exposed
    def open(self, config: str):
        global servers

        for server_config in CONFIGS[config]:
            server = OpenOcdServer(**server_config)
            servers.append(server)

    @rpyc.exposed
    def check(self) -> bool:
        global servers

        if len(servers) == 0:
            return False

        for server in servers:
            if not server.alive():
                return False

        return True

    @rpyc.exposed
    def close(self):
        global servers

        for server in servers:
            server.stop()

        servers = []


if __name__ == "__main__":
    print("Started debug service.")
    service = rpyc.utils.server.ThreadedServer(DebugService, port=SERVICE_PORT)
    service.start()
