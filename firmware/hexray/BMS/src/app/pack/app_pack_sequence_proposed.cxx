// ============================================================================================
// PROPOSED app_pack_sequence.cpp — review copy, not in the build.
//
// Named .cxx on purpose: CMakeLists globs src/app/*.cpp recursively, so a second .cpp defining
// app::pack::sequence::{init,tick} would be a duplicate-symbol link error. Rename to .cpp (and
// delete the original) when you want to adopt it.
// ============================================================================================
//
// DESIGN
//
// A hierarchical state machine. An outer table-driven scheduler decides *which* machine gets
// serviced this tick; four inner machines decide *what* that machine does next. Every phase is
// a named state and every transition is an explicit return value — there are no status booleans
// carrying hidden phase.
//
// Each inner machine is a pure transition function:
//
//     [[nodiscard]] XStepState stepX(XStepState current);   // performs the state's action,
//     ...                                                   // returns the next state
//     x_step_state = stepX(x_step_state);
//
// The [[nodiscard]] return is the point: you cannot fall out of a case without naming where you
// go, so every edge in the diagrams below is forced to exist in the code.
//
// Machines own *control state* (the enums). Measurement buffers, retry counters and the pending
// balancing request are *extended state* — data the machine carries between states, kept next to
// the machine it belongs to.
//
// --------------------------------------------------------------------------------------------
// WHY IT NEVER BLOCKS
//
// Every state does at most one group of bus transactions and then returns. Nothing calls
// io::time::delay() and nothing polls blocking. Where a blocking design would sit on a
// conversion, this one *pipelines*: a state starts the hardware, and a later state reads it back
// on a subsequent visit. The gap between visits is the conversion time, and it is time the RTOS
// gets back.
//
// That is why the states come in START_/READ_ pairs, and why the mux settle costs a state rather
// than a delay.
//
// --------------------------------------------------------------------------------------------
// WHY THE TWO ADC BANKS
//
//   C-ADC   cell voltages, free-running. One ADCV(CONT=1) at startup and never again; the
//           registers are never more than 1 ms stale, so READ_VOLTAGES is a pure register read.
//   S-ADC   open wire, odd and even passes. ADSV only — it does not disturb a running C-ADC, so
//           cell voltages keep updating straight through an open-wire check. Nothing below ever
//           issues ADCV after init, because ADCV restarts the C-ADCs and resets the averaging
//           window.
//   AUX     thermistors, two external mux banks, one bank started at a time.
//
// PWM discharge runs underneath all of it. The only thing that mutes it is the DCP=0 open-wire
// conversion, ~8 ms twice per full cycle, and the chip re-enables discharge itself afterwards.
// Balancing loses well under 5% duty, versus the 72% ceiling the datasheet quotes for running
// the full diagnostic sequence every 100 ms FTTI.
//
// --------------------------------------------------------------------------------------------
// OUTER SCHEDULER — SCHEDULE[] below, one entry per tick, wrapping
//
//   Cells, Therms, Cells, Therms, Cells, OwcOdd, Cells, OwcEven, Cells, Config, Cells, Flags
//
//   cell voltages    every 2 ticks   (100 ms)   — safety critical, so it gets every other slot
//   every other      every 12 ticks  (600 ms)
//   full therm set   4 therm visits             (1.2 s, two mux banks)
//
//   Retuning cadence means editing the table, nothing else.
//
// --------------------------------------------------------------------------------------------
// DEPENDS ON, outside this file:
//
//   1. io_adbms.hpp + io_adbms/io_adbms_cells.cpp — one new command:
//
//        result<void> command::startCellsAdcContinuous(bool reset_filter)
//            { uint16_t c = (ADCV_BASE & ~DCP) | CONT; if (reset_filter) c |= RSTF; return sendCmd(c); }
//
//      Load-bearing. ADCV_BASE is 0x0270, which already has DCP=1, and Table 19 lists ADCV with
//      DCP=1 and CONT=1 as "Invalid command is ignored, CC not incremented". So
//      `startCellsAdc() | CONT` silently does nothing and READ_VOLTAGES would sit reading stale
//      registers forever. DCP must be cleared for continuous mode — which is also what leaves
//      PWM discharge running (RD=0/DCP=0 → "PWM controlled discharge remains unaffected").
//
//   2. app_pack.hpp — fixes the current header needs regardless of this rewrite:
//        - move `enum class RequestKind` above `struct BalancingRequest` (used before declared)
//        - `Cells<uint8_t> duty` → `io::adbms::Cells<uint8_t> duty` (unqualified name)
//        - `void snapshot();` → `[[nodiscard]] const Snapshot &snapshot();`
//          (jobs.cpp:163 already calls it as `view::publish(sequence::snapshot())`)
//        - add:  namespace requests { void post(const BalancingRequest &request); }
//          (app_pack_balancing.cpp calls requests::post; nothing defines it)
//
//   3. app_pack_internal.hpp — optional tidy. The mux bank now lives in ThermStepState, so
//      Step::ThermsMux1/ThermsMux2 no longer name anything real; both just service the therm
//      machine. Collapsing them to a single `Step::Therms` listed twice in SCHEDULE reads
//      better. Left as-is here so this file drops onto your existing enum.
// ============================================================================================

