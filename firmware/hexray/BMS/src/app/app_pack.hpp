#pragma once

#include <array>
#include <bitset>
#include <cstdint>

#include "io_semaphore.hpp"
#include "util_errorCodes.hpp"
#include "io_adbms.hpp"

namespace app::pack
{

using CellFlags    = std::array<std::bitset<io::adbms::CELLS_PER_SEGMENT>, io::adbms::NUM_SEGMENTS>;
using ThermFlags   = std::array<std::bitset<io::adbms::THERMISTORS_PER_SEGMENT>, io::adbms::NUM_SEGMENTS>;
using SegmentFlags = std::bitset<io::adbms::NUM_SEGMENTS>;

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
    CellFlags  cells_owc_ok{};
    ThermFlags therms_owc_ok{};
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
    CellFlags    ov_ok{};
    CellFlags    uv_ok{};
    SegmentFlags therm_shdn_ok{};
    SegmentFlags self_test_ok{};
    SegmentFlags supply_ok{};
};

inline VoltageBuffer       voltage_buffer{};
inline TemperatureBuffer   temp_buffer{};
inline OpenWireCheckBuffer owc_buffer{};
inline SupplyBuffer        supply_buffer{};
inline DiagnosticBuffer    diag_buffer{};
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

    [[nodiscard]] Request getRequest();
    void                  setRequest(const Request &r);
} // namespace balancing

namespace sequence
{
    void init();
    void tick();
} // namespace sequence

namespace broadcast
{
    void cellVoltages(const VoltStats &stats);
    void cellTemps(const TempStats &stats);
    void cellOpenWire(const OpenWireCheckBuffer &owc);
    void adbmsFlags(const ADBMS6830Diag &diag);
    void balancing(const io::adbms::PackCells<uint8_t> &duty);
} // namespace broadcast

} // namespace app::pack
