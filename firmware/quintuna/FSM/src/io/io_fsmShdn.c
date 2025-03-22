#include "io_fsmShdn.h"
#include "hw_gpios.h"

bool io_fsmShdn_FSM_SHDN_OK_get(void)
{
    return hw_gpio_readPin(&cockpit_shdn_3v3); // 3 more needed for BOTS, FL, FR, CPIT
}
