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
   }

   namespace coolant
   {
        static float flowRate = 0.0f;
        void setFlowRate(float value)
        {
            flowRate = value;
        }
   }

   namespace suspension
   {
        static float RLTravel = 0.0f;
        static float RRTravel = 0.0f;
        static bool RROCSC  = false;
        static bool RLOCSC = false;

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
   }

   namespace imu 
   {
        static float x_lin_accel;
        static float y_lin_accel;
        static float z_lin_accel;
        static float roll_ang_vel;
        static float pitch_ang_vel;
        static float yaw_ang_vel;
        static RSMImuFaults imu_faults;  
        

        void setLinearAccelerationX(float value, ExitCode status)
        {
            x_lin_accel = value;
            imu_faults.x_lin_accel_fault = status;
        }

        void setLinearAccelerationY(float value, ExitCode status)
        {
            y_lin_accel = value;
            imu_faults.y_lin_accel_fault = status;
        }

        void setLinearAccelerationZ(float value, ExitCode status)
        {
            z_lin_accel = value;
            imu_faults.z_lin_accel_fault = status;
        }

        void setAngularVelocityRoll(float value, ExitCode status)
        {
            roll_ang_vel = value;
            imu_faults.roll_ang_vel_fault = status;
            
        }

        void setAngularVelocityPitch(float value, ExitCode status)
        {
            pitch_ang_vel = value;
            imu_faults.pitch_ang_vel_fault = status;
        }

        void setAngularVelocityYaw(float value, ExitCode status)
        {
            yaw_ang_vel = value;
            imu_faults.yaw_ang_vel_fault = status;
        }

   }

   namespace tireTemp
   {
        static float temperature = 0.0f;

        void set(const float value)
        {
            temperature = value;
        }
   }
}

namespace io 
{
    namespace brake
    {
        float getRearPressurePsi()
        {
            return fakes::io::brake::rearPressurePsi;
        }
    }

    namespace coolant
    {
        float getFlowRate()
        {
            return fakes::io::coolant::flowRate;
        }
    }

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
    }

    namespace rPump
    {
        ExitCode setPercentage(float value)
        {
            return ExitCode::EXIT_CODE_OK;
        }
    }

    namespace tireTemp
    {
        float get()
        {
            return fakes::io::tireTemp::temperature;
        }
    }
}


ExitCode io_imu_getLinearAccelerationX(float *value)
{
    *value = fakes::io::imu::x_lin_accel;
    return fakes::io::imu::imu_faults.x_lin_accel_fault;
}

ExitCode io_imu_getLinearAccelerationY(float *value)
{
    *value = fakes::io::imu::y_lin_accel;
    return fakes::io::imu::imu_faults.y_lin_accel_fault;
}

ExitCode io_imu_getLinearAccelerationZ(float *value)
{
    *value = fakes::io::imu::z_lin_accel;
    return fakes::io::imu::imu_faults.z_lin_accel_fault;
}

ExitCode io_imu_getAngularVelocityRoll(float *value)
{
    *value = fakes::io::imu::roll_ang_vel;
    return fakes::io::imu::imu_faults.roll_ang_vel_fault;
}

ExitCode io_imu_getAngularVelocityPitch(float *value)
{
    *value = fakes::io::imu::pitch_ang_vel;
    return fakes::io::imu::imu_faults.pitch_ang_vel_fault;
}

ExitCode io_imu_getAngularVelocityYaw(float *value)
{
    *value = fakes::io::imu::yaw_ang_vel;
    return fakes::io::imu::imu_faults.yaw_ang_vel_fault;
}

