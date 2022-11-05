//
// Created by formulae on 29/10/22.
//

#ifndef FORMULA_ELECTRIC_FIRMWARE_APP_GSMDIGTIAL_H
#define FORMULA_ELECTRIC_FIRMWARE_APP_GSMDIGTIAL_H

#endif //FORMULA_ELECTRIC_FIRMWARE_APP_GSMDIGTIAL_H
struct GSMDigital;
struct GSMDigital *GSMDigital_create (float slope,
                                      float y_int,
                                      float (*get_sensorDigitalVal_voltage)(void),
                                      float (*get_output)(void),
                                      char* units );
void App_GSMDigital_Destroy (struct GSMDigital *gsmDigital);
float get_output(const struct GSMDigital *gsmDigital);
float get_sensorDigitalVal_voltage(const struct GSMDigital *gsmDigital);
float get_frequency(float voltageInput);
