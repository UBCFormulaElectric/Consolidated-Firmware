#include "test_fakes.h"

extern "C"
{
#include "io_ltc6813.h"
    static bool state = false;
    void        io_ltc6813_writeConfigurationRegisters_setOutput(bool output)
    {
        state = output;
    }

    static bool startCellAdcConv = false;
    void        io_ltc6813_startCellsAdcConversion_setOutput(bool output)
    {
        startCellAdcConv = output;
    }

    void io_ltc6813_readVoltages(
        float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT],
        bool  success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS])
    {
    }

    static bool startThermAdcConv = false;
    void        io_ltc6813_startThermistorsAdcConversion_setOutput(bool output)
    {
        startThermAdcConv = output;
    }

    void io_ltc6813_readTemperatures(
        float  cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT],
        float *vref,
        bool   success[NUM_SEGMENTS][AUX_REG_GROUPS])
    {
    }

#include "io_irs.h"
    static bool positiveClosed = false;
    void        io_irs_setPositive(const bool closed)
    {
        positiveClosed = closed;
    }
    bool io_irs_isPositiveClosed(void)
    {
        return positiveClosed;
    }

    static bool prechargeClosed = false;
    void        io_irs_setPrecharge(const bool closed)
    {
        prechargeClosed = closed;
    }
    bool io_irs_isPrechargeClosed(void)
    {
        return prechargeClosed;
    }

    static bool negativeClosed = false;
    bool        io_irs_isNegativeClosed(void)
    {
        return negativeClosed;
    }

#include "io_tractiveSystem.h"
    void io_tractiveSystem_init(void) {}

    static float tractiveSystemVoltage = 0.0f;
    float        io_tractiveSystem_getVoltage(void)
    {
        return tractiveSystemVoltage;
    }

    static float currentHighResolution = 0.0f, currentLowResolution = 0.0f;
    float        io_tractiveSystem_getCurrentHighResolution(void)
    {
        return currentHighResolution;
    }
    float io_tractiveSystem_getCurrentLowResolution(void)
    {
        return currentLowResolution;
    }
}

namespace fakes
{
namespace irs
{
    void setNegativeClosed(const bool closed)
    {
        negativeClosed = closed;
    }
} // namespace irs

namespace tractiveSystem
{
    void setVoltage(const float voltage)
    {
        tractiveSystemVoltage = voltage;
    }
    void setCurrentHighResolution(const float current)
    {
        currentHighResolution = current;
    }
    void setCurrentLowResolution(const float current)
    {
        currentLowResolution = current;
    }
} // namespace tractiveSystem
} // namespace fakes
