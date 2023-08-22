set -e

# Set `HOST_IP` environment variable, which is required to connect to the debug server.
# In Unix-like systems, the system's IP address is shown after the "nameserver" field
# in /etc/resolve.conf.
export HOST_IP=$(cat /etc/resolv.conf | grep '^nameserver' | awk '{print $2}')

# Start Docker service.
docker compose up -d