#include "app_pack.hpp"
#include "app_pack_internal.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>

#include "app_thermistors.hpp"
#include "io_log.hpp"
#include "io_time.hpp"

using io::adbms::Cells;
using io::adbms::OpenWireSwitch;
using io::adbms::Segments;
using io::adbms::ThermGpios;
using io::adbms::Therms;

namespace
{
// ============================================================================================
// Tuning
// ============================================================================================

// Consecutive all-segments-failed reads before a machine declares the chain lost and re-arms.
constexpr uint8_t MAX_CONSECUTIVE_READ_FAILURES = 3U;
constexpr uint8_t MAX_CONFIG_WRITE_ATTEMPTS     = 3U;

// ============================================================================================
// Conversions
// ============================================================================================

constexpr float V_LSB    = 150e-6f; // every 16-bit result register shares this scaling
constexpr float V_OFFSET = 1.5f;

constexpr float V_REF2   = 3.0f;  // thermistor divider reference
constexpr float R_SERIES = 10e3f; // fixed leg of the divider

// A cell whose C-pin is open has its input capacitor drained by the open-wire current source, so
// the pulled reading collapses relative to the un-pulled one.
constexpr float OW_CELL_DELTA_V = 1.0f;
// An open thermistor leg floats the divider up toward VREF2.
constexpr float OW_THERM_MAX_V = 2.8f;

constexpr float convertRegToVoltage(const int16_t reg)
{
    return static_cast<float>(reg) * V_LSB + V_OFFSET;
}

float convertVoltageToTemp(const float voltage)
{
    // Guard the divider: at or above VREF2 the thermistor leg reads as infinite resistance.
    if (voltage >= V_REF2 || voltage <= 0.0f)
        return -1.0f;

    const float resistance = R_SERIES * (voltage / (V_REF2 - voltage));
    return app::therm::adbms_ntc10k_lut.resistanceToTemp(resistance);
}

// The snapshot every machine writes into. Handed out by reference; jobs.cpp copies it under the
// view lock so readers on other tasks get a coherent version.
app::pack::Snapshot local{};

// ============================================================================================
// CELL VOLTAGE MACHINE — continuous C-ADC
//
//        ┌──────────────────┐  ADCV ok   ┌────────────────┐
//   ────►│ START_CONTINUOUS ├───────────►│ READ_VOLTAGES  │◄─┐
//        └────────┬─────────┘            └───────┬────────┘  │ read ok
//                 ▲    │ ADCV failed             │           │
//                 │    └─────────────────────────┼───────────┘
//                 │                              │ N consecutive total read failures
//                 │        ┌─────────┐           │
//                 └────────┤ RECOVER │◄──────────┘
//                          └─────────┘
//
// Steady state is READ_VOLTAGES → READ_VOLTAGES: one register read, no ADCV, no PLCADC, no wait.
// The chip has been converting at 1 kHz since START_CONTINUOUS and the results are never >1 ms
// old. RECOVER is its own state so "the chain died and we re-armed it" is a visible, loggable
// event rather than a counter quietly wrapping.
// ============================================================================================

enum class CellStepState : uint8_t
{
    START_CONTINUOUS,
    READ_VOLTAGES,
    RECOVER,
};

CellStepState cell_step_state    = CellStepState::START_CONTINUOUS;
uint8_t       cell_read_failures = 0U;

[[nodiscard]] CellStepState stepCells(const CellStepState current)
{
    switch (current)
    {
        case CellStepState::START_CONTINUOUS:
        {
            // Issued once. RSTF clears the IIR filters so post-restart samples aren't blended
            // with pre-fault history.
            if (const auto res = io::adbms::command::startCellsAdcContinuous(true); !res)
            {
                LOG_WARN("ADCV(CONT) failed: %s", error_code_to_string(res.error()));
                return CellStepState::START_CONTINUOUS; // retry on the next Cells slot
            }
            cell_read_failures = 0U;
            return CellStepState::READ_VOLTAGES;
        }

        case CellStepState::READ_VOLTAGES:
        {
            const Cells<result<int16_t>> raw = io::adbms::read::cellVoltage();

            app::pack::LocatedValue min{ .value = std::numeric_limits<float>::infinity() };
            app::pack::LocatedValue max{ .value = -std::numeric_limits<float>::infinity() };
            bool                    any_valid = false;

            for (uint8_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
            {
                for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; ++cell)
                {
                    const result<int16_t> &reg = raw[seg][cell];
                    if (!reg)
                    {
                        local.voltage_stats.valid[seg].reset(cell);
                        continue;
                    }

                    const float volts = convertRegToVoltage(reg.value());

                    local.voltage_stats.voltages[seg][cell] = volts;
                    local.voltage_stats.valid[seg].set(cell);
                    any_valid = true;

                    if (volts < min.value)
                        min = { .value = volts, .segment = seg, .index = cell };
                    if (volts > max.value)
                        max = { .value = volts, .segment = seg, .index = cell };
                }
            }

            if (!any_valid)
            {
                // One bad visit is noise. A run of them means the chain reset or slept, either of
                // which drops it out of continuous mode.
                if (++cell_read_failures >= MAX_CONSECUTIVE_READ_FAILURES)
                    return CellStepState::RECOVER;
                return CellStepState::READ_VOLTAGES;
            }

            cell_read_failures      = 0U;
            local.voltage_stats.min = min;
            local.voltage_stats.max = max;
            local.voltage_stats.stamp(io::time::getCurrentMs());
            return CellStepState::READ_VOLTAGES;
        }

        case CellStepState::RECOVER:
        {
            LOG_WARN("no valid cell reads in %u visits, restarting C-ADCs", cell_read_failures);
            local.voltage_stats.stamped = false; // stop consumers trusting the last good sweep
            return CellStepState::START_CONTINUOUS;
        }
    }
    return CellStepState::START_CONTINUOUS;
}

// NOTE on redundancy: the chip can cross-check C-ADC against S-ADC itself and report per-cell
// mismatches in CSxFLT, via ADSV with DCP=0/CONT=1. Deliberately not wired up, because that
// command does not merely mute discharge for the conversion — Table 19 says "The PWM controlled
// discharge is stopped", and with CONT=1 the S-ADCs never finish, so it stays stopped until an
// explicit ADSV with DCP=1/CONT=0 ends it. If you want CSxFLT, add START_REDUNDANT /
// WAIT_REDUNDANT / READ_REDUNDANT plus a RESUME_DISCHARGE terminator to the open-wire machine
// below, where the S-ADC is already committed and discharge is already muted.
// (WAIT, not POLL: an ADC in continuous mode never reports done, so PLSADC will never come back
// ready for that phase. The two CONT=0 open-wire passes are pollable; the redundant one is not.)

// ============================================================================================
// THERMISTOR MACHINE — AUX ADC behind an external 2:1 mux
//
//  ┌────────────────┐  ┌───────────────┐  ┌──────────────┐  ┌────────────────┐  ┌───────────────┐
// ►│ MUX_0_7_SELECT ├─►│ MUX_0_7_START ├─►│ MUX_0_7_READ ├─►│ MUX_8_13_START ├─►│ MUX_8_13_READ │
//  └────────────────┘  └───────▲───────┘  └──────────────┘  └────────────────┘  └───────┬───────┘
//                              └──────────────────────────────────────────────────────────┘
//
//   MUX_x_SELECT   entry only: point the mux at that bank. Nothing to read back yet.
//   MUX_x_START    start ADAX. The mux was pointed here a state ago, so it has settled.
//   MUX_x_READ     read the AUX registers, then point the mux at the *other* bank.
//
// The bank lives in the state name rather than in a variable, so "which bank is armed" is never
// ambiguous. The settle is the gap between MUX_0_7_READ and MUX_8_13_START — one scheduler slot,
// ~600 ms, versus the ~20 ms the RC actually needs. The conversion likewise has a full slot to
// finish, so there is nothing to poll.
//
// Deliberately not using command::pollAuxAdc(): it spins with io::time::delay() between retries,
// which is exactly the blocking behaviour this design exists to remove.
// ============================================================================================

enum class ThermStepState : uint8_t
{
    MUX_0_7_SELECT,
    MUX_0_7_START,
    MUX_0_7_READ,
    MUX_8_13_START,
    MUX_8_13_READ,
};

ThermStepState therm_step_state = ThermStepState::MUX_0_7_SELECT;

// Extended state: a full 14-thermistor set spans two banks and therefore two reads. Built up here
// and copied into the snapshot only when both halves are fresh, so consumers never see half of
// one sweep stitched onto half of the previous one.
Therms<float>         therm_scratch{};
app::pack::ThermFlags therm_scratch_valid{};
app::pack::ThermFlags therm_scratch_owc_ok{};
bool                  therm_bank_0_7_seen = false;

// Reads the AUX registers into the half of the scratch buffer belonging to `bank`.
void readThermBank(const ThermMux bank)
{
    const ThermGpios<result<int16_t>> raw = io::adbms::read::thermGpioVoltage();

    const size_t base = (bank == ThermMux::MUX_8_13) ? THERM_GPIOS_PER_SEGMENT : 0U;

    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        for (uint8_t gpio = 0U; gpio < THERM_GPIOS_PER_SEGMENT; ++gpio)
        {
            const size_t therm = base + gpio;
            if (therm >= THERMISTORS_PER_SEGMENT)
                continue; // the 8_13 bank only populates 6 of its 8 channels

            const result<int16_t> &reg = raw[seg][gpio];
            if (!reg)
            {
                therm_scratch_valid[seg].reset(therm);
                therm_scratch_owc_ok[seg].reset(therm);
                continue;
            }

            const float voltage = convertRegToVoltage(reg.value());

            therm_scratch[seg][therm] = convertVoltageToTemp(voltage);
            therm_scratch_valid[seg].set(therm);
            therm_scratch_owc_ok[seg].set(therm, voltage < OW_THERM_MAX_V);
        }
    }
}

