import rpyc
import time
import argparse

SERVICE_PORT = 8080

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", type=str, required=True)
    parser.add_argument("--config", type=str, required=False)
    parser.add_argument("--close", action="store_true", required=False)
    args = parser.parse_args()

    conn = rpyc.connect(host=args.ip, port=SERVICE_PORT)
    debug_service = conn.root

    if args.close:
        # Close OpenOCD server connections.
        debug_service.close()
    elif args.config is not None:
        # Open OpenOCD server connections, and confirm they're valid.
        debug_service.open(args.config)
