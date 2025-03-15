#include "crit.pb.h"
#include "hw_gpio.h"
#include "hw_gpios.h"

const Gpio *id_to_gpio[] = { [crit_GpioNetName_BOOT]                 = &boot,
                             [crit_GpioNetName_LED]                  = &led,
                             [crit_GpioNetName_TELEM_SIG]            = &telem_sig,
                             [crit_GpioNetName_PUSH_DRIVE_SIG]       = &push_drive_sig,
                             [crit_GpioNetName_ROT_S]                = &rot_s,
                             [crit_GpioNetName_ROT_B]                = &rot_b,
                             [crit_GpioNetName_ROT_A]                = &rot_a,
                             [crit_GpioNetName_LAUNCH_CONTROL_SIG]   = &launch_control_sig,
                             [crit_GpioNetName_TORQUE_VECTORING_SIG] = &torque_vectoring_sig,
                             [crit_GpioNetName_REGEN_SIG]            = &regen_sig,
                             [crit_GpioNetName_LED_RCK]              = &led_rck,
                             [crit_GpioNetName_SEVEN_SEG_RCK]        = &seven_seg_rck };
