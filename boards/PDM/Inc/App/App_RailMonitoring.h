
struct RailMonitoring;

struct RailMonitoring *App_RailMonitoring_Create(
    float (*get_VBAT_voltage)(void),
    float (*get__24V_ACC_voltage)(void),
    float (*get__22V_AUX_voltage)(void));

void App_RailMonitoring_Destroy(struct RailMonitoring *rail_monitor);

float App_RailMonitoring_Get_VBAT_Voltage(struct RailMonitoring *rail_monitor);
float App_RailMonitoring_Get__24V_ACC_Voltage(struct RailMonitoring *rail_monitor);
float App_RailMonitoring_Get__22V_AUX_Voltage(struct RailMonitoring *rail_monitor);

bool App_RailMonitoring_VoltageTooLowCheck(float value, float min_value);
bool App_RailMonitoring_VoltageTooHighCheck(float value, float max_value);

bool App_RailMonitoring_VBAT_VoltageTooLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring_VBAT_VoltageTooHighCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__24V_ACC_VoltageTooLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__24V_ACC_VoltageTooHighCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__22V_AUX_VoltageTooLowCheck(struct RailMonitoring *rail_monitor);
bool App_RailMonitoring__22V_AUX_VoltageTooHighCheck(struct RailMonitoring *rail_monitor);