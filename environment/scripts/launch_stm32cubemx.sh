#!/bin/bash

# Throw an error if being invoked from inside the dev container.
if [ -n "$DEV_CONTAINER" ]; then
  echo "ERROR: Attempting to launch STM32CubeMX inside the dev container, which will not work."
  echo "Please re-run from outside the container."
  exit 1
fi

# Launch STM32CubeMX in background.
# & = run in background.
java -jar /usr/local/STM32CubeMX/STM32CubeMX &
