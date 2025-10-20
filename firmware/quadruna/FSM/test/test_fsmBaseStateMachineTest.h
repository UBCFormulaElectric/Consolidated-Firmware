#include <gtest/gtest.h>
#include "test_baseStateMachineTest.h"

#include "fake_io_apps.hpp"
#include "fake_io_brake.hpp"
#include "fake_io_steering.hpp"
#include "fake_io_suspension.hpp"
#include "fake_io_wheels.hpp"
#include "fake_io_loadCell.hpp"

extern "C"
{
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_stateMachine.h"
#include "app_canUtils.h"
#include "app_utils.h"
#include "app_mainState.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_steering.h"
#include "app_suspension.h"
#include "app_wheels.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class FsmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        app_canTx_init();
        app_canRx_init();

        app_apps_init();
        app_stateMachine_init(app_mainState_get());
    }

    void TearDown() override
    {
        // Reset fakes
        fake_io_apps_init_reset();
        fake_io_apps_getPrimary_reset();
        fake_io_apps_isPrimaryOCSC_reset();
        fake_io_apps_getSecondary_reset();
        fake_io_apps_isSecondaryOCSC_reset();

        fake_io_brake_isActuated_reset();
        fake_io_brake_getFrontPressurePsi_reset();
        fake_io_brake_frontPressureSensorOCSC_reset();
        fake_io_brake_getRearPressurePsi_reset();
        fake_io_brake_rearPressureSensorOCSC_reset();

        fake_io_steering_getAngleDegrees_reset();
        fake_io_steering_sensorOCSC_reset();

        fake_io_suspension_getLeftTravel_reset();
        fake_io_suspension_getRightTravel_reset();
        fake_io_suspension_leftSensorOCSC_reset();
        fake_io_suspension_rightSensorOCSC_reset();

        fake_io_wheels_getLeftSpeedKph_reset();
        fake_io_wheels_getRightSpeedKph_reset();
        fake_io_wheels_checkIfLeftSensorActive_reset();
        fake_io_wheels_checkIfRightSensorActive_reset();

        fake_io_loadCell_getMechanicalLoad1_reset();
        fake_io_loadCell_getMechanicalLoad2_reset();
        fake_io_loadCell_sensor1OCSC_reset();
        fake_io_loadCell_sensor2OCSC_reset();
    }
};
