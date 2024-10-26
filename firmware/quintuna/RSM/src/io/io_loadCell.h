#pragma once

namespace io::loadCell
{

    float loadCell_voltageToMechancialLoad(float voltage)

    float loadCell_OCSC(Adc adcPin_toCheck)

    /**
    * @returns mechanical loadd based off adc pin voltage (after op-amp gain)
    */
    float io_loadCell_getMechanicalLoad3(void);

    /**
    * @returns mechanical loadd based off adc pin voltage (after op-amp gain)
     */
    float io_loadCell_getMechanicalLoad4(void);

    /**
     * @returns checks open and short circuit for load cell 3
     */
    bool io_loadCell_sensor3OCSC(void);

    /**
     * @returns checks open and short circuit for load cell 3
     */
    bool io_loadCell_sensor4OCSC(void);

} // namespace io::loadCell



