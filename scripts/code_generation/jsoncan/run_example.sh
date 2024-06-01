# Disallow errors
set -e

python3 generate_can_from_json.py \
--board=JCT \
--can_data_dir="./example_json" \
--output_dir="./example_generated/" \
--dbc_output="./example_generated/example.dbc" 

echo "Generated code."