#pragma once
#include <cstdint>
#include <cstddef>

namespace vc::hw::adcs{
    template <size_t AdcBufferSize, size_t VoltageBufferSize> class AdcChannel(){
    public:
            constexpr explicit AcdChannel(*hadc2, *htim3, *adc2_raw_adc_values, *adc2_adc_voltages){
                adc2 = { .hadc            = hadc2,
                        .htim            = htim3,
                        .raw_adc_values  = adc2_raw_adc_values,
                        .adc_voltages    = adc2_adc_voltages,
                        .channel_count   = BufferSize,
                        .is_differential = false };
            }
    private: 
    static 
    uint16_t static AdcBuffer[AdcBufferSize];
    float  static VoltageBuffer[AdcBufferSize];
    };
} //namespace vc::hw::adcs