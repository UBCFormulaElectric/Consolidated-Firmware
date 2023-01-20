
struct RailMonitoring;

struct RailMonitoring *App_RailMonitoring_Create(
    float (*get_VBAT_voltage)(void),
    float (*get__24V_ACC_voltage)(void),
    float (*get__22V_AUX_voltage)(void));

void App_RailMonitoring_Destroy(struct RailMonitoring *rail_monitor);

float App_RailMonitoring_Get_VBAT_Voltage(struct RailMonitoring *rail_monitor);
float App_RailMonitoring_Get__24V_ACC_Voltage(struct RailMonitoring *rail_monitor);
float App_RailMonitoring_Get__22V_AUX_Voltage(struct RailMonitoring *rail_monitor);

bool App_RailMonitoring_VoltageLowCheck(float value, float min_value);
bool App_RailMonitoring_VoltageHighCheck(float value, float max_value);

bool App_RailMonitoring_VBAT_VoltageLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring_VBAT_VoltageHighCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__24V_ACC_VoltageLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__24V_ACC_VoltageHighCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__22V_AUX_VoltageLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__22V_AUX_VoltageHighCheck(struct RailMonitoring *rail_monitor);

bool App_RailMonitoring_VBAT_VoltageCriticalCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__24V_ACC_VoltageCriticalCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__22V_AUX_VoltageCriticalCheck(struct RailMonitoring *rail_monitor);
