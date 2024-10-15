#include "hw_adcs.h"
#include "main.h"

namespace hw::adc
{
const AdcChip<NUM_ADC_CHANNELS> adc1{ &hadc1 };
const Adc                       load_cell_1_adc{ adc1.getChannel(ADC1_IN13_LOAD_CELL_1) };
const Adc                       load_cell_2_adc{ adc1.getChannel(ADC1_IN1_LOAD_CELL_2) };
const Adc                       apps2_adc{ adc1.getChannel(ADC1_IN5_APPS2) };
const Adc                       apps1_adc{ adc1.getChannel(ADC1_IN12_APPS1) };
const Adc                       break_pres_sensor_f_adc{ adc1.getChannel(ADC1_IN7_BPS_F) };
const Adc                       break_pres_sensor_b_adc{ adc1.getChannel(ADC1_IN15_BPS_B)};
const Adc                       susp_travel_fl_adc{ adc1.getChannel(ADC1_IN8_SUSP_TRAVEL_FL)};
const Adc                       susp_travel_fl_adc{ adc1.getChannel(ADC1_IN9_SUSP_TRAVEL_FR)};
const Adc                       steering_angle{ adc1.getChannel(ADC1_IN11_STEERING_ANGLE)}
} // namespace hw::adc

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    UNUSED(hadc);
    hw::adc::adc1.update_callback();
}