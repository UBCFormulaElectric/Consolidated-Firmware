#include "io_coolant.h"

#include <math.h>

#include "hw_pwmInputFreqOnly.h"
#include "hw_adc.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "io_log.h"

// source: https://www.adafruit.com/product/828#:~:text=7.5%20*%20Flow%20rate%20(L/min)
#define FLOW_RATE_CONVERSION_FACTOR (7.5f)

// from the schematic we see that the voltage value coming is passes through a voltage dividor before it is read at the
// ADC PIN.
#define PRESSURE_VOLTAGE_DIVIDOR (PRESSURE_BOT_RESISTOR / (PRESSURE_BOT_RESISTOR + PRESSURE_TOP_RESISTOR))
#define PRESSURE_VOLTAGE_MIN (0.5f * PRESSURE_VOLTAGE_DIVIDOR)
#define PRESSURE_VOLTAGE_MAX (4.5f * PRESSURE_VOLTAGE_DIVIDOR)
#define PRESSURE_PSI_MAX (100.0f)
// min pressure is 0 PSI
#define VOLTAGE_PRESSURE_CONVERSION(voltage) \
    (((voltage)-PRESSURE_VOLTAGE_MIN) * (PRESSURE_PSI_MAX / (PRESSURE_VOLTAGE_MAX - PRESSURE_VOLTAGE_MIN)))

// source: https://www.tdk-electronics.tdk.com/inf/50/db/ntc/NTC_Probe_ass_K301_A003.pdf
#define TEMPERATURE_VOLTAGE_MIN (0.0f)
#define TEMPERATURE_VOLTAGE_MAX (4.0f)
#define PRESSURE_TOP_RESISTOR (28.0f)
#define PRESSURE_BOT_RESISTOR (56.0f)

#define T0 (298.15f)           // 25 degrees celsius is used for T0 in datasheet but for equation we do kelvin
#define R0 (10000.f)           // resistance at T0
#define B_COEFFIECENT (3988.f) // source: https://www.tdk-electronics.tdk.com/inf/50/db/ntc/NTC_Probe_ass_K301_A003.pdf
#define VIN (5.0f)
#define R2 (2200.f) // bottom resistor in the coolant temp sensor circuit
// the coolant temp sensor circuit is made of a voltage divider where the thermistor which we use for the temperature
// readings is the top resistor in the voltage divider and R2 is the bottom this tells us that Vout = Vin (R2/ (Rtherm +
// R2)) and by manipulating this equation we can get -- here Vout is our measured value from ADC 4 and 5 Rtherm = ((Vin
// * R2)/ Vout ) - R2)
#define RTHERM(voltage_out) (VIN * R2 / voltage_out - R2)
// below are constants for Steinhart Hart EQN used to model temprature as a function of a resistor for a thermistor
#define BTERM_STEIN_EQN(rtherm) ((float)log((float)(rtherm / R0)) / B_COEFFIECENT)