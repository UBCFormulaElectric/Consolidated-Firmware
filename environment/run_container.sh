#!/bin/bash

# CI container image.
IMAGE="ubcformulaelectric/environment:new_cube"

# Check if the image exists locally.
if [[ "$(docker images -q ${IMAGE} 2> /dev/null)" == "" ]]; then
  echo "Image '${IMAGE}' not found locally. Pulling from repository..."

  # Pull the image from the repository.
  docker pull ${IMAGE}

  # Check if the pull was successful.
  if [[ $? -eq 0 ]]; then
    echo "Image pulled successfully."
  else
    echo "Failed to pull the image."
    exit 1
  fi
else
  echo "Image '${IMAGE}' found locally."
fi

ENVIRONMENT_DIR="$(cd "$(dirname "$0")" && pwd)" # Directory of this script
REPO_ROOT_DIR="${ENVIRONMENT_DIR}/.." # Path to repo root
CONTAINER_MOUNT_DIR="/root/Consolidated-Firmware" # Directory in container to mount repo

# Run container, and mount local repo.
docker run \
    --rm \
    -it \
    --mount type=bind,source=${REPO_ROOT_DIR},target=${CONTAINER_MOUNT_DIR} \
    ${IMAGE} \
    bash
