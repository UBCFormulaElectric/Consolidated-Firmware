#!/bin/sh
# Cleans up stale Docker containers and networks left behind by previous
# Tracksight compose runs (e.g. all.yml, db_only.yml, db_backend.yml).
#
# Symptoms this fixes:
#   - backend panics with "dns error ... failed to lookup address information"
#   - a service is "healthy" but not actually attached to its network
#   - influx port not published / unreachable on localhost:8086
#
# Usage: ./docker-clean.sh

cd "$(dirname "$0")" || exit 1

if ! [ -f "envs/common.env" ]; then
    echo "common.env does not exist! Create one with the template in ./envs."
    exit 1
fi

# Bring down every compose project defined under ./configs. Each config sets its
# own project `name:`, so `down` targets the correct project and removes its
# containers and default network. --remove-orphans clears renamed/removed services.
for config in configs/*.yml; do
    [ -f "$config" ] || continue
    echo "Tearing down $config ..."
    docker compose --env-file envs/common.env -f "$config" down --remove-orphans
done

# Catch any stray Tracksight containers that aren't tracked by the configs above
# (e.g. from an old project name or a half-removed run).
stray=$(docker ps -aq --filter "name=tracksight-")
if [ -n "$stray" ]; then
    echo "Removing stray tracksight containers ..."
    docker rm -f $stray
fi

# Remove dangling networks (including broken ones that leave services unattached).
echo "Pruning unused networks ..."
docker network prune -f
