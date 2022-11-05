
struct RailMonitoring;

struct RailMonitoring *App_RailMonitoring_Create(
        float (*get_VBAT_voltage)(void),
        float (*get__24V_ACC_voltage)(void),
        float (*get__24V_AUX_voltage)(void));

void App_RailMonitoring_Destroy(struct RailMonitoring *rail_monitor);

float App_RailMonitoring_Get_VBAT_Voltage(struct RailMonitoring *rail_monitor);
float App_RailMonitoring_Get__24V_ACC_Voltage(struct RailMonitoring *rail_monitor);
float App_RailMonitoring_Get__24V_AUX_Voltage(struct RailMonitoring *rail_monitor);
