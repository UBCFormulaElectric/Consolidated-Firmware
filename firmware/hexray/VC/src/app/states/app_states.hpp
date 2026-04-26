#pragma once

#include "app_stateMachine.hpp"
#include "app_canTx.hpp"
#include "torque_vectoring/datatypes/torque_limits.hpp"

using namespace app::tv::datatypes::torque_limits;
namespace app::states
{

extern State init_state;

extern State inverterOn_state;
extern State bmsOn_state;
extern State pcmOn_state;
extern State hvInit_state;
extern State hv_state;

extern State fault_state;

extern State drive_state;
extern State inverter_fault_handling_state;

// TODO: should we put these all in some utils file or keep it here?
inline void send_torque(float fl, float fr, float rl, float rr)
{
    app::can_tx::VC_INVFLTorqueSetpoint_set(TORQUE_REQUEST(fl));
    app::can_tx::VC_INVFRTorqueSetpoint_set(TORQUE_REQUEST(fr));
    app::can_tx::VC_INVRLTorqueSetpoint_set(TORQUE_REQUEST(rl));
    app::can_tx::VC_INVRRTorqueSetpoint_set(TORQUE_REQUEST(rr));
}

inline void set_torque_limit_negative(float fl, float fr, float rl, float rr)
{
    app::can_tx::VC_INVFRTorqueLimitNegative_set(TORQUE_REQUEST(fl));
    app::can_tx::VC_INVFLTorqueLimitNegative_set(TORQUE_REQUEST(fr));
    app::can_tx::VC_INVRRTorqueLimitNegative_set(TORQUE_REQUEST(rl));
    app::can_tx::VC_INVRLTorqueLimitNegative_set(TORQUE_REQUEST(rr));
}

inline void set_torque_limit_positive(float fl, float fr, float rl, float rr)
{
    app::can_tx::VC_INVFRTorqueLimitPositive_set(TORQUE_REQUEST(fl));
    app::can_tx::VC_INVFLTorqueLimitPositive_set(TORQUE_REQUEST(fr));
    app::can_tx::VC_INVRRTorqueLimitPositive_set(TORQUE_REQUEST(rl));
    app::can_tx::VC_INVRLTorqueLimitPositive_set(TORQUE_REQUEST(rr));
}

inline void inverter_on_toggle(bool fl, bool fr, bool rl, bool rr)
{
    app::can_tx::VC_INVFLbInverterOn_set(fl);
    app::can_tx::VC_INVFRbInverterOn_set(fr);
    app::can_tx::VC_INVRRbInverterOn_set(rl);
    app::can_tx::VC_INVRLbInverterOn_set(rr);
}

inline void inverter_enable_toggle(bool fl, bool fr, bool rl, bool rr)
{
    app::can_tx::VC_INVFLbEnable_set(fl);
    app::can_tx::VC_INVFRbEnable_set(fr);
    app::can_tx::VC_INVRLbEnable_set(rl);
    app::can_tx::VC_INVRRbEnable_set(rr);
}

inline void inverter_dc_toggle(bool fl, bool fr, bool rl, bool rr)
{
    app::can_tx::VC_INVFLbDcOn_set(fl);
    app::can_tx::VC_INVFRbDcOn_set(fr);
    app::can_tx::VC_INVRRbDcOn_set(rl);
    app::can_tx::VC_INVRLbDcOn_set(rr);
}

inline void inverter_bError_reset_set(bool fl, bool fr, bool rl, bool rr)
{
    app::can_tx::VC_INVFLbErrorReset_set(fl);
    app::can_tx::VC_INVFRbErrorReset_set(fr);
    app::can_tx::VC_INVRLbErrorReset_set(rl);
    app::can_tx::VC_INVRRbErrorReset_set(rr);
}
} // namespace app::states
