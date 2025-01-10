#include "io_suspension.h"
#include <stdbool.h>
#include "hw_adcs.h"

// CHECK MIN/MAX VALUES FOR RSM

#define MIN_SUSPENSION_VOLTAGE (0.2f)
#define MAX_SUSPENSION_VOLTAGE (3.5f)

float travelFromVoltage(float voltage) {
    // TODO: implement
    return voltage;
}

float io_suspension_getRearLeftTravel(void) {
    return hw_adc_getVoltage(&susp_travel_rl);
}

float io_suspension_getRearRightTravel(void) {
    return hw_adc_getVoltage(&susp_travel_rr);
}

bool io_suspension_leftSensorOCSC(void) {
    const float leftSensorVoltage = hw_adc_getVoltage(&susp_travel_rl);
    return !(MIN_SUSPENSION_VOLTAGE <= leftSensorVoltage && leftSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}

bool io_suspension_rightSensorOCSC(void) {
    const float rightSensorVoltage = hw_adc_getVoltage(&susp_travel_rr);
    return !(MIN_SUSPENSION_VOLTAGE <= rightSensorVoltage && rightSensorVoltage <= MAX_SUSPENSION_VOLTAGE);
}