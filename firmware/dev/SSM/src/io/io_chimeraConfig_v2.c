#include "ssm.pb.h"
#include "shared.pb.h"
#include "hw_gpios.h"
#include "hw_spis.h"
#include "io_chimeraConfig_v2.h"

// Chimera V2 enums to GPIO peripherals.
const Gpio *id_to_gpio[] = {
    [ssm_GpioNetName_GPIO_INDICATOR_1] = &indicator_1, [ssm_GpioNetName_GPIO_INDICATOR_2] = &indicator_2,
    [ssm_GpioNetName_GPIO_INDICATOR_3] = &indicator_3, [ssm_GpioNetName_GPIO_DOUT_4] = &dout_4,
    [ssm_GpioNetName_GPIO_DEBUG_LED] = &debug_led,
};

io_chimera_v2_Config chimera_v2_config = { .gpio_net_name_tag = GpioNetName_ssm_net_name_tag,
                                           .id_to_gpio        = id_to_gpio };