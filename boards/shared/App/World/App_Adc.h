#ifndef APP_ADC_H
#define APP_ADC_H

struct App_Adc_Interface;

App_Adc_Interface*
    App_Adc_Alloc(float32_t (*get_adc_voltage)(uint32_t regular_rank));

float32_t
    App_Adc_GetVoltage(App_Adc_Interface *adc_interface, uint32_t regular_rank);

#endif /* APP_ADC_H */
