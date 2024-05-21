#include "io_fsmShdn.h"

static const FsmShdnConfig *shdn_config = NULL;

void io_fsmShdn_init(const FsmShdnConfig *fsm_shdn_config)
{
    shdn_config = fsm_shdn_config;
}

bool io_fsmShdn_FSM_SHDN_OK_get(void)
{
    return hw_gpio_readPin(&shdn_config->fsm_shdn_ok_gpio);
}
