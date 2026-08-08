#pragma once

#include <array>
#include <bitset>
#include <cstdint>

#include "util_errorCodes.hpp"
#include "io_adbms.hpp"

namespace app::pack
{

inline constexpr uint32_t MAX_VOLTAGE_AGE_MS = 100U;
inline constexpr uint32_t MAX_TEMP_AGE_MS    = 500U;
inline constexpr uint32_t MAX_OWC_AGE_MS     = 1000U;
inline constexpr uint32_t MAX_FLAGS_AGE_MS   = 500U;
inline constexpr uint32_t MAX_TRACTIVE_AGE_MS = 100U;

using CellFlags    = std::array<std::bitset<CELLS_PER_SEGMENT>, NUM_SEGMENTS>;
using ThermFlags   = std::array<std::bitset<THERMISTORS_PER_SEGMENT>, NUM_SEGMENTS>;
using SegmentFlags = std::bitset<NUM_SEGMENTS>;

struct LocatedValue
{
    float   value   = 0.0f;
    uint8_t segment = 0U;
    uint8_t index   = 0U;
};

template <typename T> class Stamped
{
  private:
    T        value_{};
    uint32_t updated_ms_ = 0U;
    bool     valid_      = false;

  public:
    void publish(const T &v, const uint32_t now_ms)
    {
        value_      = v;
        updated_ms_ = now_ms;
        valid_      = true;
    }

    [[nodiscard]] bool fresh(const uint32_t now_ms, const uint32_t max_age_ms) const
    {
        return valid_ && (now_ms - updated_ms_) <= max_age_ms;
    }

    [[nodiscard]] result<const T *> get(const uint32_t now_ms, const uint32_t max_age_ms) const
    {
        if (!fresh(now_ms, max_age_ms))
        {
            return std::unexpected(ErrorCode::STALE_VALUE);
        }
        return &value_;
    }
};

struct TractiveStats
{
    // everything needed from adbms2950 by other tasks
    // acc_voltage
    // tractive_voltage
    // tractive_current
};

struct ADBMS2950Flags
{
    // these are the flags in the registers of the chip
    // whichever ones are relevent for faulting
};

struct VoltStats
{
    LocatedValue            max{};
    LocatedValue            min{};
    io::adbms::Cells<float> voltages{};
    CellFlags               valid{}; // bit set == this cell's voltage read succeeded
};

struct TempStats
{
    LocatedValue             max{};
    LocatedValue             min{};
    io::adbms::Therms<float> temperatures{};
    ThermFlags               valid{}; // bit set == this thermistor's read succeeded
};

struct OwcStats
{
    CellFlags  cells_ok{};
    ThermFlags therms_ok{};
};

struct ADBMS6830Flags
{
    CellFlags    ov_ok{};         
    CellFlags    uv_ok{};         
    SegmentFlags therm_shdn_ok{}; 
    SegmentFlags self_test_ok{};
    SegmentFlags supply_ok{};
};

struct Snapshot
{
    Stamped<TractiveStats>  tractive_stats;
    Stamped<VoltStats>      voltage_stats;
    Stamped<TempStats>      temperature_stats;
    Stamped<OwcStats>       owc_stats;
    Stamped<ADBMS6830Flags> adbms6830_flags;
    Stamped<ADBMS2950Flags> adbms2950_flags;
};

struct BalancingView
{
    Stamped<VoltStats> voltage_stats;
};

struct SocView
{
    Stamped<TractiveStats> tractive_stats;
    Stamped<VoltStats>     voltage_stats;
    Stamped<TempStats>     temperature_stats;
};

struct TractiveView
{
    Stamped<TractiveStats> tractive_stats;
};

void publish(const Snapshot &snapshot);
[[nodiscard]] BalancingView balancingView();
[[nodiscard]] SocView       socView();
[[nodiscard]] TractiveView  tractiveView();
[[nodiscard]] Snapshot latest();
} // namespace app::pack
