MOUNT_LOCAL_DIR=${PWD} # Wherever this repo is located in developer's filesystem
MOUNT_CONTAINER_DIR=/usr/src/Consolidated-Firmware # Directory in container to bind to local repo
CONTAINER_REPO="consolidated-firmware-dev"
CONTAINER_NAME="consolidated-firmware-dev"

docker run --rm -it \
    -e "TERM=xterm-256color" \
    --name ${CONTAINER_NAME} \
    --mount type=bind,source=${MOUNT_LOCAL_DIR},target=${MOUNT_CONTAINER_DIR} \
    ${CONTAINER_REPO} \
    bash
