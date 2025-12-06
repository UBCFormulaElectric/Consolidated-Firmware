namespace fakes::io // Set all the fake values using functions here
{
namespace apps
{
    static float primary_pressure = 0;
    void         setPrimaryPressure(float pressure)
    {
        primary_pressure = pressure;
    }

    static bool primary_ocsc = false;
    void        setPrimaryOCSC(bool ocsc)
    {
        primary_ocsc = ocsc;
    }

    static float secondary_pressure = 0;
    void         setSecondaryPressure(float pressure)
    {
        secondary_pressure = pressure;
    }

    static bool secondary_ocsc = false;
    void        setSecondaryOCSC(bool ocsc)
    {
        secondary_ocsc = ocsc;
    }
} // namespace apps

namespace brake
{
    static bool brake_actuated = false;
    void        setBrakeActuated(bool actuated)
    {
        brake_actuated = actuated;
    }

    static float brake_pressure_psi = 0;
    void         setFrontPressurePsi(float psi)
    {
        brake_pressure_psi = psi;
    }

    static bool pressure_ocsc = false;
    void        setOCSC(bool ocsc)
    {
        pressure_ocsc = ocsc;
    }
} // namespace brake

namespace steering
{
    static float angle_degrees = 0;
    void         setAngleDegrees(float angle)
    {
        angle_degrees = angle;
    }

    static bool ocsc = false;
    void        setOCSC(bool o)
    {
        ocsc = o;
    }
} // namespace steering

namespace suspension
{
    static float leftTravel = 0;
    void         setLeftTravel(float travel)
    {
        leftTravel = travel;
    }

    static float rightTravel = 0;
    void         setRightTravel(float travel)
    {
        rightTravel = travel;
    }

    static bool leftOCSC = false;
    void        setLeftSensorOCSC(bool ocsc)
    {
        leftOCSC = ocsc;
    }

    static bool rightOCSC = false;
    void        setRightSensorOCSC(bool ocsc)
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

    float getPrimary(void)
    {
        return fakes::io::apps::primary_pressure;
    }

    bool isPrimaryOCSC(void)
    {
        return fakes::io::apps::primary_ocsc;
    }

    float getSecondary(void)
    {
        return fakes::io::apps::secondary_pressure;
    }

    bool isSecondaryOCSC(void)
    {
        return fakes::io::apps::secondary_ocsc;
    }
} // namespace apps

namespace brake
{
    bool isActuated(void)
    {
        return fakes::io::brake::brake_actuated;
    }

    float getFrontPressurePsi(void)
    {
        return fakes::io::brake::brake_pressure_psi;
    }

    bool OCSC(void)
    {
        return fakes::io::brake::pressure_ocsc;
    }
} // namespace brake

namespace fsmShdn
{
    bool BOTS_SHDN_OK_get(void)
    {
        return true;
    }
    bool COCKPIT_SHDN_OK_get(void)
    {
        return true;
    }
    bool FL_SHDN_OK_get(void)
    {
        return true;
    }
    bool FR_SHDN_OK_get(void)
    {
        return true;
    }
} // namespace fsmShdn

namespace steering
{
    float getAngleDegrees(void)
    {
        return fakes::io::steering::angle_degrees;
    }

    bool sensorOCSC(void)
    {
        return fakes::io::steering::ocsc;
    }
} // namespace steering

namespace suspension
{
    float getLeftTravel(void)
    {
        return fakes::io::suspension::leftTravel;
    }

    float getRightTravel(void)
    {
        return fakes::io::suspension::rightTravel;
    }

    bool leftSensorOCSC(void)
    {
        return fakes::io::suspension::leftOCSC;
    }

    bool rightSensorOCSC(void)
    {
        return fakes::io::suspension::rightOCSC;
    }
} // namespace suspension
} // namespace io