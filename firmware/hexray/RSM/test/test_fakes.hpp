#pragma once

namespace fakes::io
{
   namespace brake
   { 
     void setRearPressurePsi(float rear_PressurePsi);
   }

   namespace tireTemp
   {
     void set(float temp);
   }

}