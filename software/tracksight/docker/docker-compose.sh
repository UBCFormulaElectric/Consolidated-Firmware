if [ -z "$1" ]; then
    echo "Missing YAML configuration file for Docker compose, pick one from ./configs or configure your own."
    exit 1
fi

if ! [ -f "envs/common.env" ]; then
    echo "common.env does not exist! Create one with the template in ./envs."
    exit 1
fi

echo "Running Docker compose with $1"
docker compose --env-file envs/common.env -f $1 up --build