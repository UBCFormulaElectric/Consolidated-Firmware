#!/bin/bash

# Start container in the background, as per the docker compose file. 
docker compose up --detach

# Git doesn't like it when the current user isn't the user that cloned the repo
# (which is the case in the dev container, since the container is run as the root user).
# After bringing up the container and invoking git, you'll see something like: 
#
# fatal: detected dubious ownership in repository at '/root/Consolidated-Firmware'
# To add an exception for this directory, call:
#
#         git config --global --add safe.directory /root/Consolidated-Firmware
#
# Run this at startup, using wildcard to get submodules too.
docker exec ubcformulaelectric-develop git config --system --add safe.directory "*"
