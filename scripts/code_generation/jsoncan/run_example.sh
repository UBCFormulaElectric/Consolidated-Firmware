# Disallow errors
set -e

GENERATED_DIR="./example_generated"
GENERATED_APP_DIR="${GENERATED_DIR}/app"
GENERATED_IO_DIR="${GENERATED_DIR}/io"
GENERATED_DBC_DIR="${GENERATED_DIR}/dbc"

python3 generate_can_from_json.py \
--board=JCT \
--can_data_dir="./example_json" \
--app_can_tx_header_output="${GENERATED_APP_DIR}/app_canTx.h" \
--app_can_tx_source_output="${GENERATED_APP_DIR}/app_canTx.c" \
--io_can_tx_header_output="${GENERATED_IO_DIR}/io_canTx.h" \
--io_can_tx_source_output="${GENERATED_IO_DIR}/io_canTx.c" \
--app_can_rx_header_output="${GENERATED_APP_DIR}/app_canRx.h" \
--app_can_rx_source_output="${GENERATED_APP_DIR}/app_canRx.c" \
--io_can_rx_header_output="${GENERATED_IO_DIR}/io_canRx.h" \
--io_can_rx_source_output="${GENERATED_IO_DIR}/io_canRx.c" \
--app_can_utils_header_output="${GENERATED_APP_DIR}/app_canUtils.h" \
--app_can_utils_source_output="${GENERATED_APP_DIR}/app_canUtils.c" \
--app_can_alerts_header_output="${GENERATED_APP_DIR}/app_canAlerts.h" \
--app_can_alerts_source_output="${GENERATED_APP_DIR}/app_canAlerts.c" \
--dbc_output="${GENERATED_DBC_DIR}/example.dbc" 

echo "Generated code."