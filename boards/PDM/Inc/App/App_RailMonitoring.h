
struct RailMonitoring;

/**
 *
 * @param get_VBAT_voltage VBAT Voltage Value
 * @param get__24V_ACC_voltage Accumulator Voltage Value
 * @param get__22V_AUX_voltage Boost Voltage Value
 * @return rail_monitor structure
 */
struct RailMonitoring *App_RailMonitoring_Create(
    float (*get_VBAT_voltage)(void),
    float (*get__24V_ACC_voltage)(void),
    float (*get__22V_AUX_voltage)(void));

/**
 * Gets the VBAT Voltage from the PDM in Volts
 * @param rail_monitor
 * @return VBAT Voltage Value from the PDM
 */
float App_RailMonitoring_GetVbatVoltage(struct RailMonitoring *rail_monitor);

/**
 * Gets the Accumulator Voltage from the PDM in Volts
 * @param rail_monitor
 * @return Accumulator Voltage Value from the PDM
 */
float App_RailMonitoring_Get24VAccumulatorVoltage(struct RailMonitoring *rail_monitor);

/**
 * Gets the AUX Voltage from the PDM in Volts
 * @param rail_monitor
 * @return AUX Voltage Value from the PDM
 */
float App_RailMonitoring_Get22VAuxiliaryVoltage(struct RailMonitoring *rail_monitor);

/**
 * Checks if the VBAT Voltage from the PDM is too low
 * @param rail_monitor
 * @return true if voltage too low, false otherwise
 */
bool App_RailMonitoring_VbatVoltageLowCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if the VBAT Voltage from the PDM is too high
 * @param rail_monitor
 * @return true if voltage too high, false otherwise
 */
bool App_RailMonitoring_VbatVoltageHighCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if the Accumulator Voltage from the PDM is too low
 * @param rail_monitor
 * @return true if voltage too low, false otherwise
 */
bool App_RailMonitoring_24VAccumulatorVoltageLowCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if the Accumulator Voltage from the PDM is too high
 * @param rail_monitor
 * @return true if voltage too high, false otherwise
 */
bool App_RailMonitoring_24VAccumulatorVoltageHighCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if the AUX Voltage from the PDM is too low
 * @param rail_monitor
 * @return true if voltage too low, false otherwise
 */
bool App_RailMonitoring_22VAuxiliaryVoltageLowCheck(struct RailMonitoring *rail_monitor);

/**
 * Checks if the AUX Voltage from the PDM is too high
 * @param rail_monitor
 * @return true if voltage too high, false otherwise
 */
bool App_RailMonitoring_22VAuxiliaryVoltageHighCheck(struct RailMonitoring *rail_monitor);
