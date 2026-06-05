#pragma once

#include <algorithm>
#include <cmath>
#include <span>
#include "app_segments.hpp"

inline constexpr uint8_t  MAX_NUM_SEGMENTS        = 10U;
inline constexpr uint16_t VUV                     = 0x01A1; // 2.5V
inline constexpr uint16_t VOV                     = 0x0465; // 4.2V
inline constexpr float    OW_CELL_DELTA_THRESHOLD = 1.0f;   // 1 V from experience lmao

inline constexpr float V_REF2             = 3.0f;
inline constexpr float R_SERIES           = 10e3f; // Fixed resistor
inline constexpr float R_NOMINAL          = 10e3f; // Thermistor at 25C
inline constexpr float T_NOMINAL          = 298.15f;
inline constexpr float BETA_COEFF         = 3610.0f;
inline constexpr float KELVIN_OFFSET      = 273.15f;
inline constexpr float OW_THERM_THRESHOLD = 2.8f; // TODO: need to calibrate

namespace app::segments
{
constexpr float convertUVOVToFloat(const uint16_t hex)
{
    return (hex * 16 * 150e-6f + 1.5f);
}

constexpr float convertRegToVoltage(const int16_t reg)
{
    return static_cast<float>(reg) * 150e-6f + 1.5f;
}

constexpr float convertRegToTemp(const uint16_t reg)
{
    const float voltage    = convertRegToVoltage(reg);
    const float resistance = R_SERIES * (voltage / (V_REF2 - voltage));
    const float inv_temp_k = (1.0f / T_NOMINAL) + (1.0f / BETA_COEFF) * std::log(resistance / R_NOMINAL);

    return (1.0f / inv_temp_k) - KELVIN_OFFSET;
}

constexpr bool checkThermOwcOk(const uint16_t reg)
{
    return convertRegToVoltage(reg) > OW_THERM_THRESHOLD;
}

template <typename T, size_t N, void (*Send)()> class BroadcastBuffer
{
    std::span<T, N> _buf;

  public:
    template <typename CanMsg> explicit BroadcastBuffer(CanMsg &can_msg) : _buf{ reinterpret_cast<T *>(&can_msg), N }
    {
        static_assert(sizeof(CanMsg) == N * sizeof(T));
    }

    T &operator[](const size_t i) const { return _buf[i]; }

    void fill(const T &value) const { std::fill(_buf.begin(), _buf.end(), value); }

    void send() const { Send(); }
};

template <typename T, void (*Send03)(), void (*Send47)(), void (*Send89)()> class CellBroadcaster
{
    std::span<T, 4 * CELLS_PER_SEGMENT> _segments0_3;
    std::span<T, 4 * CELLS_PER_SEGMENT> _segments4_7;
    std::span<T, 2 * CELLS_PER_SEGMENT> _segments8_9;

  public:
    template <typename Seg03, typename Seg47, typename Seg89>
    CellBroadcaster(Seg03 &segments0_3_can, Seg47 &segments4_7_can, Seg89 &segments8_9_can)
      : _segments0_3{ reinterpret_cast<T *>(&segments0_3_can), 4 * CELLS_PER_SEGMENT },
        _segments4_7{ reinterpret_cast<T *>(&segments4_7_can), 4 * CELLS_PER_SEGMENT },
        _segments8_9{ reinterpret_cast<T *>(&segments8_9_can), 2 * CELLS_PER_SEGMENT }
    {
        static_assert(sizeof(Seg03) == 4 * CELLS_PER_SEGMENT * sizeof(T));
        static_assert(sizeof(Seg47) == 4 * CELLS_PER_SEGMENT * sizeof(T));
        static_assert(sizeof(Seg89) == 2 * CELLS_PER_SEGMENT * sizeof(T));
    }

    std::span<T, CELLS_PER_SEGMENT> operator[](const size_t seg) const
    {
        if (seg < 4)
        {
            return std::span<T, CELLS_PER_SEGMENT>{ _segments0_3.data() + seg * CELLS_PER_SEGMENT, CELLS_PER_SEGMENT };
        }
        if (seg < 8)
        {
            return std::span<T, CELLS_PER_SEGMENT>{ _segments4_7.data() + (seg - 4) * CELLS_PER_SEGMENT,
                                                    CELLS_PER_SEGMENT };
        }
        return std::span<T, CELLS_PER_SEGMENT>{ _segments8_9.data() + (seg - 8) * CELLS_PER_SEGMENT,
                                                CELLS_PER_SEGMENT };
    }

    void fill(const T &value) const
    {
        std::fill(_segments0_3.begin(), _segments0_3.end(), value);
        std::fill(_segments4_7.begin(), _segments4_7.end(), value);
        std::fill(_segments8_9.begin(), _segments8_9.end(), value);
    }

    void send() const
    {
        Send03();
        Send47();
        Send89();
    }
};
} // namespace app::segments
