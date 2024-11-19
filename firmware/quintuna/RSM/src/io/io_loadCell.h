#pragma once

float voltageToMechancialLoad(float voltage);

bool OCSC(Adc adcPin_toCheck);
namespace io::loadCell
{

/**
 * @returns mechanical load based off adc pin voltage (after op-amp gain)
 */
float getMechanicalLoad3(void);

/**
 * @returns mechanical loadd based off adc pin voltage (after op-amp gain)
 */
float getMechanicalLoad4(void);

/**
 * @returns checks open and short circuit for load cell 3
 */
bool sensor3OCSC(void);

/**
 * @returns checks open and short circuit for load cell 3
 */
bool sensor4OCSC(void);

} // namespace io::loadCell
