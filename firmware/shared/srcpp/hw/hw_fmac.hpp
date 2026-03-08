#pragma once

extern "C"
{
#include "hw_hal.hpp"
#ifndef HAL_FMAC_MODULE_ENABLED
#error "HAL_FMAC_MODULE_ENABLED must be defined and set to 1"
#endif
} // gracefully handle the case where the module is not enabled, but fail to compile if it is not defined. (Prevents future linker errors if silently ignoring the error)

#include "util_errorCodes.hpp"

namespace hw::fmac
{

// IIR filter limits from FMAC reference manual (RM0481)
// P = feedforward (B) tap count, Q = feedback (A) tap count, R = output gain exponent
// Using english names for the variables to match the reference manual but increasing readability
constexpr uint8_t IIR_MIN_B_TAPS = 2;   // P min
constexpr uint8_t IIR_MAX_B_TAPS = 64;  // P max
constexpr uint8_t IIR_MIN_A_TAPS = 1;   // Q min
constexpr uint8_t IIR_MAX_A_TAPS = 63;  // Q max
constexpr uint8_t IIR_MIN_GAIN = 0;     // R min (gain = 2^R)
constexpr uint8_t IIR_MAX_GAIN = 7;     // R max

struct FmacIirConfig
{
    uint8_t numBTaps;
    uint8_t numATaps;
    uint8_t gainExponent;
    const int16_t* coefB;
    const int16_t* coefA;
};

class FmacIir
{
  public:
    explicit FmacIir(FMAC_HandleTypeDef* handle) : m_handle(handle) {}
    std::expected<void, ErrorCode> init(const FmacIirConfig& config);
    std::expected<void, ErrorCode> process(int16_t sample_q15, int16_t* output_q15);
    std::expected<void, ErrorCode> stop();

  private:
    FMAC_HandleTypeDef* m_handle;
    int16_t m_outputSample = 0;
    uint16_t m_outputSize = 1;
    std::expected<void, ErrorCode> pushSample(int16_t sample_q15);
    std::expected<void, ErrorCode> popSample(int16_t* output_q15);
};

// Helper functions
// Multiply by 32768 and cast to int16_t
inline int16_t floatToQ15(float x){
    return static_cast<int16_t>(x * 32768.0f);
} 

// Cast to float and divide by 32768
inline float q15ToFloat(int16_t x){
    return static_cast<float>(x) / 32768.0f;
}

} // namespace hw::fmac