void publishTemperatures()
{
    local.temperature_stats.temperatures = therm_scratch;
    local.temperature_stats.valid        = therm_scratch_valid;
    local.owc_stats.therms_ok            = therm_scratch_owc_ok;

    app::pack::LocatedValue min{ .value = std::numeric_limits<float>::infinity() };
    app::pack::LocatedValue max{ .value = -std::numeric_limits<float>::infinity() };
    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        for (uint8_t therm = 0U; therm < THERMISTORS_PER_SEGMENT; ++therm)
        {
            if (!therm_scratch_valid[seg].test(therm))
                continue;
            const float temp = therm_scratch[seg][therm];
            if (temp < min.value)
                min = { .value = temp, .segment = seg, .index = therm };
            if (temp > max.value)
                max = { .value = temp, .segment = seg, .index = therm };
        }
    }
    local.temperature_stats.min = min;
    local.temperature_stats.max = max;
    local.temperature_stats.stamp(io::time::getCurrentMs());
}

[[nodiscard]] ThermStepState stepTherms(const ThermStepState current)
{
    switch (current)
    {
        case ThermStepState::MUX_0_7_SELECT:
        {
            if (const auto res = app::pack::config::setThermMuxConfig(ThermMux::MUX_0_7); !res)
            {
                LOG_WARN("therm mux select 0_7 failed: %s", error_code_to_string(res.error()));
                return ThermStepState::MUX_0_7_SELECT;
            }
            return ThermStepState::MUX_0_7_START;
        }

        case ThermStepState::MUX_0_7_START:
        {
            if (const auto res = io::adbms::command::startAuxAdc(); !res)
            {
                LOG_WARN("ADAX (bank 0_7) failed: %s", error_code_to_string(res.error()));
                return ThermStepState::MUX_0_7_START; // mux is still on 0_7, just retry
            }
            return ThermStepState::MUX_0_7_READ;
        }

        case ThermStepState::MUX_0_7_READ:
        {
            readThermBank(ThermMux::MUX_0_7);
            therm_bank_0_7_seen = true;

            // Point the mux at 8_13. It settles over the gap to MUX_8_13_START.
            if (const auto res = app::pack::config::setThermMuxConfig(ThermMux::MUX_8_13); !res)
            {
                LOG_WARN("therm mux select 8_13 failed: %s", error_code_to_string(res.error()));
                // Mux is still on 0_7, so re-arm that rather than converting the wrong bank.
                return ThermStepState::MUX_0_7_START;
            }
            return ThermStepState::MUX_8_13_START;
        }

        case ThermStepState::MUX_8_13_START:
        {
            if (const auto res = io::adbms::command::startAuxAdc(); !res)
            {
                LOG_WARN("ADAX (bank 8_13) failed: %s", error_code_to_string(res.error()));
                return ThermStepState::MUX_8_13_START;
            }
            return ThermStepState::MUX_8_13_READ;
        }

        case ThermStepState::MUX_8_13_READ:
        {
            readThermBank(ThermMux::MUX_8_13);

            // Only publish once bank 0_7 has been read at least once, otherwise the first sweep
            // would ship a snapshot that is half real data and half zeroes.
            if (therm_bank_0_7_seen)
                publishTemperatures();

            if (const auto res = app::pack::config::setThermMuxConfig(ThermMux::MUX_0_7); !res)
            {
                LOG_WARN("therm mux select 0_7 failed: %s", error_code_to_string(res.error()));
                return ThermStepState::MUX_8_13_START;
            }
            return ThermStepState::MUX_0_7_START;
        }
    }
    return ThermStepState::MUX_0_7_SELECT;
}

