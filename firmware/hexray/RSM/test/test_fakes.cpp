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

namespace imu
{
    static float accel_x; 
    static float accel_y; 
    static float accel_z; 
    static float gyro_x; 
    static float gyro_y; 
    static float gyro_z; 
    static RSMImuFaults imu_faults; 

    void setAccelX(float value, ExitCode status)
    {
        accel_x = value; 
        imu_faults.accel_x_fault = status; 
    }
    void setAccelY(float value, ExitCode status)
    {
        accel_y = value; 
        imu_faults.accel_y_fault = status; 
    }
    void setAccelZ(float value, ExitCode status)
    {
        accel_z = value; 
        imu_faults.accel_z_fault = status; 
    }
    void setGyroX(float value, ExitCode status)
    {
        gyro_x = value; 
        imu_faults.gyro_x_fault = status; 
    }
    void setGyroY(float value, ExitCode status)
    {
        gyro_y = value; 
        imu_faults.gyro_y_fault = status; 
    }
    void setGyroZ(float value, ExitCode status)
    {
        gyro_z = value; 
        imu_faults.gyro_z_fault = status; 
    }
}

// {
//     static float        x_lin_accel;
//     static float        y_lin_accel;
//     static float        z_lin_accel;
//     static float        roll_ang_vel;
//     static float        pitch_ang_vel;
//     static float        yaw_ang_vel;
//     static RSMImuFaults imu_faults;

//     void setLinearAccelerationX(float value, ExitCode status)
//     {
//         x_lin_accel                  = value;
//         imu_faults.x_lin_accel_fault = status;
//     }

//     void setLinearAccelerationY(float value, ExitCode status)
//     {
//         y_lin_accel                  = value;
//         imu_faults.y_lin_accel_fault = status;
//     }

//     void setLinearAccelerationZ(float value, ExitCode status)
//     {
//         z_lin_accel                  = value;
//         imu_faults.z_lin_accel_fault = status;
//     }

//     void setAngularVelocityRoll(float value, ExitCode status)
//     {
//         roll_ang_vel                  = value;
//         imu_faults.roll_ang_vel_fault = status;
//     }

//     void setAngularVelocityPitch(float value, ExitCode status)
//     {
//         pitch_ang_vel                  = value;
//         imu_faults.pitch_ang_vel_fault = status;
//     }

//     void setAngularVelocityYaw(float value, ExitCode status)
//     {
//         yaw_ang_vel                  = value;
//         imu_faults.yaw_ang_vel_fault = status;
//     }

// } // namespace imu

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

namespace imu
{
    class Imu {
        public: 
            struct AccelData
            {
                float x, y, z; 
            };
            struct GyroData
            {
                float x, y, z; 
            };

            ExitCode getAccelX(float &value)
            {
                value = fakes::io::imu::accel_x; 
                return fakes::io::imu::imu_faults.accel_x_fault; 
            }
            ExitCode getAccelY(float &value)
            {
                value = fakes::io::imu::accel_y; 
                return fakes::io::imu::imu_faults.accel_y_fault; 
            }
            ExitCode getAccelZ(float &value)
            {
                value = fakes::io::imu::accel_z; 
                return fakes::io::imu::imu_faults.accel_z_fault; 
            }
            ExitCode getGyroX(float &value)
            {
                value = fakes::io::imu::gyro_x; 
                return fakes::io::imu::imu_faults.gyro_x_fault; 
            }
            ExitCode getGyroY(float &value)
            {
                value = fakes::io::imu::gyro_y; 
                return fakes::io::imu::imu_faults.gyro_y_fault; 
            }
            ExitCode getGyroZ(float &value)
            {
                value = fakes::io::imu::gyro_z; 
                return fakes::io::imu::imu_faults.gyro_z_fault; 
            }
    };
}


// {
//     ExitCode getLinearAccelerationX(float *value)
//     {
//         *value = fakes::io::imu::x_lin_accel;
//         return fakes::io::imu::imu_faults.x_lin_accel_fault;
//     }

//     ExitCode getLinearAccelerationY(float *value)
//     {
//         *value = fakes::io::imu::y_lin_accel;
//         return fakes::io::imu::imu_faults.y_lin_accel_fault;
//     }

//     ExitCode getLinearAccelerationZ(float *value)
//     {
//         *value = fakes::io::imu::z_lin_accel;
//         return fakes::io::imu::imu_faults.z_lin_accel_fault;
//     }

//     ExitCode getAngularVelocityRoll(float *value)
//     {
//         *value = fakes::io::imu::roll_ang_vel;
//         return fakes::io::imu::imu_faults.roll_ang_vel_fault;
//     }

//     ExitCode getAngularVelocityPitch(float *value)
//     {
//         *value = fakes::io::imu::pitch_ang_vel;
//         return fakes::io::imu::imu_faults.pitch_ang_vel_fault;
//     }

//     ExitCode getAngularVelocityYaw(float *value)
//     {
//         *value = fakes::io::imu::yaw_ang_vel;
//         return fakes::io::imu::imu_faults.yaw_ang_vel_fault;
//     }
// } // namespace imu

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
