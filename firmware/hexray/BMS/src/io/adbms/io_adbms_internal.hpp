#pragma once
#include <stdint.h>

namespace io::adbms
{
    // Write configuration register commands.
    constexpr uint16_t WRCFGA = 0x0001;
    constexpr uint16_t WRCFGB = 0x0024;

    // Read configuration register commands.
    constexpr uint16_t RDCFGA = 0x0002;
    constexpr uint16_t RDCFGB = 0x0026;

    // Read cell voltage register commands.
    constexpr uint16_t RDCVA = 0x04U;
    constexpr uint16_t RDCVB = 0x06U;
    constexpr uint16_t RDCVC = 0x08U;
    constexpr uint16_t RDCVD = 0x0AU;
    constexpr uint16_t RDCVE = 0x09U;

    // Read auxillary register commands.
    constexpr uint16_t RDAUXA = 0x0019U;
    constexpr uint16_t RDAUXB = 0x001AU;
    constexpr uint16_t RDAUXC = 0x001BU;
    constexpr uint16_t RDAUXD = 0x001FU;
}