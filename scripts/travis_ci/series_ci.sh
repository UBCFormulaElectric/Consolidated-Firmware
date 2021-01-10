#!/bin/bash

if [ "$#" -ne 5 ]; then
    echo "Please provide the Series name, .ELF file, SHA, max RAM size, and max FLASH size as arguments!"
    exit 1
else
    SERIES_NAME=$1
    ELF=$2
    SHA=$3
    RAM_MAX_SIZE=$4
    FLASH_MAX_SIZE=$5
fi

# Example output of arm-none-eabi-size:
#
# $ arm-none-eabi-size /path/to/elf
#   text    data     bss     dec     hex filename
#  17652    1092    2784   21528    5418 /path/to/elf

size_output=($(arm-none-eabi-size $ELF | grep -P [0-9]+ -o))

text="${size_output[0]}"
data="${size_output[1]}"
bss="${size_output[2]}"

# Flash memory consists of .text and .data because the initialization values
# for the initialized static values are stored in flash.
#
# Read more: https://interrupt.memfault.com/best-firmware-size-tools
flash="$(echo "scale=2; 100 * ($text + $data) / $FLASH_MAX_SIZE" | bc)%"
ram="$(echo "scale=2; 100 * ($bss + $data) / $RAM_MAX_SIZE" | bc)%"

echo "FLASH: $(($text + $data)) / $FLASH_MAX_SIZE ($flash)"
echo "RAM: $(($bss + $data)) / $RAM_MAX_SIZE ($ram)"

curl \
  --header "Authorization: Token 27d62d1f-71c6-4075-95b5-f7aec56d0204" \
  --header "Content-Type: application/json" \
  --data "{\"values\":[{\"value\":\"$ram\",\"line\":\"RAM\"},{\"value\":\"$flash\",\"line\":\"FLASH\"}],\"sha\":\"$SHA\"}" \
  -k https://seriesci.com/api/UBCFormulaElectric/Consolidated-Firmware/$SERIES_NAME/many
