import socket
import time
from xml import dom

from zeroconf import ServiceInfo, Zeroconf

# Define the service type and name
service_type = "_http._tcp.local."
service_name = "_telemserver._http._tcp.local."
domain_name = "telemserver.local."
# Get the local IP address
services = "192.168.1.126"
ip_address = socket.inet_aton(services)
# Define the service information
info = ServiceInfo(
    type_=service_type,
    name=service_name,
    addresses=[ip_address],
    port=8080,
    server=domain_name,
)
# Register the service
zeroconf = Zeroconf()


def register_mdns_service():

    zeroconf.register_service(info)
    print(
        f"Registering service {service_name} on {services} access by domain name {domain_name}")