// ============================================================================================
// OPEN WIRE MACHINE — S-ADC, odd and even passes
//
//        ┌──────────────┐      ┌─────────────┐      ┌──────────────┐
//   ────►│ START_OW_ODD ├─────►│ READ_OW_ODD ├─────►│ READ_OW_EVEN ├──┐
//        └──────────────┘      └──────▲──────┘      └──────┬───────┘  │
//                                     │                    │ evaluate │
//                                     └────────────────────┴──────────┘
//
//   START_OW_ODD   entry only: fire the odd pass. Nothing to read back yet.
//   READ_OW_ODD    read the odd pass, then fire the even one.
//   READ_OW_EVEN   read the even pass, compare both, then fire the odd one again.
//
// Having START_OW_ODD as a distinct entry state is what keeps the first cycle honest — without
// it the first read returns whatever was left in the S registers and reports it as a measurement.
//
// Odd and even are separate passes on purpose: the datasheet is explicit that activating the
// differential switches on two neighbouring channels at once creates a divider between the two
// cells that can *mask* a break in the wire they share.
//
// Runs on ADSV only, so the C-ADCs are never interrupted and cell voltages keep updating
// throughout. Each ADSV is a single shot with DCP=0: discharge is muted for the ~8 ms conversion
// and the chip re-enables it on its own afterwards, so there is no explicit unmute to forget.
// (That is only true because CONT=0. See the redundancy note under the cell machine.)
// ============================================================================================

