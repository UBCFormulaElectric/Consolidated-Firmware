#include "fsmMocks.hpp"
#include "app_canTx.hpp"

namespace fakes::io // Set all the fake values using functions here
{
namespace apps
{
    static float primary_pressure = 0;
    void         setPrimaryPressure(const float pressure)
    {
        primary_pressure = pressure;
    }

    static bool primary_ocsc = false;
    void        setPrimaryOCSC(const bool ocsc)
    {
        primary_ocsc = ocsc;
    }

    static float secondary_pressure = 0;
    void         setSecondaryPressure(const float pressure)
    {
        secondary_pressure = pressure;
    }

    static bool secondary_ocsc = false;
    void        setSecondaryOCSC(const bool ocsc)
    {
        secondary_ocsc = ocsc;
    }
} // namespace apps

namespace brake
{
    static bool brake_actuated = false;
    void        setBrakeActuated(const bool actuated)
    {
        brake_actuated = actuated;
    }

    static float brake_pressure_psi = 0;
    void         setFrontPressurePsi(const float psi)
    {
        brake_pressure_psi = psi;
    }

    static bool pressure_ocsc = false;
    void        setOCSC(const bool ocsc)
    {
        pressure_ocsc = ocsc;
    }
} // namespace brake

namespace steering
{
    static float angle_degrees = 0;
    void         setAngleDegrees(const float angle)
    {
        angle_degrees = angle;
    }

    static bool ocsc = false;
    void        setOCSC(const bool o)
    {
        ocsc = o;
    }
} // namespace steering

namespace suspension
{
    static float leftTravel = 0;
    void         setLeftTravel(const float travel)
    {
        leftTravel = travel;
    }

    static float rightTravel = 0;
    void         setRightTravel(const float travel)
    {
        rightTravel = travel;
    }

    static bool leftOCSC = false;
    void        setLeftSensorOCSC(const bool ocsc)
    {
        leftOCSC = ocsc;
    }

    static bool rightOCSC = false;
    void        setRightSensorOCSC(const bool ocsc)
    {
        rightOCSC = ocsc;
    }
} // namespace suspension
} // namespace fakes::io

namespace io // Define the mocked functions here
{
namespace apps
{
    void init()
    {
        // Do nothing
    }

    float getPrimary()
    {
        return fakes::io::apps::primary_pressure;
    }

    bool isPrimaryOCSC()
    {
        return fakes::io::apps::primary_ocsc;
    }

    float getSecondary()
    {
        return fakes::io::apps::secondary_pressure;
    }

    bool isSecondaryOCSC()
    {
        return fakes::io::apps::secondary_ocsc;
    }
} // namespace apps

namespace brake
{
    bool isActuated()
    {
        return fakes::io::brake::brake_actuated;
    }

    float getFrontPressurePsi()
    {
        return fakes::io::brake::brake_pressure_psi;
    }

    bool OCSC()
    {
        return fakes::io::brake::pressure_ocsc;
    }
} // namespace brake

namespace steering
{
    float getAngleDegrees()
    {
        return fakes::io::steering::angle_degrees;
    }

    bool sensorOCSC()
    {
        return fakes::io::steering::ocsc;
    }
} // namespace steering

namespace suspension
{
    float getLeftTravel()
    {
        return fakes::io::suspension::leftTravel;
    }

    float getRightTravel()
    {
        return fakes::io::suspension::rightTravel;
    }

    bool leftSensorOCSC()
    {
        return fakes::io::suspension::leftOCSC;
    }

    bool rightSensorOCSC()
    {
        return fakes::io::suspension::rightOCSC;
    }
} // namespace suspension
} // namespace io

#include "io_fsmShdn.hpp"
const io::shdn::node cockpit_node{ app::can_tx::FSM_COCKPITOKStatus_set };
const io::shdn::node bots_node{ app::can_tx::FSM_BOTSOKStatus_set };
const io::shdn::node fl_shdn_ok_node{ app::can_tx::FSM_FrontLeftILCKInertiaOKStatus_set };
const io::shdn::node fr_shdn_ok_node{ app::can_tx::FSM_FrontRightILCKOKStatus_set };
