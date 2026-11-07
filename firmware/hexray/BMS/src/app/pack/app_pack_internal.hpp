#pragma once

#include "app_pack.hpp"
#include "io_adbms.hpp"

inline constexpr float V_FAULT_UV = 2.5f;
inline constexpr float V_FAULT_OV = 4.2f;

inline constexpr uint32_t TEMP_PERIOD_MS    = 1000;
inline constexpr uint32_t COWC_PERIOD_MS    = 5 * 1000;
inline constexpr uint32_t SOWC_PERIOD_MS    = 5 * 1000;
inline constexpr uint32_t BALANCE_PERIOD_MS = 60 * 1000;
inline constexpr uint32_t SETTLE_MS         = 10 * 1000;

namespace app::pack
{

namespace config
{
    [[nodiscard]] result<void> setSegmentConfig(io::adbms::ThermistorMux mux);
    [[nodiscard]] result<void> setPWMConfig(const io::adbms::PackCells<uint8_t> &duty);
    [[nodiscard]] result<void> checkSegmentConfig();
    [[nodiscard]] result<void> checkPWMConfig();
} // namespace config

namespace owc
{
    [[nodiscard]] OpenWireCheckBuffer check(
        const io::adbms::PackCells<result<float>> &odd_voltages,
        const io::adbms::PackCells<result<float>> &even_voltages);
    [[nodiscard]] ThermOpenWireCheckBuffer checkTherms(const io::adbms::PackTherms<result<float>> &therm_voltages);
} // namespace owc

namespace broadcast
{
    void cellVoltages(const VoltageBuffer &stats);
    void cellTemps(const TemperatureBuffer &stats);
    void cadcOpenWire(const OpenWireCheckBuffer &owc);
    void sadcOpenWire(const OpenWireCheckBuffer &owc);
    void thermOpenWire(const ThermOpenWireCheckBuffer &owc);
    void adbmsFlags(const DiagnosticBuffer &diag);
    void adbmsSupply(const SupplyBuffer &supply);
    void balancing(const io::adbms::PackCells<uint8_t> &duty);
} // namespace broadcast

} // namespace app::pack
