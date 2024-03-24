#include "io_fsmShdn.h"

static const FsmShdnConfig *shdn_config;

void io_fsmShdn_init(const FsmShdnConfig *fsm_shdn_config) 
{
    shdn_config = fsm_shdn_config;
}

int io_fsm_num_shdn_nodes() {
    return shdn_config->fsm_num_nodes;
}

bool io_get_FSM_SHDN_OK()
{
    return hw_gpio_readPin(&shdn_config->fsm_shdn_ok_gpio);
}

