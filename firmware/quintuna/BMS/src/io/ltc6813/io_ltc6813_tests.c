#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"

ExitCode io_ltc6813_sendSelfTestVoltages(void)
{
    return io_ltc6813_sendCommand(CVST);
}

ExitCode io_ltc6813_sendSelfTestAux(void)
{
    return io_ltc6813_sendCommand(AXST);
}

ExitCode io_ltc6813_sendSelfTestStat(void)
{
    return io_ltc6813_sendCommand(STATST);
}

uint16_t io_ltc6813_selfTestExpectedValue(void)
{
    // See Table 13.
    return 0x6AAA;
}

ExitCode io_ltc6813_diagnoseMUX(void)
{
    return io_ltc6813_sendCommand(DIAGN);
}

ExitCode io_ltc6813_overlapADCTest(void)
{
    ASSERT_EXIT_OK(io_ltc6813_clearCellRegisters());
    return io_ltc6813_sendCommand(ADOL);
}
