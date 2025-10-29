#pragma once

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
      void setLinearAccelerationX(float value);
      void setLinearAccelerationY(float value);
      void setLinearAccelerationZ(float value);
      void setAngularVelocityRoll(float value);
      void setAngularVelocityPitch(float value);
      void setAngularVelocityYaw(float value);
   }

   
   namespace tireTemp
   {
     void set(float temp);
   }

}