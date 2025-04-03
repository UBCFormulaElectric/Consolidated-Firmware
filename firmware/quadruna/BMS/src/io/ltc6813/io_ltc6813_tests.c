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

bool io_ltc6813_sendSelfTestVoltages(void)
{
#define MD (11U)
#define ST (0x0)
#define CVST (0x207 | MD << 7 | ST << 5)
    if (!io_ltc6813_sendCommand(CVST))
    {
        return false;
    }
    return true;
}

bool io_ltc6813_sendSelfTestAux(void)
{
#define MD (11U)
#define ST (0x0)
#define AXST (0x407 | MD << 7 | ST << 5)
    if (!io_ltc6813_sendCommand(AXST))
    {
        return false;
    }
    return true;
}

bool io_ltc6813_sendSelfTestStat(void)
{
#define MD (11U)
#define ST (0x0)
#define STATST (0x40F | MD << 7 | ST << 5)
    if (!io_ltc6813_sendCommand(STATST))
    {
        return false;
    }
    return true;
}
