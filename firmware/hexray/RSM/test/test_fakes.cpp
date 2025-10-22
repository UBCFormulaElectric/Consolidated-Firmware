#include "test_fakes.hpp"
#include "app_brake.hpp"
#include "app_tireTemp.hpp"

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
            io::tireTemp::temperature = temp;
        }

        float get()
        {
            return(io::tireTemp::temperature);
        }
   }

}

namespace io {
    namespace brake{
        float getRearPressurePsi()
        {
            return fakes::io::brake::rearPressurePsi;
        }

        bool isActuated()
        {
        #define BRAKE_ACTUATED_THRESHOLD_PSI 200.0f
            return fakes::io::brake::getRearPressurePsi() > BRAKE_ACTUATED_THRESHOLD_PSI;
        }
    }
}