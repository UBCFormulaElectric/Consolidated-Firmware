#pragma once

namespace fakes::io
{
   namespace brake
   { 
      void setRearPressurePsi(float rear_PressurePsi);
   }

   namespace coolant
   {
      float setFlowRate(float flow_rate);
   }

   namespace suspension
   {
      void setRLTravel(float value);
      void setRRTravel(float value);
      void setRR_OCSC(bool value);
      void setRL_OCSC(bool value);
   }

   namespace tireTemp
   {
     void set(float temp);
   }

}