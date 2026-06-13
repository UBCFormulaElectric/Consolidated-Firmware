#include "hw_gpios.hpp"
#include "io_irs.hpp"

using namespace app::can_utils;

namespace io::irs
{

ContactorState negativeState()
{
    return shdn_term_sns.readPin() ? ContactorState::CONTACTOR_STATE_CLOSED : ContactorState::CONTACTOR_STATE_OPEN;
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