enum class OwcStepState : uint8_t
{
    START_OW_ODD,
    READ_OW_ODD,
    READ_OW_EVEN,
};

OwcStepState owc_step_state = OwcStepState::START_OW_ODD;

// Extended state, indexed by OpenWire: the S-ADC reading taken while that parity was pulled.
std::array<Cells<float>, static_cast<size_t>(OpenWire::COUNT)>         owc_voltages{};
std::array<app::pack::CellFlags, static_cast<size_t>(OpenWire::COUNT)> owc_valid{};

void readSecondaryVoltages(const OpenWire slot)
{
    const Cells<result<int16_t>> raw = io::adbms::read::secondaryCellVoltage();

    Cells<float>         &dest  = owc_voltages[static_cast<size_t>(slot)];
    app::pack::CellFlags &valid = owc_valid[static_cast<size_t>(slot)];

    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; ++cell)
        {
            if (!raw[seg][cell])
            {
                valid[seg].reset(cell);
                continue;
            }
            dest[seg][cell] = convertRegToVoltage(raw[seg][cell].value());
            valid[seg].set(cell);
        }
    }
}

// The signal is the difference between the pass that pulled a given cell's channel and the pass
// that didn't. A broken C-pin shows up as a collapsed reading on the pulled pass, and it
// implicates both cells sharing that pin.
void evaluateOpenWire()
{
    constexpr size_t NUM_C_PINS = CELLS_PER_SEGMENT + 1U;

    const auto &odd        = owc_voltages[static_cast<size_t>(OpenWire::ODD)];
    const auto &even       = owc_voltages[static_cast<size_t>(OpenWire::EVEN)];
    const auto &odd_valid  = owc_valid[static_cast<size_t>(OpenWire::ODD)];
    const auto &even_valid = owc_valid[static_cast<size_t>(OpenWire::EVEN)];

    for (uint8_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
    {
        std::array<float, CELLS_PER_SEGMENT> delta{};
        std::array<bool, CELLS_PER_SEGMENT>  delta_valid{};

        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; ++cell)
        {
            if (!odd_valid[seg].test(cell) || !even_valid[seg].test(cell))
            {
                local.owc_stats.cells_ok[seg].reset(cell); // couldn't test it, don't claim it's fine
                continue;
            }

            // Even-numbered cells sit on the even channels, so the even pass is their pulled read.
            const bool  even_cell = (cell % 2U) == 0U;
            const float pulled    = even_cell ? even[seg][cell] : odd[seg][cell];
            const float unpulled  = even_cell ? odd[seg][cell] : even[seg][cell];

            delta[cell]       = pulled - unpulled;
            delta_valid[cell] = true;
            local.owc_stats.cells_ok[seg].set(cell); // provisional, cleared below on a hit
        }

        // One open C-pin drags the cell below it and the cell above it. Walk the deltas and mark
        // every pin implicated by a collapse.
        std::array<bool, NUM_C_PINS> pin_open{};
        for (size_t cell = 0U; cell + 1U < CELLS_PER_SEGMENT; ++cell)
        {
            if (!delta_valid[cell] || !delta_valid[cell + 1U])
                continue;
            if (delta[cell] > OW_CELL_DELTA_V)
            {
                pin_open[cell] = true;
                if (delta[cell] - delta[cell + 1U] <= -OW_CELL_DELTA_V)
                    pin_open[cell + 1U] = true;
            }
        }

        // The outermost pins have no neighbour to difference against; a rail-pinned reading is
        // the only tell.
        if (even_valid[seg].test(0U) && even[seg][0] <= convertRegToVoltage(0))
            pin_open[0] = true;
        if (odd_valid[seg].test(CELLS_PER_SEGMENT - 1U) && odd[seg][CELLS_PER_SEGMENT - 1U] <= convertRegToVoltage(0))
            pin_open[CELLS_PER_SEGMENT] = true;

        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; ++cell)
        {
            if (pin_open[cell] || pin_open[cell + 1U])
                local.owc_stats.cells_ok[seg].reset(cell);
        }
    }

    local.owc_stats.stamp(io::time::getCurrentMs());
}

