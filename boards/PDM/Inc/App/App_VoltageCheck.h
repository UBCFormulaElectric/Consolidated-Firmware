#pragma once

struct VoltageCheck;

enum VoltageCheck_Status
{
    VOLTAGECHECK_NORMAL,
    VOLTAGECHECK_UNDERVOLTAGE,
    VOLTAGECHECK_OVERVOLTAGE
};

// TODO: Fill in javadoc once the review looks good
struct VoltageCheck *App_VoltageCheck_Create(
    float (*get_voltage)(void),
    float (*get_min_voltage)(void),
    float (*get_max_voltage)(void));

// TODO: Fill in javadoc once the review looks good
void App_VoltageCheck_Tick(struct VoltageCheck *voltage_check);

// TODO: Fill in javadoc once the review looks good
enum VoltageCheck_Status
    App_VoltageCheck_GetStatus(struct VoltageCheck *voltage_check);

// TODO: Fill in javadoc once the review looks good
void App_VoltageCheck_Destroy(struct VoltageCheck *voltage_check);
