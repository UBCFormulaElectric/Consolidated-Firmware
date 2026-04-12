#include <gtest/gtest.h>
#include "test/test_VCBase.hpp"
#include "app_shdnLast.hpp"
#include "vc_fakes.hpp"
#include "util_errorCodes.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

class VCShdnLastTest : public VCBaseTest
{
};

TEST_F(VCShdnLastTest, test_latch_not_ok_in_order)
{
    app::can_rx::BMS_BmsLatchOk_update(true);
    app::can_rx::BMS_ImdLatchOk_update(true);
    app::can_rx::BMS_BspdLatchOk_update(true);
    app::can_rx::BMS_HVPShdnOKStatus_update(true);
    app::can_rx::BMS_HVNShdnOKStatus_update(true);
    app::can_tx::VC_MSDOrEMeterOKStatus_set(true);
    app::can_rx::RSM_RearLeftMotorInterlock_update(true);
    app::can_tx::VC_RearRightMotorInterlock_set(true);
    app::can_rx::DAM_REStopOKStatus_update(true);
    app::can_rx::DAM_LEStopOKStatus_update(true);
    app::can_rx::FSM_COCKPITOKStatus_update(true);
    app::can_rx::FSM_BOTSOKStatus_update(true);
    app::can_rx::FSM_FrontLeftILCKInertiaOKStatus_update(true);
    app::can_rx::FSM_FrontRightILCKOKStatus_update(true);
    app::can_tx::VC_TSMSOKStatus_set(true);

    app::can_tx::VC_TSMSOKStatus_set(false);
    app::shdnLast::broadcast();
    ASSERT_EQ(app::can_utils::ShutdownNode::SHDN_TSMS, app::can_tx::VC_FirstFaultNode_get());

    app::can_tx::VC_TSMSOKStatus_set(true);
    app::can_rx::FSM_FrontRightILCKOKStatus_update(false);
    app::shdnLast::broadcast();
    ASSERT_EQ(app::can_utils::ShutdownNode::SHDN_FR_ILCK, app::can_tx::VC_FirstFaultNode_get());

    app::can_rx::FSM_FrontRightILCKOKStatus_update(true);
    app::can_tx::VC_RearRightMotorInterlock_set(false);
    app::shdnLast::broadcast();
    ASSERT_EQ(app::can_utils::ShutdownNode::SHDN_RR_ILCK, app::can_tx::VC_FirstFaultNode_get());

    app::can_tx::VC_RearRightMotorInterlock_set(true);
    app::can_tx::VC_MSDOrEMeterOKStatus_set(false);
    app::shdnLast::broadcast();
    ASSERT_EQ(app::can_utils::ShutdownNode::SHDN_MSD_EMETER_ILCK, app::can_tx::VC_FirstFaultNode_get());
}
