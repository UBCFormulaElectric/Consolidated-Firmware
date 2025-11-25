// #include "io_suspension.h"
// #include "hw_adcs.h"
// #include "hw_gpios.h"

// namespace io::suspension
// {
// float getLeftTravel(void)
// {
//     // Return left suspension travel based on the ADC voltage.
//     return hw_adc_getVoltage(&susp_fl);
// }

// float getRightTravel(void)
// {
//     // Return right suspension travel based on the ADC voltage.
//     return hw_adc_getVoltage(&susp_fr);
// }

// bool leftSensorOCSC(void)
// {
//     return !hw_gpio_readPin(&nsusp_fl_ocsc);
// }

// bool rightSensorOCSC(void)
// {
//     return !hw_gpio_readPin(&nsusp_fr_ocsc);
// }
// } // namespace io::suspension