#pragma once

namespace fakes::io
{
   namespace brake
   { 
     float getRearPressurePsi();
     bool isActuated();
   }

   namespace tireTemp
   {
     float set();
   }

}