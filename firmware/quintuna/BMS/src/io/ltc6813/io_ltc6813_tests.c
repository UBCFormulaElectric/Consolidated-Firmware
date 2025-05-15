#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"

#define ST (0x2)

ExitCode io_ltc6813_sendSelfTestVoltages(const ADCSpeed speed)
{
    ASSERT_EXIT_OK(io_ltc6813_clearCellRegisters());
    const uint16_t MD_SHIFT_7 = (speed & 0x3) << 7;
#define CVST (0x0207U | MD_SHIFT_7 | ST << 5)
    return io_ltc6813_sendCommand(CVST);
}

ExitCode io_ltc6813_sendSelfTestAux(const ADCSpeed speed)
{
    ASSERT_EXIT_OK(io_ltc6813_clearAuxRegisters());
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
#define AXST (0x407 | adc_speed_factor | (uint16_t)(ST << 5))
    return io_ltc6813_sendCommand(AXST);
}

ExitCode io_ltc6813_sendSelfTestStat(const ADCSpeed speed)
{
    ASSERT_EXIT_OK(io_ltc6813_clearStatRegisters());
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
#define STATST (0x40F | adc_speed_factor | (uint16_t)(ST << 5))
    return io_ltc6813_sendCommand(STATST);
}

uint16_t io_ltc6813_selfTestExpectedValue(const ADCSpeed speed)
{
#if ST == 0x1
    switch (speed)
    {
        case ADCSpeed_27kHz:
            return 0x9565;
        case ADCSpeed_14kHz:
            return 0x9553;
        case ADCSpeed_7kHz:
        case ADCSpeed_3kHz:
        case ADCSpeed_2kHz:
        case ADCSpeed_1kHz:
        case ADCSpeed_422Hz:
        case ADCSpeed_26Hz:
            return 0x9555;
        default:
            assert(false);
    }
#elif ST == 0x2
    switch (speed)
    {
        case ADCSpeed_27kHz:
            return 0x6A9A;
        case ADCSpeed_14kHz:
            return 0x6AAC;
        case ADCSpeed_7kHz:
        case ADCSpeed_3kHz:
        case ADCSpeed_2kHz:
        case ADCSpeed_1kHz:
        case ADCSpeed_422Hz:
        case ADCSpeed_26Hz:
            return 0x6AAA;
        default:
            assert(false);
    }
#else
#error "ST must be 0x1 or 0x2"
#endif
    return 0xffff;
}

ExitCode io_ltc6813_diagnoseMUX()
{
#define DIAGN (0x0715)
    return io_ltc6813_sendCommand(DIAGN);
}

ExitCode io_ltc6813_overlapADCTest(const ADCSpeed speed)
{
    ASSERT_EXIT_OK(io_ltc6813_clearCellRegisters());
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
#define DCP (0x0)
#define ADOL (0x0201 | DCP << 4 | adc_speed_factor)
    return io_ltc6813_sendCommand(ADOL);
}