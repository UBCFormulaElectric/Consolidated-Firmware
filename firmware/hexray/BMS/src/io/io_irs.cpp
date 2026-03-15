#include "hw_gpios.hpp"
#include "io_irs.hpp"
#include "app_canUtils.hpp"


using namespace app::can_utils;
namespace io::irs
{

constexpr float TOTAL_ISENSE_SHUNT_R_OHMS = 0.075f;
constexpr float TOTAL_ISENSE_AMPLIFIER    = 20.0f;
constexpr float TOTAL_ISENSE_V_TO_A       = (1.0f / (TOTAL_ISENSE_SHUNT_R_OHMS * TOTAL_ISENSE_AMPLIFIER));
constexpr float POSITIVE_CLOSED_THRESH_A  = 0.5f;
constexpr float LOOP_GAIN                 = (1.0f / 20.0f);
constexpr float LOOP_SHUNT_RES            = (1.0f / 75.0e-3f);

ContactorState negativeState()
{
    return msd_shdn_sns.readPin() ? ContactorState::CONTACTOR_STATE_CLOSED : ContactorState::CONTACTOR_STATE_OPEN;
}

void setPositive(const ContactorState state)
{
    ir_p_en.writePin(state == ContactorState::CONTACTOR_STATE_CLOSED);
}

ContactorState positiveState()
{
    return ir_p_en.readPin() ? ContactorState::CONTACTOR_STATE_CLOSED : ContactorState::CONTACTOR_STATE_OPEN;
}

void setPrecharge(const ContactorState state)
{
    precharge_en.writePin(state == ContactorState::CONTACTOR_STATE_CLOSED);
}

ContactorState prechargeState()
{
    return precharge_en.readPin() ? ContactorState::CONTACTOR_STATE_CLOSED : ContactorState::CONTACTOR_STATE_OPEN;
}
} // namespace io::irs
