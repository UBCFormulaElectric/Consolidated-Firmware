#include "crit.pb.h"
#include "shared.pb.h"
#include "hw_gpios.h"
#include "hw_spis.h"
#include "io_chimeraConfig_v2.h"

// Chimera V2 enums to GPIO peripherals.
const Gpio *id_to_gpio[] = { [crit_GpioNetName_GPIO_BOOT]                 = &boot,
                             [crit_GpioNetName_GPIO_LED]                  = &led,
                             [crit_GpioNetName_GPIO_TELEM_SIG]            = &telem_sig,
                             [crit_GpioNetName_GPIO_PUSH_DRIVE_SIG]       = &push_drive_sig,
                             [crit_GpioNetName_GPIO_ROT_S]                = &rot_s,
                             [crit_GpioNetName_GPIO_ROT_B]                = &rot_b,
                             [crit_GpioNetName_GPIO_ROT_A]                = &rot_a,
                             [crit_GpioNetName_GPIO_LAUNCH_CONTROL_SIG]   = &launch_control_sig,
                             [crit_GpioNetName_GPIO_TORQUE_VECTORING_SIG] = &torque_vectoring_sig,
                             [crit_GpioNetName_GPIO_REGEN_SIG]            = &regen_sig,
                             [crit_GpioNetName_GPIO_LED_RCK]              = &led_rck,
                             [crit_GpioNetName_GPIO_SEVEN_SEG_RCK]        = &seven_seg_rck,
                             [crit_GpioNetName_GPIO_SEVEN_SEG_DIMMING]    = &seven_seg_dimming,
                             [crit_GpioNetName_GPIO_LED_DIMMING]          = &led_dimming };

const SpiDevice
    *id_to_spi[] = { [crit_SpiNetName_SPI_LED] = &led_spi, [crit_SpiNetName_SPI_SEVEN_SEG] = &seven_seg_spi };

io_chimera_v2_Config chimera_v2_config = { .gpio_net_name_tag = GpioNetName_crit_net_name_tag,
                                           .id_to_gpio        = id_to_gpio,
                                           .spi_net_name_tag  = SpiNetName_crit_net_name_tag,
                                           .id_to_spi         = id_to_spi };
