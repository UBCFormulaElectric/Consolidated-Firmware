#include "io_irs.hpp"

constexpr float IR_TOTAL_ISENSE_SHUNT_R_OHMS = 0.075f;
constexpr float IR_TOTAL_ISENSE_AMPLIFIER = 20.0f;
constexpr float IR_TOTAL_ISENSE_V_TO_A  = (1.0f / (IR_TOTAL_ISENSE_SHUNT_R_OHMS * IR_TOTAL_ISENSE_AMPLIFIER));
constexpr float IR_POSITIVE_CLOSED_THRESH_A = 0.5f;
constexpr float IR_LOOP_GAIN = (1.0f / 20.0f);
constexpr float IR_LOOP_SHUNT_RES = (1.0f / 75.0e-3f);

namespace io::irs{
    ContactorState negativeState(){
       
    }

    void setPositive(const ContactorState state){
        
    }

    ContactorState positiveState(){
        
    }

    void setPrecharge(const ContactorState state){
        
    }

    ContactorState prechargeState(){
        
    }
}