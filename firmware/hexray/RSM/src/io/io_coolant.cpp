#include "io_coolant.hpp"
// #include "hw_pwmInputFreqOnly.hpp"
// #include "hw_pwms.hpp"          // 3 of them not created yet
// #include "io_log.hpp"

#define FREQ_TO_LITERS_PER_MINUTE 7.5f

int hw_pwmInputFreqOnly_init(int *ptr){return 0;}
int hw_pwmInputFreqOnly_getFrequency(int* ptr){return 0;}
bool hw_pwmInputFreqOnly_checkIfPwmIsActive(int* ptr){return true;}
int* flow_meter_5v5 = (int*)1;
int* flow_meter_config = (int*)1;

void io_coolant_init()
{
    //could have diff flow meter, thus diff address
    hw_pwmInputFreqOnly_init(&flow_meter_5v5, &flow_meter_config);
}

float io_coolant_getFlowRate()
{
    const float freq_read = hw_pwmInputFreqOnly_getFrequency(&flow_meter_5v5);
    return freq_read * FREQ_TO_LITERS_PER_MINUTE;
}

void io_coolant_checkIfFlowMeterActive()
{
    hw_pwmInputFreqOnly_checkIfPwmIsActive(&flow_meter_5v5);
}