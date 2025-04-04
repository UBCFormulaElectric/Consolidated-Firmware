#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"

/**
 * Clears the status registers
 * @return success of operation
 */
// static bool clearStatusRegisters()
// {
// #define CLRSTAT (0x0713)
//     return io_ltc6813_sendCommand(CLRSTAT);
// }

// TODO assert that for each speed that the ADCOPT is correct
#define ST (0x2)

bool io_ltc6813_sendSelfTestVoltages(const ADCSpeed speed)
{
    const uint16_t adc_speed_factor = speed & 0x3;
#define CVST (0x0207U | adc_speed_factor | ST << 5)
    if (!io_ltc6813_sendCommand(CVST))
    {
        return false;
    }
    return true;
}

bool io_ltc6813_sendSelfTestAux(const ADCSpeed speed)
{
    const uint16_t adc_speed_factor = speed & 0x3;
#define AXST (0x407 | adc_speed_factor | (uint16_t)(ST << 5))
    if (!io_ltc6813_sendCommand(AXST))
    {
        return false;
    }
    return true;
}

bool io_ltc6813_sendSelfTestStat(const ADCSpeed speed)
{
    const uint16_t adc_speed_factor = speed & 0x3;
#define STATST (0x40F | adc_speed_factor | (uint16_t)(ST << 5))
    if (!io_ltc6813_sendCommand(STATST))
    {
        return false;
    }
    return true;
}
