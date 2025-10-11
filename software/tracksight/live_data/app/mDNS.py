import socket
import time
from zeroconf import ServiceInfo, Zeroconf

from logger import logger

started = False


def register_mdns_service(ip: str, service_name: str):

    # Get the local IP address
    ip_address = socket.inet_aton(ip)
    domain_name = f'{service_name}.local.'

    # Define the service information
    info = ServiceInfo(
        type_="_http._tcp.local.",
        name=f'_{service_name}._http._tcp.local.',
        addresses=[ip_address],
        port=8080,  # actually doesn't matter, you get the ip via domain name anyways
        server=f'{service_name}.local.',
    )
    # Register the service
    zeroconf = Zeroconf()
    # if registered, it will update the service
    zeroconf.unregister_all_services()

    # Register the service with the Zeroconf instance

    zeroconf.register_service(info)
    logger.info(f"Map domain name {domain_name} to {ip}")


if __name__ == "__main__":
    register_mdns_service(ip="206.87.75.184", service_name="testt_services")
    while True:
        time.sleep(1)
