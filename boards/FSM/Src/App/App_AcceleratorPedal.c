#include <assert.h>
#include <stdlib.h>
#include "App_AcceleratorPedal.h"

struct AcceleratorPedal
{
    bool (*is_encoder_alarm_active)(void);
};

struct AcceleratorPedal *
    App_AcceleratorPedal_Create(bool (*is_encoder_alarm_active)(void))
{
    struct AcceleratorPedal *accelerator_pedal =
        malloc(sizeof(struct AcceleratorPedal));
    assert(accelerator_pedal != NULL);

    accelerator_pedal->is_encoder_alarm_active = is_encoder_alarm_active;

    return accelerator_pedal;
}

void App_AcceleratorPedal_Destroy(struct AcceleratorPedal *accelerator_pedal)
{
    free(accelerator_pedal);
}

bool App_AcceleratorPedal_IsEncoderAlarmActive(
    const struct AcceleratorPedal *accelerator_pedal)
{
    return accelerator_pedal->is_encoder_alarm_active;
}
