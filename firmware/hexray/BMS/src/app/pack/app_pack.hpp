#pragma once

#include <array>
#include <bitset>
#include <cstdint>

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

struct Stamped
{
    uint32_t updated_ms = 0U;
    bool     stamped    = false;

    void stamp(const uint32_t now_ms)
    {
        updated_ms = now_ms;
        stamped    = true;
    }
};

struct TractiveStats : Stamped
{
    // everything needed from adbms2950 by other tasks
    // acc_voltage
    // tractive_voltage
    // tractive_current
};

struct ADBMS2950Flags : Stamped
{
    // these are the flags in the registers of the chip
    // whichever ones are relevent for faulting
};

struct VoltStats : Stamped
{
    LocatedValue            max{};
    LocatedValue            min{};
    io::adbms::Cells<float> voltages{};
    CellFlags               valid{}; // bit set == this cell's voltage read succeeded
};

struct TempStats : Stamped
{
    LocatedValue             max{};
    LocatedValue             min{};
    io::adbms::Therms<float> temperatures{};
    ThermFlags               valid{}; // bit set == this thermistor's read succeeded
};

struct OwcStats : Stamped
{
    CellFlags  cells_ok{};
    ThermFlags therms_ok{};
};

//idk what to store here
struct ADBMS6830Flags : Stamped
{
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
    ADBMS6830Flags adbms6830_flags;
    ADBMS2950Flags adbms2950_flags;
};

struct BalancingRequest
{
    RequestKind    kind;
    Cells<uint8_t> duty{};
};

enum class RequestKind {
    MUTE_BALANCING,
    UNMUTE_BALANCING
};

namespace view {
void publish(const Snapshot &snapshot);
[[nodiscard]] Snapshot latest();
}

namespace alerts
{
void init();
bool voltFault(const VoltStats &stats);      // invalid reads, undervoltage, overvoltage
bool tempFault(const TempStats &stats);      // invalid reads, overtemp, undertemp
bool owcFault(const OwcStats &stats);        // cell open wire, thermistor open wire
bool flagFault(const ADBMS6830Flags &stats); // ADBMS6830 uv/ov, therm shutdown, self test, supply
} // namespace alerts

namespace balancing
{
void init();
void disable();
void tick (const VoltStats &volts);
}

namespace sequence 
{
void init();
void tick();
void snapshot();
}


} // namespace app::pack
