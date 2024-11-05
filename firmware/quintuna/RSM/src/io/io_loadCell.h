#pragma once

namespace io::loadCell
{

float voltageToMechancialLoad(float voltage);

bool OCSC(Adc adcPin_toCheck);

/**
 * @returns mechanical loadd based off adc pin voltage (after op-amp gain)
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