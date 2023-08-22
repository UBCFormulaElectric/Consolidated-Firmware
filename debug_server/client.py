from Pyro5.api import Proxy
import argparse
from server import SERVER_PORT, SERVER_NAME


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", type=str, required=True)
    parser.add_argument("--config", type=str, required=False)
    parser.add_argument("--shutdown", action="store_true", required=False)
    args = parser.parse_args()

    debug_server = Proxy(uri=f"PYRO:{SERVER_NAME}@{args.ip}:{SERVER_PORT}")

    if args.shutdown:
        # Close OpenOCD server connections.
        debug_server.shutdown()
    elif args.config is not None:
        # Open OpenOCD server connections, and confirm all are alive.
        debug_server.shutdown()
        debug_server.run(args.config)
