#include "test_fakes.hpp"
#include "io_brake.hpp"
#include "io_coolant.hpp"
#include "io_suspension.hpp"
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

   namespace coolant
   {
        static float flowRate = 0.0f;
        void setFlowRate(float flow_rate)
        {
            flowRate = flow_rate;
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

        bool getRL_OCSC()
        {
            return fakes::io::suspension::RLOCSC;
        }

        bool getRR_OCSC()
        {
            return fakes::io::suspension::RROCSC;
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