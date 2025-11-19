#pragma once
#include "util_errorCodes.hpp"

namespace fakes::io
{
   namespace brake
   { 
      void setRearPressurePsi(float rear_PressurePsi);
   }

   namespace coolant
   {
      void setFlowRate(float flow_rate);
   }

   namespace suspension
   {
      void setRLTravel(float value);
      void setRRTravel(float value);
      void setRR_OCSC(bool value);
      void setRL_OCSC(bool value);
   }

   namespace imu
   {
      void setLinearAccelerationX(float value, ExitCode status);
      void setLinearAccelerationY(float value, ExitCode status);
      void setLinearAccelerationZ(float value, ExitCode status);
      void setAngularVelocityRoll(float value, ExitCode status);
      void setAngularVelocityPitch(float value, ExitCode status);
      void setAngularVelocityYaw(float value, ExitCode status);
   }

   namespace rPump
   {
      void setStatus(ExitCode value); 
   }

   namespace tireTemp
   {
     void set(float temp);
   }

}