#include "io_vcShdn.h"
#include "hw_gpios.h"

bool io_vcShdn_TsmsFault_get(void)
{
    return hw_gpio_readPin(&tsms_3v3_out);
}

bool io_vcShdn_InertiaSwitch_get(void)
{
    return hw_gpio_readPin(&interia_3v3_out);
}

bool io_vcShdn_RearRightMotorInterlock_get(void)
{
    return hw_gpio_readPin(&rr_ilck_3v3_out);
}

bool io_vcShdn_SplitterBoxInterlockFault_get(void)
{
    return hw_gpio_readPin(&sb_shdn_3v3_out);
}
