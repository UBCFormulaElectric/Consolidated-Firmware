extern "C"
{
#include "io_apps.h"
#include "io_brake.h"
#include "io_fsmShdn.h"
#include "io_steering.h"
#include "io_suspension.h"
}

static float io_apps_primary = 0, io_app_secondary = 0;
static bool  io_apps_primary_ocsc = false, io_apps_secondary_ocsc = false;

static bool  io_brake_is_actuated         = false;
static float io_brake_front_pressure_psi  = 0;
static bool  io_brake_front_pressure_ocsc = false;

static bool cockpit_shdn_ok = false, bots_shdn_ok = false, fl_shdn_ok = false, fr_shdn_ok = false;

static float io_steering_angle_degrees = 0;
static bool  io_steering_sensor_ocsc   = false;

static float io_suspension_left_travel = 0, io_suspension_right_travel = 0;
static bool  io_suspension_left_ocsc = false, io_suspension_right_ocsc = false;

extern "C"
{
    // io apps
    float io_apps_getPrimary()
    {
        return io_apps_primary;
    }
    bool io_apps_isPrimaryOCSC()
    {
        return io_apps_primary_ocsc;
    }
    float io_apps_getSecondary()
    {
        return io_app_secondary;
    }
    bool io_apps_isSecondaryOCSC()
    {
        return io_apps_secondary_ocsc;
    }

    // brake
    bool io_brake_isActuated()
    {
        return io_brake_is_actuated;
    }
    float io_brake_getFrontPressurePsi()
    {
        return io_brake_front_pressure_psi;
    }
    bool io_brake_hwOCSC()
    {
        return io_brake_front_pressure_ocsc;
    }

    // shdn
    bool io_fsmShdn_COCKPIT_SHDN_OK_get()
    {
        return cockpit_shdn_ok;
    }
    bool io_fsmShdn_BOTS_SHDN_OK_get()
    {
        return bots_shdn_ok;
    }
    bool io_fsmShdn_FL_SHDN_OK_get()
    {
        return fl_shdn_ok;
    }
    bool io_fsmShdn_FR_SHDN_OK_get()
    {
        return fr_shdn_ok;
    }

    // steering
    float io_steering_getAngleDegrees()
    {
        return io_steering_angle_degrees;
    }
    bool io_steering_sensorOCSC()
    {
        return io_steering_sensor_ocsc;
    }

    // suspension
    float io_suspension_getLeftTravel()
    {
        return io_suspension_left_travel;
    }
    float io_suspension_getRightTravel()
    {
        return io_suspension_right_travel;
    }
    bool io_suspension_leftSensorOCSC()
    {
        return io_suspension_left_ocsc;
    }
    bool io_suspension_rightSensorOCSC()
    {
        return io_suspension_right_ocsc;
    }
}