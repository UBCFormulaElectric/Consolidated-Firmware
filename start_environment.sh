export HOST_IP=$(cat /etc/resolv.conf | grep '^nameserver' | awk '{print $2}')
docker compose up -d
