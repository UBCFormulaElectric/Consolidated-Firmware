#include "f4dev.pb.h"
#include "shared.pb.h"
#include "hw/chimeraConfig_v2.h"

#include "hw/gpios.h"
#include "hw/spis.h"

const Gpio *id_to_gpio[] = {
    [f4dev_GpioNetName_GPIO_6] = &gpio_6,
    [f4dev_GpioNetName_GPIO_5] = &gpio_5,
};

hw_chimera_v2_Config chimera_v2_config = {
    .gpio_net_name_tag = GpioNetName_f4dev_net_name_tag,
    .id_to_gpio        = id_to_gpio,
};