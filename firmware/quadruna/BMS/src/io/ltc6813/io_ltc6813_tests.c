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

bool io_ltc6813_sendSelfTestVoltages(const ADCSpeed speed)
{
#define ST (0x0)
#define CVST (0x207 | (speed & 0x3) << 7 | ST << 5)
    if (!io_ltc6813_sendCommand(CVST))
    {
        return false;
    }
    return true;
}

bool io_ltc6813_sendSelfTestAux(const ADCSpeed speed)
{
#define ST (0x0)
#define AXST (0x407 | (speed & 0x3) << 7 | ST << 5)
    if (!io_ltc6813_sendCommand(AXST))
    {
        return false;
    }
    return true;
}

bool io_ltc6813_sendSelfTestStat(const ADCSpeed speed)
{
#define ST (0x0)
#define STATST (0x40F | (speed & 0x3) << 7 | ST << 5)
    if (!io_ltc6813_sendCommand(STATST))
    {
        return false;
    }
    return true;
}
