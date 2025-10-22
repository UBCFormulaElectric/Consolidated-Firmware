#include "test_fakes.hpp"
#include "io_brake.hpp"
#include "io_tireTemp.hpp"

namespace fakes::io
{
   namespace brake
   {    
        static float rearPressurePsi = 0.0f;

        void setRearPressurePsi(const float rear_PressurePsi)
        {
            rearPressurePsi = rear_PressurePsi;
        }
   }

   namespace tireTemp
   {
        static float temperature = 0.0f;

        void set(const float temp)
        {
            temperature = temp;
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

    namespace tireTemp
    {
        float get()
        {
            return fakes::io::tireTemp::temperature;
        }
    }


}