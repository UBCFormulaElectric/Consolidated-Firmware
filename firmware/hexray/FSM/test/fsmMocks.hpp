#pragma once
namespace fakes::io
{
    namespace apps {
        void setPrimaryPressure(float pressure);
        void setPrimaryOCSC(bool ocsc);
        void setSecondaryPressure(float pressure);
        void setSecondaryOCSC(bool ocsc);
    } // namespace apps
    namespace brake {
        void setBrakeActuated(bool actuated);
        void setFrontPressurePsi(float psi);
        void setOCSC(bool ocsc);
    } // namespace brake
    namespace fsmShdn {
        bool BOTS_SHDN_OK_get(void);
        bool COCKPIT_SHDN_OK_get(void);
        bool FL_SHDN_OK_get(void);
        bool FR_SHDN_OK_get(void);
    }
    namespace steering {
        void setAngleDegrees(float angle);
        void setOCSC(bool o);
    } // namespace steering
    namespace suspension {
        void setLeftTravel(float travel);
        void setRightTravel(float travel);
        void setLeftSensorOCSC(bool ocsc);
        void setRightSensorOCSC(bool ocsc);
    } // namespace suspension
} // namespace fake::io