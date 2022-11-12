//
// Created by formulae on 20/10/22.
//
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <linkedlist.h>
#include "App_GsmAnalog.h"
struct GSMAnalog {
    struct GSMAnalog *GSMAnalog_create;
    float slope;
    float y_int;
    float (*get_sensorAnalogVal_voltage)(void);
    float (*get_output)(void);
    char* units;

};
struct GSMAnalog *GSMAnalog_create (float slope,
        float y_int,
        float (*get_sensorAnalogVal_voltage)(void),
        float (*get_output)(void),
        char* units)
{
    struct GSMAnalog *gsmAnalog = malloc(sizeof (struct GSMAnalog));
    assert(gsmAnalog != NULL);
    gsmAnalog ->slope = slope;
    gsmAnalog -> y_int = y_int;
    gsmAnalog -> get_sensorAnalogVal_voltage = get_sensorAnalogVal_voltage;
    gsmAnalog -> get_output = get_output;
    gsmAnalog -> units = units;
    return gsmAnalog;

}
void App_GSMAnalog_Destroy (struct GSMAnalog *gsmAnalog)
{
    free(gsmAnalog);
}
float get_output(const struct GSMAnalog *gsmAnalog)
        {
    return gsmAnalog->slope *  gsmAnalog->get_sensorAnalogVal_voltage() + gsmAnalog->y_int;
        }

