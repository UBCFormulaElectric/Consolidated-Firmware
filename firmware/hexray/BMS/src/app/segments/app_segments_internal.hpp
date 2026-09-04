#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <span>
#include "app_segments.hpp"

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

template <typename T, size_t SEGMENTS_PER_MSG, void (*...Sends)()> class CellBroadcaster
{
    static_assert(SEGMENTS_PER_MSG * sizeof...(Sends) >= MAX_NUM_SEGMENTS);

    // One pointer per CAN message, each covering SEGMENTS_PER_MSG segments (the last may cover fewer).
    std::array<T *, sizeof...(Sends)> _msgs;

  public:
    template <typename... CanMsgs>
    explicit CellBroadcaster(CanMsgs &...can_msgs) : _msgs{ { reinterpret_cast<T *>(&can_msgs)... } }
    {
        static_assert(sizeof...(CanMsgs) == sizeof...(Sends));
        static_assert((sizeof(CanMsgs) + ...) == MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT * sizeof(T));
    }

    std::span<T, CELLS_PER_SEGMENT> operator[](const size_t seg) const
    {
        return std::span<T, CELLS_PER_SEGMENT>{
            _msgs[seg / SEGMENTS_PER_MSG] + (seg % SEGMENTS_PER_MSG) * CELLS_PER_SEGMENT, CELLS_PER_SEGMENT
        };
    }

    void fill(const T &value) const
    {
        for (size_t seg = 0U; seg < MAX_NUM_SEGMENTS; seg++)
        {
            const auto cells = (*this)[seg];
            std::fill(cells.begin(), cells.end(), value);
        }
    }

    void send() const { (Sends(), ...); }
};
} // namespace app::segments
