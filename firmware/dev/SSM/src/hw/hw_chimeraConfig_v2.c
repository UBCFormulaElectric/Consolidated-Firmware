#include "ssm.pb.h"
#include "shared.pb.h"
#include "hw/gpios.h"
#include "hw/spis.h"
#include "hw/chimeraConfig_v2.h"

// Chimera V2 enums to GPIO peripherals.
const Gpio *id_to_gpio[] = {
    [ssm_GpioNetName_GPIO_DOUT_1] = &dout_1,           [ssm_GpioNetName_GPIO_DOUT_2] = &dout_2,
    [ssm_GpioNetName_GPIO_DOUT_3] = &dout_3,           [ssm_GpioNetName_GPIO_DOUT_4] = &dout_4,

    [ssm_GpioNetName_GPIO_DEBUG_LED] = &debug_led,     [ssm_GpioNetName_GPIO_BOOT_LED] = &boot_led,

    [ssm_GpioNetName_GPIO_INDICATOR_1] = &indicator_1, [ssm_GpioNetName_GPIO_INDICATOR_2] = &indicator_2,
    [ssm_GpioNetName_GPIO_INDICATOR_3] = &indicator_3,

    [ssm_GpioNetName_GPIO_INTERLOCK_1] = &interlock_1, [ssm_GpioNetName_GPIO_INTERLOCK_2] = &interlock_2,
    [ssm_GpioNetName_GPIO_INTERLOCK_3] = &interlock_3, [ssm_GpioNetName_GPIO_INTERLOCK_4] = &interlock_4,

    [ssm_GpioNetName_GPIO_DAC_N_CLEAR] = &dac_n_clear
};

// Chimera V2 enums to SPI peripherals.
const SpiDevice *id_to_spi[] = {
    [ssm_SpiNetName_SPI_ISOSPI_LS] = &isospi_ls,
    [ssm_SpiNetName_SPI_ISOSPI_HS] = &isospi_hs,
    [ssm_SpiNetName_SPI_DAC]       = &dac,
};

hw_chimera_v2_Config chimera_v2_config = { .gpio_net_name_tag = GpioNetName_ssm_net_name_tag,
                                           .id_to_gpio        = id_to_gpio,
                                           .spi_net_name_tag  = SpiNetName_ssm_net_name_tag,
                                           .id_to_spi         = id_to_spi };