#include "App_CanTx.h"
#include "TL85.h"

extern struct setter SetTirePressure_setter;
extern struct setter SetBatteryVoltage_setter;
extern struct setter SetWheelSpeed_setter;

static void TL85_EnableSignalOverride(uint32_t signal_id, uint32_t value)
{
    switch (signal_id)
    {
        case 0:
        {
            SetTirePressure_setter.override_setter(value);
        }
        break;

        case 1:
        {
            SetBatteryVoltage_setter.override_setter(value);
        }
        break;

        case 2:
        {
            SetWheelSpeed_setter.override_setter(value);
        }
        break;
    }
}

static void TL85_DisableSignalOverride(uint32_t signal_id)
{
    switch (signal_id)
    {
        case 0:
        {
            SetTirePressure_setter.disable_override();
        }
        break;

        case 1:
        {
            SetBatteryVoltage_setter.disable_override();
        }
        break;

        case 2:
        {
            SetWheelSpeed_setter.disable_override();
        }
        break;
    }
}

void TL85_CanCallback(uint32_t stdid, uint8_t data[8])
{
    if (stdid == CANMSGS_SIGNAL_INJECTION_FRAME_ID)
    {
        struct CanMsgs_signal_injection_t request;

        App_CanMsgs_signal_injection_unpack(
            &request, data, CANMSGS_SIGNAL_INJECTION_LENGTH);

        uint32_t signal_id = request.signal_id;

        if (request.enable_override)
        {
            TL85_EnableSignalOverride(signal_id, request.value);
        }
        else
        {
            TL85_DisableSignalOverride(signal_id);
        }
    }
}
