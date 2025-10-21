#include "app/utils.h"
#include "io/ltc6813.h"

#include "io/ltc6813_internal.h"

ExitCode io_ltc6813_sendSelfTestVoltages(void)
{
    RETURN_IF_ERR(io_ltc6813_clearCellRegisters());
    return io_ltc6813_sendCommand(CVST);
}

ExitCode io_ltc6813_sendSelfTestAux(void)
{
    RETURN_IF_ERR(io_ltc6813_clearAuxRegisters());
    return io_ltc6813_sendCommand(AXST);
}

ExitCode io_ltc6813_sendSelfTestStat(void)
{
    return io_ltc6813_sendCommand(STATST);
}

ExitCode io_ltc6813_diagnoseMUX(void)
{
    return io_ltc6813_sendCommand(DIAGN);
}

ExitCode io_ltc6813_overlapADCTest(void)
{
    RETURN_IF_ERR(io_ltc6813_clearCellRegisters());
    return io_ltc6813_sendCommand(ADOL);
}
