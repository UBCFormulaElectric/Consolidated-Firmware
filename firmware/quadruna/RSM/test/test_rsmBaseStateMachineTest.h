#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_coolant.hpp"
#include "fake_io_loadCell.hpp"
#include "fake_io_suspension.hpp"
#include "fake_io_fans.hpp"
#include "fake_io_brake_light.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_heartbeatMonitors.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "app_mainState.h"
#include "app_coolant.h"
#include "app_loadCell.h"
#include "app_suspension.h"
}
// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.
class RsmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_coolant_init();
        app_stateMachine_init(app_mainState_get());
    }

    void TearDown() override
    {
        fake_io_coolant_getFlowRate_reset();
        fake_io_coolant_checkIfFlowMeterActive_reset();
        fake_io_coolant_getTemperatureA_reset();
        fake_io_coolant_getTemperatureB_reset();
        fake_io_coolant_getPressureA_reset();
        fake_io_coolant_getPressureB_reset();

        fake_io_loadCell_getMechanicalLoad3_reset();
        fake_io_loadCell_getMechanicalLoad4_reset();
        fake_io_loadCell_sensor3OCSC_reset();
        fake_io_loadCell_sensor4OCSC_reset();

        fake_io_suspension_getRearLeftTravel_reset();
        fake_io_suspension_getRearRightTravel_reset();
        fake_io_suspension_leftSensorOCSC_reset();
        fake_io_suspension_rightSensorOCSC_reset();

        fake_io_acc_fan_set_reset();
        fake_io_rad_fan_set_reset();

        fake_io_brake_light_set_reset();
    }
};