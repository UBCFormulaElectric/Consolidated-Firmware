if [ -z "$1" ]; then
    echo "Missing YAML configuration file for Docker compose, pick one from ./configs or configure your own."
    exit 1
fi

echo "Downing Docker compose with $1"
docker compose -f $1 down "${@:2}"