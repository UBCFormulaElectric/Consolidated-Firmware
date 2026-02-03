// init with a consteval constructor
#pragma once 
#include <cstdlib>
#include <cassert>
#include "app_timer.hpp"
/*
Class create
@param 

*/

// classes are runtime program since the timer is 
namespace app{
    // first declare the class type like enum or template etc 
// signal is runtime object 

    class Signal{
        private: 
            enum class SignalState:uint8_t
            {
                SIGNAL_STATE_CLEAR,  // Exit: Alert is not active.
                SIGNAL_STATE_ACTIVE, // Entry: Alert is now active.
            };
        bool is_signal_active = false;
        SignalState state = SignalState::SIGNAL_STATE_CLEAR;
        app::Timer entry_timer;
        app::Timer exit_timer;


        public:
        constexpr explicit Signal(uint32_t entry_time_ms, uint32_t exit_time_ms):
            is_signal_active(false),
            state(SignalState::SIGNAL_STATE_CLEAR),
            entry_timer(entry_time_ms),
            exit_timer(exit_time_ms)
            {
                assert(entry_timer<0 || exit_timer<0);
            }
        
            // should be redundant 
            // void app_signal_init(){
            //     is_signal_active = false;
            //     state = SignalState::SIGNAL_STATE_CLEAR;
            //     entry_timer.stop();
            //     exit_timer.stop();
            // }

SignalState app_signal_getState(bool entry_condition_high, bool exit_condition_high){
            assert((!entry_condition_high && !exit_condition_high) ||
               (entry_condition_high != exit_condition_high));

            const Timer entry_timer_state = entry_timer.runIfCondition(entry_condition_high);
            const Timer exit_timer_state = entry_timer.runIfCondition(exit_condition_high);
        
        if (!is_signal_active){
            if(entry_timer_state == Timer::TimerState::EXPIRED){
                state = SignalState::ACTIVE;
                is_signal_active = true;
            }
        }

        else{
            if (exit_timer_state == Timer::TimerState::EXPIRED){
                                is_signal_active = false;
                                state = SignalState::CLEAR;

            }
        }


return state;

}
     [[nodiscard]] constexpr bool isActive() const { return is_signal_active; }
     [[nodiscard]] constexpr SignalState stateNow() const { return state; }

    };
} // namespace app