#pragma once

#include <array>
#include <bitset>
#include <cstdint>

#include "io_semaphore.hpp"
#include "util_errorCodes.hpp"
#include "io_adbms.hpp"

namespace app::pack
{

using PackCellFlags    = std::array<std::bitset<io::adbms::CELLS_PER_SEGMENT>, io::adbms::NUM_SEGMENTS>;
using PackThermFlags   = std::array<std::bitset<io::adbms::THERMISTORS_PER_SEGMENT>, io::adbms::NUM_SEGMENTS>;
using SegmentFlags     = std::bitset<io::adbms::NUM_SEGMENTS>;

struct Reading
{
    float   value   = 0.0f;
    uint8_t segment = 0U;
    uint8_t index   = 0U;
};

struct VoltageBuffer
{
    uint32_t                            updated_ms = 0U;
    Reading                             max{};
    Reading                             min{};
    io::adbms::PackCells<result<float>> voltages{};
};

struct TemperatureBuffer
{
    uint32_t                             updated_ms = 0U;
    Reading                              max{};
    Reading                              min{};
    io::adbms::PackTherms<result<float>> temperatures{};
};

struct OpenWireCheckBuffer
{
    uint32_t   updated_ms = 0U;
    PackCellFlags  cells_owc_ok{};
    PackThermFlags therms_owc_ok{};
};

struct SupplyBuffer
{
    uint32_t                           updated_ms = 0U;
    io::adbms::Segments<result<float>> vmv{};
    io::adbms::Segments<result<float>> vpv{};
    io::adbms::Segments<result<float>> vref{};
    io::adbms::Segments<result<float>> itmp{};
    io::adbms::Segments<result<float>> va{};
    io::adbms::Segments<result<float>> vd{};
    io::adbms::Segments<result<float>> vres{};
};

struct DiagnosticBuffer
{
    uint32_t     updated_ms = 0U;
    PackCellFlags    ov_ok{};
    PackCellFlags    uv_ok{};
    SegmentFlags therm_shdn_ok{};
    SegmentFlags self_test_ok{};
    SegmentFlags supply_ok{};
};

struct BalancingBuffer
{
    bool balancing_enabled = false;
    io::adbms::PackCells<uint8_t> pwm{};
    
    uint32_t updated_ms = 0U;
    io::adbms::PackCells<result<float>> voltages{};
};

inline VoltageBuffer       voltage_buffer{};
inline TemperatureBuffer   temp_buffer{};
inline OpenWireCheckBuffer owc_buffer{};
inline SupplyBuffer        supply_buffer{};
inline DiagnosticBuffer    diag_buffer{};
inline BalancingBuffer     balance_buffer{};
inline const io::Semaphore buffer_lock{ true };

namespace alerts
{
    void init();
    bool tick();
} // namespace alerts

namespace balancing
{
    void init();
    void tick();
    void stop();
} // namespace balancing

namespace sequence
{
    void init();
    void tick();
} // namespace sequence

} // namespace app::pack
