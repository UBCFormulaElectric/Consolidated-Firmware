
struct CurrentMonitoring;

struct CurrentMonitoring *App_CurrentMonitoring_Create(
        float (*get_AUX1_current)(void),
        float (*get_AUX2_current)(void),
        float (*get_LEFT_INVERTER_current)(void),
        float (*get_RIGHT_INVERTER_current)(void),
        float (*get_ENERGY_METER_current)(void),
        float (*get_CAN_current)(void),
        float (*get_AIR_SHUTDOWN_current)(void));

void App_CurrentMonitoring_Destroy(struct CurrentMonitoring *current_monitor);

float App_CurrentMonitoring_Get_AUX1_Current(struct CurrentMonitoring *current_monitor);
float App_CurrentMonitoring_Get_AUX2_Current(struct CurrentMonitoring *current_monitor);
float App_CurrentMonitoring_Get_LEFT_INVERTER_Current(struct CurrentMonitoring *current_monitor);
float App_CurrentMonitoring_Get_RIGHT_INVERTER_Current(struct CurrentMonitoring *current_monitor);
float App_CurrentMonitoring_Get_ENERGY_METER_Current(struct CurrentMonitoring *current_monitor);
float App_CurrentMonitoring_Get_CAN_Current(struct CurrentMonitoring *current_monitor);
float App_CurrentMonitoring_Get_AIR_SHUTDOWN_Current(struct CurrentMonitoring *current_monitor);

