#include "app_lowVoltageBattery.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_batteryCharging.hpp"
#include "io_batteryMonitoring_datatypes.hpp"
#include "app_canTx.hpp"
#include "app_timer.hpp"

namespace
{
constexpr float    DISCHARGE_THRESHOLD_V   = 10e-3f; // discharge until min/max are between this
constexpr float    VUV                     = 2.6f;
constexpr float    VOV                     = 4.15f;
constexpr uint32_t SETTLE_TIME_MS          = 5 * 1000;  // 5 seconds
constexpr uint32_t BALANCE_TIME_MS         = 20 * 1000; // 20 seconds
constexpr float    CHARGE_THRESHOLD_LOW_V  = 15.5f;
constexpr float    CHARGE_THRESHOLD_HIGH_V = 16.2f;

BalancingState balance_state       = BalancingState::DISABLED;
BalancingPhase balance_phase       = BalancingPhase::ODD;
ChargeState    charge_state        = ChargeState::STOPPED;
uint16_t       active_balance_mask = 0x0000;

app::Timer settle_timer(SETTLE_TIME_MS);
app::Timer balance_timer(BALANCE_TIME_MS);

constexpr uint8_t NUM_CELLS = 4;

constexpr std::array<CellBalance_BitMask, NUM_CELLS> CELL_BALANCE_MAP = {
    { CellBalance_BitMask::CELL1, CellBalance_BitMask::CELL2, CellBalance_BitMask::CELL3, CellBalance_BitMask::CELL4 }
};

/**
 * @brief Determines the minimum cell amount from the 4 cells
 * @param voltages Array of cell voltages as floats
 * @return MinCell (value and index)
 */
MinCell get_min(const std::array<std::expected<float, ErrorCode>, NUM_CELLS> &voltages)
{
    MinCell min = { 0, std::numeric_limits<float>::max() };

    for (int i = 0; i < NUM_CELLS; i++)
    {
        if (voltages[i] && voltages[i].value() < min.value)
            min = { (uint8_t)(i + 1), voltages[i].value() };
    }
    return min;
}
MaxCell get_max(const std::array<std::expected<float, ErrorCode>, NUM_CELLS> &voltages)
{
    MaxCell max = { 0, 0.0f };

    for (int i = 0; i < NUM_CELLS; i++)
    {
        if (voltages[i] && voltages[i].value() > max.value)
            max = { (uint8_t)(i + 1), voltages[i].value() };
    }

    return max;
}

/**
 * @brief Creates the mask for you to send as a subcommand to balance
 * @param voltages, min_cell, phase
 * @return uint16_t: the bit mask
 */
uint16_t get_balance_mask(
    const std::array<std::expected<float, ErrorCode>, NUM_CELLS> &voltages,
    const MinCell                                                &min_cell,
    BalancingPhase                                                phase)
{
    uint16_t mask = 0x0000;

    for (uint8_t i = 0; i < NUM_CELLS; i++)
    {
        const bool is_odd = (i % 2 == 0);
        if (phase == BalancingPhase::ODD && !is_odd)
            continue;
        if (phase == BalancingPhase::EVEN && is_odd)
            continue;

        if (!voltages[i])
            continue;
        if ((i + 1) == min_cell.index)
            continue;
        if (voltages[i].value() <= VUV)
            continue;
        if (voltages[i].value() - min_cell.value < DISCHARGE_THRESHOLD_V)
            continue;

        mask |= static_cast<uint16_t>(CELL_BALANCE_MAP[i]);
    }

    return mask;
}

/**
 * @brief Charging logic
 * @param max_cell, min_cell
 * @return 1 or 0: charge or not
 */
bool should_charge(float pack_voltage, MaxCell max_cell, MinCell min_cell)
{
    switch (charge_state)
    {
        case ChargeState::STOPPED:
        {
            if ((max_cell.value < VOV) && (min_cell.value > VUV) && (pack_voltage < CHARGE_THRESHOLD_LOW_V))
                charge_state = ChargeState::CHARGING;
            break;
        }
        case ChargeState::CHARGING:
        {
            if ((min_cell.value < VUV) || (max_cell.value >= VOV) || (pack_voltage >= CHARGE_THRESHOLD_HIGH_V))
                charge_state = ChargeState::STOPPED;
            break;
        }
        default:
            charge_state = ChargeState::STOPPED;
            break;
    }
    return charge_state == ChargeState::CHARGING;
}

[[maybe_unused]] std::expected<void, ErrorCode>
    balancing_tick(const std::array<std::expected<float, ErrorCode>, NUM_CELLS> &voltages, const MinCell &min_cell)
{
    switch (balance_state)
    {
        case BalancingState::DISABLED:
        {
            settle_timer.restart();
            balance_state = BalancingState::SETTLE;
            break;
        }
        case BalancingState::SETTLE:
        {
            if (settle_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
            {
                const uint16_t mask = get_balance_mask(voltages, min_cell, balance_phase);
                balance_phase = (balance_phase == BalancingPhase::ODD) ? BalancingPhase::EVEN : BalancingPhase::ODD;
                RETURN_IF_ERR(io::batteryMonitoring::send_balancing_subcommand(static_cast<CellBalance_BitMask>(mask)));
                active_balance_mask = mask;
                balance_timer.restart();
                balance_state = BalancingState::BALANCE;
            }
            break;
        }
        case BalancingState::BALANCE:
        {
            if (balance_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
            {
                LOG_IF_ERR(io::batteryMonitoring::stop_balancing_subcommand());
                active_balance_mask = 0x0000;
                settle_timer.restart();
                balance_state = BalancingState::SETTLE;
            }
            break;
        }
        default:
            break;
    }
    return {};
}
} // namespace

namespace app::batteryMonitoring
{
std::expected<void, ErrorCode> update()
{
    // 1. Initialize
    static bool init_done           = false;
    static bool manual_balance_sent = false;

    if (!init_done)
    {
        for (int tries = 0; tries < 10 && !init_done; tries++)
            init_done = io::batteryMonitoring::init().has_value();

        if (!init_done)
            return std::unexpected(ErrorCode::ERROR);

        LOG_INFO("Battery monitor initialized");
    }

    // 2. Send a manual balancing command. TODO: REMOVE AFTER TESTING
    if (!manual_balance_sent)
    {
        const auto stop = io::batteryMonitoring::stop_balancing_subcommand();
        const auto err  = io::batteryMonitoring::send_balancing_subcommand(CellBalance_BitMask::CELL2);
        auto       sup  = io::batteryMonitoring::read_balancing_subcommand();
        if (err.has_value())
        {
            manual_balance_sent = true;
            LOG_INFO("SHIT WAS SENT!!!!!!!!!");
        }
        LOG_INFO("done");
    }

    // 2. Check for fault
    if (io::batteryMonitoring::is_alert_pending())
    {
        io::batteryMonitoring::clear_alert_pending();
        const auto alarm_status = io::batteryMonitoring::read_alarm_status();
        if (alarm_status.has_value() && alarm_status->bits.SSA)
        {
            LOG_INFO("A flag has raised in Safety Status A");
            can_tx::VC_SafetyA_Flag_set(1);
            const auto safetyA_status = io::batteryMonitoring::get_safety_alert_a();
            if (safetyA_status.has_value() && safetyA_status->bits.COV)
            {
                LOG_INFO("OVERVOLTAGE");
                // CAN Message
            }
            if (safetyA_status.has_value() && safetyA_status->bits.CUV)
            {
                LOG_INFO("UNDERVOLTAGE");
                // CAN Message
            }
        }
        if (alarm_status.has_value() && alarm_status->bits.SSBC)
        {
            LOG_INFO("A flag has raised in Safety Status B/C");
            can_tx::VC_SafetyBC_Flag_set(1);
            const auto safetyB_status = io::batteryMonitoring::get_safety_alert_b();
            if (safetyB_status.has_value() && safetyB_status->bits.OTINT)
            {
                LOG_INFO("OVERTEMPERATURE ON DIE");
                // CAN Message
            }
        }
    }

    // auto bruhh = io::batteryMonitoring::get_Ttemperature();
    // auto brev = io::batteryMonitoring::read_currentcc1();

    // 2. Read voltages and currents
    const auto cell1_voltage = io::batteryMonitoring::get_voltage_cell(CellReading::CELL1);
    const auto cell2_voltage = io::batteryMonitoring::get_voltage_cell(CellReading::CELL2);
    const auto cell3_voltage = io::batteryMonitoring::get_voltage_cell(CellReading::CELL3);
    const auto cell4_voltage = io::batteryMonitoring::get_voltage_cell(CellReading::CELL4);
    const std::array<std::expected<float, ErrorCode>, NUM_CELLS> cell_voltages = { {
        cell1_voltage,
        cell2_voltage,
        cell3_voltage,
        cell4_voltage,
    } };

    const auto pack_voltage = io::batteryMonitoring::get_voltage_system(SystemReading::PACK_V);
    const auto load_voltage = io::batteryMonitoring::get_voltage_system(SystemReading::LOAD_V);

    const auto current = io::batteryMonitoring::get_current();

    // 3. Balancing
    const MinCell min_cell = get_min(cell_voltages);
    const MaxCell max_cell = get_max(cell_voltages);
    const float   imbalance_magnitude =
        (min_cell.index != 0 && max_cell.index != 0) ? (max_cell.value - min_cell.value) : 0.0f;
    const uint16_t computed_mask = get_balance_mask(cell_voltages, min_cell, balance_phase);
    // balancing_tick(cell_voltages, min_cell);
    const uint16_t c1_mV   = static_cast<uint16_t>(cell1_voltage.value_or(0.0f) * 1000.0f);
    const uint16_t c2_mV   = static_cast<uint16_t>(cell2_voltage.value_or(0.0f) * 1000.0f);
    const uint16_t c3_mV   = static_cast<uint16_t>(cell3_voltage.value_or(0.0f) * 1000.0f);
    const uint16_t c4_mV   = static_cast<uint16_t>(cell4_voltage.value_or(0.0f) * 1000.0f);
    const uint16_t diff_mV = static_cast<uint16_t>(imbalance_magnitude * 1000.0f);
    LOG_INFO("Cell voltages: C1=%umV C2=%umV C3=%umV C4=%umV", c1_mV, c2_mV, c3_mV, c4_mV);

    const auto mask = io::batteryMonitoring::read_balancing_subcommand();
    if (mask.has_value())
    {
        active_balance_mask = mask.value();
        LOG_INFO(
            "Balance status: Difference =%umV  Balancing mask=0x%04X", static_cast<unsigned>(diff_mV),
            static_cast<uint16_t>(mask.value()));
    }

    const auto cell_balancing_time = io::batteryMonitoring::read_balacing_time_seconds();
    cell_balancing_time.has_value()
        ? LOG_INFO("Cell Balance in seconds=%u", static_cast<unsigned>(cell_balancing_time.value()))
        : LOG_WARN("Cell Balance read failed");

    // 4. Charging
    const bool charge_enable = should_charge(pack_voltage.value(), max_cell, min_cell);
    if (charge_enable)
        io::batteryCharging::chargerEnable();
    else
        io::batteryCharging::chargerDisable();

    // Integrated Charge (SOC Stuff)
    // const auto integrated_charge = io::batteryMonitoring::get_integrated_charge();

    // 5. Broadcasting
    app::can_tx::VC_CELL1_Voltage_set(cell1_voltage.value_or(0.0f));
    app::can_tx::VC_CELL2_Voltage_set(cell2_voltage.value_or(0.0f));
    app::can_tx::VC_CELL3_Voltage_set(cell3_voltage.value_or(0.0f));
    app::can_tx::VC_CELL4_Voltage_set(cell4_voltage.value_or(0.0f));

    app::can_tx::VC_PACK_Voltage_set(pack_voltage.value_or(0.0f));
    app::can_tx::VC_LOAD_Voltage_set(load_voltage.value_or(0.0f));
    app::can_tx::VC_ShuntCurrent_set(current.value_or(0.0f));

    app::can_tx::VC_BalancingState_set(static_cast<uint8_t>(balance_state));
    app::can_tx::VC_ImbalanceMagnitude_set(imbalance_magnitude);
    app::can_tx::VC_BalancingLeader_set(min_cell.index);
    app::can_tx::VC_BalancingCell_set(static_cast<uint8_t>(active_balance_mask));

    return {};
}
} // namespace app::batteryMonitoring
