if [ "$#" -ne 1 ]; then
    echo "This script takes exactly one parameter, the name of the board to generate CAN c code for. Received $# args instead!"
fi
cd ../..
pipenv run python boards.shared.generate_c_code_from_dbc.py \
    --board="$@"
    --source_dir="boards/$@/Src/auto_generated"
    --header_dir"boards/$@/Inc/auto_generated", 
    --cantools_source_dir"boards/shared/Src/auto_generated", 
    --cantools_header_dir"boards/shared/Inc/auto_generated", 
    --dbc="boards/shared/generate_CAN/CanMsgs.dbc"
