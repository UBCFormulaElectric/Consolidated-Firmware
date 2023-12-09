Set-Variable -Name "GENERATED_DIR" -Value "./example_generated"
Set-Variable -Name "GENERATED_APP_DIR" -Value "${GENERATED_DIR}/App"
Set-Variable -Name "GENERATED_IO_DIR" -Value "${GENERATED_DIR}/Io"
Set-Variable -Name "GENERATED_DBC_DIR" -Value "${GENERATED_DIR}/Dbc"

python generate_can_from_json.py `
    --board=JCT `
    --can_data_dir="./example_json" `
    --app_can_tx_header_output="${GENERATED_APP_DIR}/App_CanTx.h" `
    --app_can_tx_source_output="${GENERATED_APP_DIR}/App_CanTx.c" `
    --io_can_tx_header_output="${GENERATED_IO_DIR}/Io_CanTx.h" `
    --io_can_tx_source_output="${GENERATED_IO_DIR}/Io_CanTx.c" `
    --app_can_rx_header_output="${GENERATED_APP_DIR}/App_CanRx.h" `
    --app_can_rx_source_output="${GENERATED_APP_DIR}/App_CanRx.c" `
    --io_can_rx_header_output="${GENERATED_IO_DIR}/Io_CanRx.h" `
    --io_can_rx_source_output="${GENERATED_IO_DIR}/Io_CanRx.c" `
    --app_can_utils_header_output="${GENERATED_APP_DIR}/App_CanUtils.h" `
    --app_can_utils_source_output="${GENERATED_APP_DIR}/App_CanUtils.c" `
    --app_can_alerts_header_output="${GENERATED_APP_DIR}/App_CanAlerts.h" `
    --app_can_alerts_source_output="${GENERATED_APP_DIR}/App_CanAlerts.c" `
    --dbc_output="${GENERATED_DBC_DIR}/example.dbc"

echo "Generated code."