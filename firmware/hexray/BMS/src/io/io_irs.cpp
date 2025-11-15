#include "hw_gpios.hpp"
#include "io_irs.hpp"

namespace io::irs
{

constexpr float TOTAL_ISENSE_SHUNT_R_OHMS = 0.075f;
constexpr float TOTAL_ISENSE_AMPLIFIER    = 20.0f;
constexpr float TOTAL_ISENSE_V_TO_A       = (1.0f / (IR_TOTAL_ISENSE_SHUNT_R_OHMS * IR_TOTAL_ISENSE_AMPLIFIER));
constexpr float POSITIVE_CLOSED_THRESH_A  = 0.5f;
constexpr float LOOP_GAIN                 = (1.0f / 20.0f);
constexpr float LOOP_SHUNT_RES            = (1.0f / 75.0e-3f);

ContactorState negativeState()
{
    return hw::Gpio::readPin(&hw::gpios::msd_shdn_sns_pin) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN;
}

void setPositive(const ContactorState state)
{
    hw::Gpio::writePin(&hw::gpios::ir_p_en_pin, state == CONTACTOR_STATE_CLOSED);
}

ContactorState positiveState()
{
    return hw::Gpio::readPin(&hw::gpios::ir_p_en_pin) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN;
}

void setPrecharge(const ContactorState state)
{
    hw::Gpio::writePin(&hw::gpios::precharge_en_pin, state == CONTACTOR_STATE_CLOSED);
}

ContactorState prechargeState()
{
    return hw::Gpio::readPin(&hw::gpios::precharge_en_pin) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN;
}
} // namespace io::irs