[[nodiscard]] OwcStepState stepCellOpenWire(const OwcStepState current)
{
    switch (current)
    {
        case OwcStepState::START_OW_ODD:
        {
            if (const auto res = io::adbms::command::owcCells(OpenWireSwitch::ODD_CHANNELS); !res)
            {
                LOG_WARN("ADSV(OWC odd) failed: %s", error_code_to_string(res.error()));
                return OwcStepState::START_OW_ODD;
            }
            return OwcStepState::READ_OW_ODD;
        }

        case OwcStepState::READ_OW_ODD:
        {
            readSecondaryVoltages(OpenWire::ODD);

            if (const auto res = io::adbms::command::owcCells(OpenWireSwitch::EVEN_CHANNELS); !res)
            {
                LOG_WARN("ADSV(OWC even) failed: %s", error_code_to_string(res.error()));
                return OwcStepState::START_OW_ODD; // odd data is stale by the retry; restart clean
            }
            return OwcStepState::READ_OW_EVEN;
        }

        case OwcStepState::READ_OW_EVEN:
        {
            readSecondaryVoltages(OpenWire::EVEN);
            // Both parities are now from the same cycle — safe to compare.
            evaluateOpenWire();

            if (const auto res = io::adbms::command::owcCells(OpenWireSwitch::ODD_CHANNELS); !res)
            {
                LOG_WARN("ADSV(OWC odd) failed: %s", error_code_to_string(res.error()));
                return OwcStepState::START_OW_ODD;
            }
            return OwcStepState::READ_OW_ODD;
        }
    }
    return OwcStepState::START_OW_ODD;
}

// ============================================================================================
// CONFIG MACHINE — mute state and per-cell PWM duty
//
//        ┌───────┐  write ok   ┌────────┐  readback ok
//   ────►│ WRITE ├────────────►│ VERIFY ├───────────────┐
//        └───▲───┘             └───┬────┘               │
//            │  mismatch, retries  │                    │
//            ├─────────────────────┤                    │
//            │                     │ mismatch, out of retries
//            │              ┌──────▼──┐                 │
//            └──────────────┤ RECOVER │                 │
//                           └─────────┘                 │
//            ▲                                          │
//            └──────────────────────────────────────────┘
//
// WRITE always re-pushes the in-memory config, whether or not a request came in. That is
// deliberate: the ADBMS watchdog resets every register — DCC, mute and PWM included — after 2 s
// without a valid command, and a thermal shutdown clears them outright. Cycling WRITE/VERIFY on
// alternate visits re-pushes roughly every 1.2 s, so the machine *is* the resync and there is no
// separate timer to keep. (The 2 s watchdog itself is never at risk — any valid command feeds
// it, and something is on the bus every 50 ms.)
//
// It also means requests::post() needs no "pending" flag: WRITE reads whatever is in
// config_request at the time, so a new request is picked up on the next visit either way.
// ============================================================================================

