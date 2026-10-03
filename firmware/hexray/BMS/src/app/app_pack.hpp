#pragma once

#include <array>
#include <bitset>
#include <cstdint>

#include "app_channel.hpp"
#include "util_errorCodes.hpp"
#include "io_adbms.hpp"

namespace app::pack
{

using CellFlags    = std::array<std::bitset<CELLS_PER_SEGMENT>, NUM_SEGMENTS>;
using ThermFlags   = std::array<std::bitset<THERMISTORS_PER_SEGMENT>, NUM_SEGMENTS>;
using SegmentFlags = std::bitset<NUM_SEGMENTS>;

struct LocatedValue
{
    float   value   = 0.0f;
    uint8_t segment = 0U;
    uint8_t index   = 0U;
};

struct TractiveStats
{
    uint32_t updated_ms = 0U;
    // everything needed from adbms2950 by other tasks
    // acc_voltage
    // tractive_voltage
    // tractive_current
};

struct ADBMS2950Diag
{
    uint32_t updated_ms = 0U;
    // these are the flags in the registers of the chip
    // whichever ones are relevent for faulting
};

struct VoltStats
{
    uint32_t                updated_ms = 0U;
    LocatedValue            max{};
    LocatedValue            min{};
    io::adbms::Cells<float> voltages{};
    CellFlags               valid{};
};

struct TempStats
{
    uint32_t                 updated_ms = 0U;
    LocatedValue             max{};
    LocatedValue             min{};
    io::adbms::Therms<float> temperatures{};
    ThermFlags               valid{};
};

struct OwcStats
{
    uint32_t   updated_ms = 0U;
    CellFlags  cells_ok{};
    ThermFlags therms_ok{};
};

//idk what to store here
struct ADBMS6830Diag
{
    uint32_t     updated_ms = 0U;
    CellFlags    ov_ok{};
    CellFlags    uv_ok{};
    SegmentFlags therm_shdn_ok{};
    SegmentFlags self_test_ok{};
    SegmentFlags supply_ok{};
};

struct Snapshot
{
    TractiveStats  tractive_stats;
    VoltStats      voltage_stats;
    TempStats      temperature_stats;
    OwcStats       owc_stats;
    ADBMS6830Diag adbms6830_diag;
    ADBMS2950Diag adbms2950_diag;
};

struct Request
{
    bool start_balance = false;
    io::adbms::Cells<uint8_t> duty{};
};

inline constexpr size_t CHANNEL_DEPTH       = 2;
inline constexpr size_t CHANNEL_SUBSCRIBERS = 2;
template <typename T> using PackChannel = Channel<T, CHANNEL_DEPTH, CHANNEL_SUBSCRIBERS>;

inline PackChannel<VoltStats>     voltage_channel{};
inline PackChannel<TempStats>     temperature_channel{};
inline PackChannel<OwcStats>      owc_channel{};
inline PackChannel<ADBMS6830Diag> diag_channel{};

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
}

namespace sequence 
{
void init();
void tick();
}

namespace broadcast
{
void cellVoltages(const VoltStats &stats);
void cellTemps(const TempStats &stats);
void cellOpenWire(const OwcStats &stats);
void adbmsFlags(const ADBMS6830Diag &diag);
void balancing(const io::adbms::Cells<uint8_t> &duty);
} // namespace broadcast


} // namespace app::pack
