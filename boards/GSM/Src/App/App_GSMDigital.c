//
// Created by formulae on 29/10/22.
//

#include "App_GSMDigital.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <linkedlist.h>
#include "Io_SharedFreqOnlyPwmInput.h"

struct GSMDigital {
    struct GSMDigital *GSMDigital_create;
    float slope;
    float y_int;
    float (*get_sensorDigitalVal_voltage)(void);
    float (*get_output)(void);
    char* units;

};

struct GSMDigital *GSMDigital_create (float slope,
        float y_int,
        float (*get_sensorDigitalVal_voltage)(void),
        float (*get_output)(void),
        char* units ){
    struct GSMDigital *gsmDigital = malloc(sizeof (struct GSMDigital));
    assert(gsmDigital != NULL);
    gsmDigital->slope = slope;
    gsmDigital->y_int = y_int;
    gsmDigital->get_sensorDigitalVal_voltage = get_sensorDigitalVal_voltage;
    gsmDigital->get_output = get_output;
    gsmDigital -> units = units;
    return gsmDigital;
}
void App_GSMDigital_Destroy (struct GSMDigital *gsmDigital)
{
    free(gsmDigital);
}

float get_output(const struct GSMDigital *gsmDigital){
    float voltageInput =gsmDigital->get_sensorDigitalVal_voltage();
    return gsmDigital->slope *  get_frequency(voltageInput) + gsmDigital->y_int;
}

float get_sensorDigitalVal_voltage(const struct GSMDigital *gsmDigital){
    return gsmDigital-> get_sensorDigitalVal_voltage();
}

float get_frequency(float voltageInput){
    return ;
}
/*
 * implement
 * Io_SetFrequency(pwm_input, pwm_input->tim_frequency_hz / (float)rising_edge_delta);
 * figure how to set up the frequency
 */