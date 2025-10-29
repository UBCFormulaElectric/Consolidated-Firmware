#include "app_imd.hpp"
#include <assert.h>

extern "C" {
    #include "app_canUtils.h"
    #include "app_canTx.h"
    #include "app_utils.h"
}

constexpr int NUM_OF_IMD_CONDITIONS = 6;
constexpr float IMD_FREQUENCY_TOLERANCE = 2.0;
constexpr float IMD_FREQUENCY_LOOKUP[] = {
    0.0f ,          // IMD_CONDITION_SHORT_CIRCUIT
    10.0f,          // IMD_CONDITION_NORMAL
    20.0f,          // IMD_CONDITION_UNDERVOLTAGE_DETECTED
    30.0f,          // IMD_CONDITION_SST
    40.0f,          // IMD_CONDITION_DEVICE_ERROR
    50.0f           // IMD_CONDITION_GROUND_FAULT
};

namespace app::imd {

    static ImdConditionName estimateConditionName(const float frequency) {
        ImdConditionName condition_name = IMD_CONDITION_INVALID;
        for (ImdConditionName i = 0U; i < NUM_OF_IMD_CONDITIONS; i++) {

            const float lower_bound = getIdealPwmFrequency(i) - IMD_FREQUENCY_TOLERANCE;
            const float upper_bound = getIdealPwmFrequency(i) + IMD_FREQUENCY_TOLERANCE;

            if (frequency >= lower_bound && frequency <= upper_bound) {
                condition_name = i;
                break;
            }
        }
        return condition_name;
    }

    static float getIdealPwmFrequency(const ImdConditionName condition_name){
        assert(condition_name < NUM_OF_IMD_CONDITIONS);
        return IMD_FREQUENCY_LOOKUP[condition_name];
    }

    ImdCondition getCondition() {
        const float pwm_frequency = io::imd::getFrequency();
        const float pwm_duty_cycle = io::imd::getDutyCycle();
        const uint8_t ticks_since_pwm_update = io::imd::pwmCounterTick();
        ImdCondition condition {
            .name = ticks_since_pwm_update == UINT8_MAX ? IMD_CONDITION_SHORT_CIRCUIT : estimateConditionName(pwm_frequency),
        }
        
        switch (condition.name) {
            case IMD_CONDITION_SHORT_CIRCUIT: {
                condition.valid_duty_cycle = false;
            }
            break;
            case IMD_CONDITION_NORMAL:
            case IMD_CONDITION_UNDERVOLTAGE_DETECTED: {
                condition.valid_duty_cycle = condition.valid_duty_cycle = (pwm_duty_cycle >= 5.0f && pwm_duty_cycle <= 90.0f);
                if (condition.valid_duty_cycle) {
                    
                }
            }
            break;
            case IMD_CONDITION_SST: {

            }
            break;
            case IMD_CONDITION_DEVICE_ERROR:
            case IMD_CONDITION_GROUND_FAULT: {
            }
            break;
            case IMD_CONDITION_INVALID:
            default: {
        }  
    }

    void broadcast() {
        app_canTx_BMS_ImdFrequency_set(io::imd::getFrequency());
        app_canTx_BMS_ImdDutyCycle_set(io::imd::getDutyCycle());
        app_canTx_BMS_ImdTimeSincePowerOn_set(io::imd::getTimeSincePowerOn());

        const ImdCondition condition = app::imd::getCondition();
        app_canTx_BMS_ImdCondition_set(condition.name);
        app_canTx_BMS_ImdValidDutyCycle_set(condition.valid_duty_cycle);

        switch (condition.name) {
            case IMD_CONDITION_SHORT_CIRCUIT: {
                app_canTx_BMS_ImdActiveFrequency_set(IMD_0Hz);
            } 
            break;
            case IMD_CONDITION_NORMAL: {
                if (condition.valid_duty_cycle) {
                    app_canTx_BMS_ImdInsulationMeasurementDcp10Hz_set(condition.payload.insulation_measurement_dcp_kohms);
                    app_canTx_BMS_ImdActiveFrequency_set(IMD_10Hz);
                }
            }
            break;
            case IMD_CONDITION_UNDERVOLTAGE_DETECTED: {
                if (condition.valid_duty_cycle) {
                    app_canTx_BMS_ImdInsulationMeasurementDcp20Hz_set(condition.payload.insulation_measurement_dcp_kohms);
                    app_canTx_BMS_ImdActiveFrequency_set(IMD_20Hz);
                }
            }
            break;
            case IMD_CONDITION_SST: {
                app_canTx_BMS_ImdSpeedStartStatus30Hz_set(condition.payload.speed_start_status);
                app_canTx_BMS_ImdActiveFrequency_set(IMD_30Hz);
            }
            break;
            case IMD_CONDITION_DEVICE_ERROR: {
                app_canTx_BMS_ImdActiveFrequency_set(IMD_40Hz);
            } 
            break;
            case IMD_CONDITION_GROUND_FAULT: {
                app_canTx_BMS_ImdActiveFrequency_set(IMD_50Hz);
            }
            break;
            case IMD_CONDITION_INVALID:
            default: {

            }
        }
    }


    


}
