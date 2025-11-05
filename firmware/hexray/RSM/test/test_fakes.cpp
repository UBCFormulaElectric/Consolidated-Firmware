#include "test_fakes.hpp"
#include "io_brake.hpp"
#include "io_coolant.hpp"
#include "io_suspension.hpp"
#include "io_tireTemp.hpp"
#include "util_errorCodes.hpp"

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

        void setLinearAccelerationX(float value)
        {
            x_lin_accel = value;
        }

        void setLinearAccelerationY(float value)
        {
            y_lin_accel = value;
        }

        void setLinearAccelerationZ(float value)
        {
            z_lin_accel = value;
        }

        void setAngularVelocityRoll(float value)
        {
            roll_ang_vel = value;
        }

        void setAngularVelocityPitch(float value)
        {
            pitch_ang_vel = value;
        }

        void setAngularVelocityYaw(float value)
        {
            yaw_ang_vel = value;
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

    namespace imu
    {
        void getLinearAccelerationX(float *value)
        {
            *value = fakes::io::imu::x_lin_accel;
        }

        void getLinearAccelerationY(float *value)
        {
            *value = fakes::io::imu::y_lin_accel;
        }

        void getLinearAccelerationZ(float *value)
        {
            *value = fakes::io::imu::z_lin_accel;
        }

        void getAngularVelocityRoll(float *value)
        {
            *value = fakes::io::imu::roll_ang_vel;
        }

        void getAngularVelocityPitch(float *value)
        {
            *value = fakes::io::imu::pitch_ang_vel;
        }

        void getAngularVelocityYaw(float *value)
        {
            *value = fakes::io::imu::yaw_ang_vel;
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