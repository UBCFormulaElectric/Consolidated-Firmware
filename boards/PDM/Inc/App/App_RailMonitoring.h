
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

void App_RailMonitoring_Destroy(struct RailMonitoring *rail_monitor);

/**
 *
 * @param rail_monitor
 * @return VBAT Voltage
 */
float App_RailMonitoring_Get_VBAT_Voltage(struct RailMonitoring *rail_monitor);

/**
 *
 * @param rail_monitor
 * @return ACC Voltage
 */
float App_RailMonitoring_Get__24V_ACC_Voltage(struct RailMonitoring *rail_monitor);

/**
 *
 * @param rail_monitor
 * @return AUX Voltage
 */
float App_RailMonitoring_Get__22V_AUX_Voltage(struct RailMonitoring *rail_monitor);

/**
 * Local function checking if voltage is too low
 * @param value
 * @param min_value
 * @return true if too low, false if not
 */
bool App_RailMonitoring_VoltageLowCheck(float value, float min_value);

/**
 * Local function checking if voltage is too high
 * @param value
 * @param max_value
 * @return true if too high, false if not
 */
bool App_RailMonitoring_VoltageHighCheck(float value, float max_value);

// Functions below check if each voltage is too low or too high
bool App_RailMonitoring_VBAT_VoltageLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring_VBAT_VoltageHighCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__24V_ACC_VoltageLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__24V_ACC_VoltageHighCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__22V_AUX_VoltageLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__22V_AUX_VoltageHighCheck(struct RailMonitoring *rail_monitor);

// Functions below check if each voltage is critical or not
bool App_RailMonitoring_VBAT_VoltageCriticalCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__24V_ACC_VoltageCriticalCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__22V_AUX_VoltageCriticalCheck(struct RailMonitoring *rail_monitor);
