#pragma once

struct VoltageCheck;

enum VoltageCheck_Status
{
    VOLTAGECHECK_NORMAL,
    VOLTAGECHECK_UNDERVOLTAGE,
    VOLTAGECHECK_OVERVOLTAGE
};

struct VoltageCheck *App_VoltageCheck_Create(
    float (*get_voltage)(void),
    float (*get_min_voltage)(void),
    float (*get_max_voltage)(void));

void App_VoltageCheck_Tick(struct VoltageCheck *voltage_check);

enum VoltageCheck_Status
    App_VoltageCheck_GetStatus(struct VoltageCheck *voltage_check);

void App_VoltageCheck_Destroy(struct VoltageCheck *voltage_check);
