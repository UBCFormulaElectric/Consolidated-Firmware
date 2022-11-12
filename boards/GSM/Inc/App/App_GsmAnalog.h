//
// Created by formulae on 20/10/22.
//
#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <linkedlist.h>
#ifndef FORMULA_ELECTRIC_FIRMWARE_APP_GSMANALOG_H
#define FORMULA_ELECTRIC_FIRMWARE_APP_GSMANALOG_H

#endif //FORMULA_ELECTRIC_FIRMWARE_APP_GSMANALOG_H

struct GsmAnalog;

struct GSMAnalog *GSMAnalog_create (float slope,
                                    float y_int,
                                    float (*get_sensorAnalogVal_voltage)(void),
                                    char* units);

void App_GSMAnalog_Destroy (struct GSMAnalog *gsmAnalog);

float get_output(const struct GSMAnalog *gsmAnalog);

char* get_units(const struct GSMAnalog *gsmAnalog);
