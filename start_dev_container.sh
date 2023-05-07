MOUNT_LOCAL_DIR=${PWD} # Wherever the repo root is located
MOUNT_CONTAINER_DIR=/usr/src/fe-firmware # Directory in container to bind to local repo
CONTAINER_IMAGE="fe-environment"

docker run --rm -it \
    -e "TERM=xterm-256color" \
    --mount type=bind,source=${MOUNT_LOCAL_DIR},target=${MOUNT_CONTAINER_DIR} \
    ${CONTAINER_IMAGE} \
    bash
