#include "io_critShdn.h"

static const CritShdnConfig* shdn_config = NULL;

void io_critShdn_init(const CritShdnConfig* crit_shdn_config) {
    shdn_config = crit_shdn_config;
}

bool io_critShdn_COCKPIT_ESTOP_OK_get(void) {
    return hw_gpio_readPin(shdn_config->cockpit_estop_gpio);
}

bool io_critShdn_get_INERTIA_SEN_OK_get(void) {
    return hw_gpio_readPin(shdn_config->inertia_sen_ok_gpio);
}
