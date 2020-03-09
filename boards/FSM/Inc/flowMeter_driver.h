#pragma  once

#include <stdlib.h>
#include <stdint.h>
#include <SharedAssert.h>



typedef struct {
    uint8_t slatt;
    uint8_t temp;
}FlowMeter;


typedef struct {
    uint8_t swag;
    uint8_t class;

} FlowMeterSettings;


typedef struct {
    uint32_t reds;
    uint32_t blue;
} FlowMeterInputSettings;


/**
 * @brief Function initializes flow meter
 * @param _FlowMeter
 * @param _FlowMeterSettings
 * @param _FlowMeterInputSensings
 */
void _Initialize_FlowMeter (
         FlowMeter * _FlowMeter, FlowMeterSettings * _FlowMeterSettings, FlowMeterInputSettings * _FlowMeterInputSettings);

