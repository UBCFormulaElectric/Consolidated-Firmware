#pragma once

#include "app_pack.hpp"
#include "io_adbms.hpp"

extern const float V_FAULT_UV;
extern const float V_FAULT_OV;

namespace app::pack::config
{
[[nodiscard]] result<void> setSegmentConfig(io::adbms::ThermistorMux mux);
[[nodiscard]] result<void> setPWMConfig(const io::adbms::PackCells<uint8_t> &duty);
[[nodiscard]] result<void> checkSegmentConfig();
[[nodiscard]] result<void> checkPWMConfig();
} // namespace app::pack::config

namespace app::pack::owc
{
[[nodiscard]] OpenWireCheckBuffer check(const io::adbms::PackCells<result<float>>  &odd_voltages,
                                        const io::adbms::PackCells<result<float>>  &even_voltages,
                                        const io::adbms::PackTherms<result<float>> &therm_voltages);
} // namespace app::pack::owc
