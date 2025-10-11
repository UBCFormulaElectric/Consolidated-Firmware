if [ -z "$1" ]; then
    echo "Missing YAML configuration file for Docker compose"
    exit 1
fi

if ! [ -f "common.env" ]; then
    echo "common.env does not exist! Create one with the template."
    exit 1
fi

echo "Running Docker compose with $1"
docker compose --env-file common.env -f $1 up --build