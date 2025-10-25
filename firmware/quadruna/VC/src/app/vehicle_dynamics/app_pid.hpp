#pragma once
#include <cstdint>
#include <algorithm> // for std::clamp

namespace app
{
    class PID 
    {
        public:
            struct Config {
                float Kp;
                float Ki;
                float Kd;
                float Kb;
                float N;
                float out_min;
                float out_max;
                bool back_calculation;
                uint32_t sample_time;
            };

        explicit PID(const Config& conf);

        float compute(float setpoint, float input);
        void reset();

        private:
            // Gains
            float Kp;
            float Ki;
            float Kd;
            float Kb;
            float N;

            // Limits
            float out_min;
            float out_max;

            // Internal state
            float error = 0.0f;
            float filtered_derivative = 0.0f;
            float integral = 0.0f;
            float prev_input = 0.0f;
            float prev_output = 0.0f;
            float prev_derivative = 0.0f;

            bool back_calculation = false;

            // Timing
            uint32_t prev_time = 0;
            uint32_t sample_time = 0;
    };
}