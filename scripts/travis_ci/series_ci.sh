if [ "$#" -ne 3 ]; then
    echo "Please provide the Series name, the .ELF file, and SHA as arguments!"
    exit 1
else
    SERIES_NAME=$1
    ELF=$2
    SHA=$3
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

curl \
  --header "Authorization: Token 27d62d1f-71c6-4075-95b5-f7aec56d020" \
  --header "Content-Type: application/json" \
  --data "{\"values\":[{\"value\":\"$text\",\"line\":\"text\"},{\"value\":\"$data\",\"line\":\"data\"},{\"value\":\"$bss\",\"line\":\"bss\"}],\"sha\":\"$SHA\"}" \
  https://seriesci.com/api/UBCFormulaElectric/Consolidated-Firmware/$SERIES_NAME/many