enum class ConfigStepState : uint8_t
{
    WRITE,
    VERIFY,
    RECOVER,
};

ConfigStepState             config_step_state = ConfigStepState::WRITE;
app::pack::BalancingRequest config_request{};
uint8_t                     config_write_attempts = 0U;

[[nodiscard]] ConfigStepState stepConfig(const ConfigStepState current)
{
    switch (current)
    {
        case ConfigStepState::WRITE:
        {
            const bool muted = config_request.kind == app::pack::RequestKind::MUTE_BALANCING;

            // Writes CFGA (mute bit) plus both PWM groups. Duty is per cell, 4 bits: 0 = off,
            // 0xF = 100% of the chip's 937 ms PWM period.
            if (const auto res = app::pack::config::setBalanceConfig(muted, config_request.duty); !res)
            {
                LOG_WARN("balance config write failed: %s", error_code_to_string(res.error()));
                return ConfigStepState::WRITE;
            }
            return ConfigStepState::VERIFY;
        }

        case ConfigStepState::VERIFY:
        {
            if (app::pack::config::checkSegmentConfig() && app::pack::config::checkPwmConfig())
            {
                config_write_attempts = 0U;
                return ConfigStepState::WRITE;
            }
            if (++config_write_attempts >= MAX_CONFIG_WRITE_ATTEMPTS)
                return ConfigStepState::RECOVER;
            return ConfigStepState::WRITE;
        }

        case ConfigStepState::RECOVER:
        {
            LOG_ERROR("config readback mismatched %u times; balancing state is not trustworthy", config_write_attempts);
            config_write_attempts = 0U;
            return ConfigStepState::WRITE;
        }
    }
    return ConfigStepState::WRITE;
}

// ============================================================================================
// FLAGS MACHINE — status registers
//
//        ┌─────────────┐        ┌──────────────┐
//   ────►│ READ_STATUS ├───────►│ CLEAR_STATUS ├──┐
//        └──────▲──────┘        └──────┬───────┘  │
//               └──────────────────────┴──────────┘
//
// Split because these flags are sticky until cleared. READ_STATUS records them; CLEAR_STATUS
// wipes them so the next read reports what is happening now rather than the first fault that
// ever fired. Splitting also halves the bus time per visit — READ_STATUS alone is five register
// groups.
// ============================================================================================

enum class FlagsStepState : uint8_t
{
    READ_STATUS,
    CLEAR_STATUS,
};

FlagsStepState flags_step_state = FlagsStepState::READ_STATUS;

[[nodiscard]] FlagsStepState stepADBMS6830Flags(const FlagsStepState current)
{
    switch (current)
    {
        case FlagsStepState::READ_STATUS:
        {
            const Segments<io::adbms::StatusGroupsRes> status = io::adbms::read::status();

            for (uint8_t seg = 0U; seg < NUM_SEGMENTS; ++seg)
            {
                // STATC: supply rails, thermal shutdown, trim / SPI / oscillator self tests.
                if (const auto &c = status[seg].stat_c; c)
                {
                    const auto &s = c.value();

                    local.adbms6830_flags.supply_ok.set(seg, !s.va_ov && !s.va_uv && !s.vd_ov && !s.vd_uv && !s.vde);
                    local.adbms6830_flags.therm_shdn_ok.set(seg, !s.thsd);
                    local.adbms6830_flags.self_test_ok.set(
                        seg, !s.ced && !s.cmed && !s.sed && !s.smed && !s.spiflt && !s.tmodchk && !s.oscchk);

                    // csflt bit N == C-ADC and S-ADC disagreed on cell N by more than CTH. Only
                    // meaningful if the redundancy compare is enabled (see the note under the cell
                    // machine); harmless either way, since it stays clear when it never runs.
                    for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; ++cell)
                    {
                        if ((s.csflt >> cell) & 0x1U)
                            local.voltage_stats.valid[seg].reset(cell);
                    }
                }
                else
                {
                    // Couldn't read the segment — report not-OK rather than leave a stale pass.
                    local.adbms6830_flags.supply_ok.reset(seg);
                    local.adbms6830_flags.therm_shdn_ok.reset(seg);
                    local.adbms6830_flags.self_test_ok.reset(seg);
                }

                // STATD: the per-cell OV/UV comparisons the chip runs itself against VOV/VUV in
                // CFGB. Bit pairs are [UV, OV] per cell, ascending.
                if (const auto &d = status[seg].stat_d; d)
                {
                    const uint32_t covuv = d.value().covuv;
                    for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; ++cell)
                    {
                        local.adbms6830_flags.uv_ok[seg].set(cell, ((covuv >> (2U * cell)) & 0x1U) == 0U);
                        local.adbms6830_flags.ov_ok[seg].set(cell, ((covuv >> (2U * cell + 1U)) & 0x1U) == 0U);
                    }
                }
                else
                {
                    local.adbms6830_flags.uv_ok[seg].reset();
                    local.adbms6830_flags.ov_ok[seg].reset();
                }
            }

            local.adbms6830_flags.stamp(io::time::getCurrentMs());
            return FlagsStepState::CLEAR_STATUS;
        }

        case FlagsStepState::CLEAR_STATUS:
        {
            LOG_IF_ERR(io::adbms::clear::flags());
            return FlagsStepState::READ_STATUS;
        }
    }
    return FlagsStepState::READ_STATUS;
}

