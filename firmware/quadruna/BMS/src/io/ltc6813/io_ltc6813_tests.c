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
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
#define CVST (0x0207U | adc_speed_factor | ST << 5)
    return io_ltc6813_sendCommand(CVST);
}

bool io_ltc6813_sendSelfTestAux(const ADCSpeed speed)
{
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
#define AXST (0x407 | adc_speed_factor | (uint16_t)(ST << 5))
    return io_ltc6813_sendCommand(AXST);
}

bool io_ltc6813_sendSelfTestStat(const ADCSpeed speed)
{
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
#define STATST (0x40F | adc_speed_factor | (uint16_t)(ST << 5))
    return io_ltc6813_sendCommand(STATST);
}

bool io_ltc6813_diagnoseMUX()
{
#define DIAGN (0x0715)
    return io_ltc6813_sendCommand(DIAGN);
}

bool io_ltc6813_overlapADCTest(const ADCSpeed speed)
{
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
#define DCP (0x0)
#define ADOL (0x0201 | DCP << 4 | adc_speed_factor)
    return io_ltc6813_sendCommand(ADOL);
}