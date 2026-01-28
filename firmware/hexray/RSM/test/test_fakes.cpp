#include "test_fakes.hpp"
#include "io_brake.hpp"
#include "io_coolant.hpp"
#include "io_suspension.hpp"
#include "io_tireTemp.hpp"
#include "io_imus.hpp"

namespace fakes::io
{
namespace brake
{
    static float rearPressurePsi = 0.0f;

    void setRearPressurePsi(const float value)
    {
        rearPressurePsi = value;
    }
} // namespace brake

namespace coolant
{
    static float flowRate = 0.0f;
    void         setFlowRate(float value)
    {
        flowRate = value;
    }
} // namespace coolant

namespace suspension
{
    static float RLTravel = 0.0f;
    static float RRTravel = 0.0f;
    static bool  RROCSC   = false;
    static bool  RLOCSC   = false;

    void setRLTravel(float value)
    {
        RLTravel = value;
    }

    void setRRTravel(float value)
    {
        RRTravel = value;
    }

    void setRR_OCSC(bool value)
    {
        RROCSC = value;
    }

    void setRL_OCSC(bool value)
    {
        RLOCSC = value;
    }
} // namespace suspension

namespace tireTemp
{
    static float temperature = 0.0f;

    void set(const float value)
    {
        temperature = value;
    }
} // namespace tireTemp
} // namespace fakes::io

namespace io
{
namespace brake
{
    float getRearPressurePsi()
    {
        return fakes::io::brake::rearPressurePsi;
    }
} // namespace brake

namespace coolant
{
    float getFlowRate()
    {
        return fakes::io::coolant::flowRate;
    }
} // namespace coolant

namespace suspension
{
    float getRLTravel()
    {
        return fakes::io::suspension::RLTravel;
    }

    float getRRTravel()
    {
        return fakes::io::suspension::RRTravel;
    }

    bool RL_OCSC()
    {
        return fakes::io::suspension::RLOCSC;
    }

    bool RR_OCSC()
    {
        return fakes::io::suspension::RROCSC;
    }
} // namespace suspension

<<<<<<< HEAD
namespace imu
{
    ExitCode getLinearAccelerationX(float *value)
    {
        *value = fakes::io::imu::x_lin_accel;
        return fakes::io::imu::imu_faults.x_lin_accel_fault;
    }

    ExitCode getLinearAccelerationY(float *value)
    {
        *value = fakes::io::imu::y_lin_accel;
        return fakes::io::imu::imu_faults.y_lin_accel_fault;
    }

    ExitCode getLinearAccelerationZ(float *value)
    {
        *value = fakes::io::imu::z_lin_accel;
        return fakes::io::imu::imu_faults.z_lin_accel_fault;
    }

    ExitCode getAngularVelocityRoll(float *value)
    {
        *value = fakes::io::imu::roll_ang_vel;
        return fakes::io::imu::imu_faults.roll_ang_vel_fault;
    }

    ExitCode getAngularVelocityPitch(float *value)
    {
        *value = fakes::io::imu::pitch_ang_vel;
        return fakes::io::imu::imu_faults.pitch_ang_vel_fault;
    }

    ExitCode getAngularVelocityYaw(float *value)
    {
        *value = fakes::io::imu::yaw_ang_vel;
        return fakes::io::imu::imu_faults.yaw_ang_vel_fault;
    }
} // namespace imu

=======
>>>>>>> master
namespace rPump
{
    ExitCode setPercentage(float value)
    {
        return ExitCode::EXIT_CODE_OK;
    }
} // namespace rPump

namespace tireTemp
{
    float get()
    {
        return fakes::io::tireTemp::temperature;
    }
} // namespace tireTemp
} // namespace io
