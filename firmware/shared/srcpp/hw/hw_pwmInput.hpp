#pragma once 

#include "app_utils.hpp"
#include <cstdint>
#include <array>
#include "hw_hal.hpp"
#include <cassert>

namespace hw
{

class PwmInput
{
    private :

        /* Conifguration for PWM input*/
        TIM_HandleTypeDef *htim;
        HAL_TIM_ActiveChannel tim_active_channel;
        float  tim_frequency_hz;
        uint32_t rising_edge_tim_channel;
        uint32_t falling_edge_tim_channel;
        uint32_t tim_auto_reload_reg;

        bool first_tick;

        /* Calculaing frequency based of rising edge*/
        uint32_t curr_rising_edge;
        uint32_t prev_rising_edge;

        /*Outputs of PWM input*/
        float duty_cycle;
        float frequency_hz;
        size_t tim_overflow_count;

        void setFrequency(const float curr_frequency){
            assert(frequency_hz >= 0.0f);
            frequency_hz = curr_frequency;
        };

    public:


        explicit PwmInput(TIM_HandleTypeDef *htim, HAL_TIM_ActiveChannel tim_active_channel,  float tim_frequency_hz,  uint32_t rising_edge_tim_channel,  uint32_t falling_edge_tim_channel)
        :htim(htim) , tim_active_channel(tim_active_channel), tim_frequency_hz(tim_frequency_hz), rising_edge_tim_channel(rising_edge_tim_channel), falling_edge_tim_channel(falling_edge_tim_channel)
        {
        }

        ~PwmInput();

        void init();

        float get_frequency() const;

        float get_dutyCycle() const;

        TIM_HandleTypeDef *get_timer_handle() const;

        HAL_TIM_ActiveChannel get_timer_activeChannel() const;

        bool pwm_isActive();

        void tick();

};
} //namespace hw::PwmInput