// ============================================================================================
// OUTER SCHEDULER
//
// One table entry per tick, wrapping. Cells takes every other slot because cell voltage is the
// safety-critical signal and shouldn't wait a full rotation. Retuning cadence is a table edit.
// ============================================================================================

constexpr std::array<Step, 12> SCHEDULE{ {
    Step::Cells, Step::ThermsMux1,
    Step::Cells, Step::ThermsMux2,
    Step::Cells, Step::CellOpenWireOdd,
    Step::Cells, Step::CellOpenWireEven,
    Step::Cells, Step::Config,
    Step::Cells, Step::ADBMS6830Flags,
} };

size_t schedule_index = 0U;

[[nodiscard]] Step nextStep()
{
    const Step step = SCHEDULE[schedule_index];
    schedule_index  = (schedule_index + 1U) % SCHEDULE.size();
    return step;
}
} // namespace

namespace app::pack
{
namespace requests
{
    void post(const BalancingRequest &request)
    {
        // Single-slot mailbox, last writer wins. Callers never touch the SPI bus and never block;
        // the config machine picks this up on its next WRITE. Balancing posts at most once per
        // state transition and its states are seconds long, so coalescing can't lose intent.
        config_request = request;
    }
} // namespace requests

namespace sequence
{
    void init()
    {
        local          = Snapshot{};
        schedule_index = 0U;

        cell_step_state    = CellStepState::START_CONTINUOUS;
        cell_read_failures = 0U;

        therm_step_state     = ThermStepState::MUX_0_7_SELECT;
        therm_scratch        = {};
        therm_scratch_valid  = {};
        therm_scratch_owc_ok = {};
        therm_bank_0_7_seen  = false;

        owc_step_state = OwcStepState::START_OW_ODD;
        owc_voltages   = {};
        owc_valid      = {};

        // Default-constructed request is MUTE with all-zero duty, i.e. balancing off. The config
        // machine pushes it on its first WRITE, so the chain comes up in a known-safe discharge
        // state rather than inheriting whatever the registers powered up with.
        config_step_state     = ConfigStepState::WRITE;
        config_request        = BalancingRequest{};
        config_write_attempts = 0U;

        flags_step_state = FlagsStepState::READ_STATUS;
    }

    void tick()
    {
        switch (nextStep())
        {
            case Step::Cells:
                cell_step_state = stepCells(cell_step_state);
                break;

            // Both therm slots service the same machine; which mux bank is armed lives in
            // ThermStepState, not in the slot. Collapsing these to one Step::Therms listed twice
            // in SCHEDULE would read better — see the header note.
            case Step::ThermsMux1:
            case Step::ThermsMux2:
                therm_step_state = stepTherms(therm_step_state);
                break;

            case Step::CellOpenWireOdd:
            case Step::CellOpenWireEven:
                owc_step_state = stepCellOpenWire(owc_step_state);
                break;

            case Step::Config:
                config_step_state = stepConfig(config_step_state);
                break;

            case Step::ADBMS6830Flags:
                flags_step_state = stepADBMS6830Flags(flags_step_state);
                break;
        }
    }

    const Snapshot &snapshot()
    {
        return local;
    }
} // namespace sequence
} // namespace app::pack
