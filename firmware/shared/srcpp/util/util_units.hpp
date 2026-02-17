#pragma once
#include <cmath>
#include <cstdint>

// =============================================================================
// MATHEMATICAL CONSTANTS
// =============================================================================

inline constexpr float M_PI_F = static_cast<float>(M_PI);

// =============================================================================
// TIME CONVERSIONS
// =============================================================================

// Convert seconds to milliseconds
inline constexpr uint32_t S_TO_MS(float time_s)
{
    return static_cast<uint32_t>(time_s * 1000.0f);
}

inline constexpr float MIN_TO_HOUR = 60.0f;

// Convert seconds to hours
inline constexpr float SECONDS_TO_HOUR(float s)
{
    return s * (1.0f / 3600.0f);
}

// Convert frequency (Hz) to period (ms)
inline constexpr uint32_t HZ_TO_MS(uint32_t hz)
{
    return 1000U / hz;
}

// =============================================================================
// LENGTH CONVERSIONS
// =============================================================================

inline constexpr float INCH_TO_KM = 0.0000254f;
inline constexpr float IN_TO_M    = 0.0254f;
inline constexpr float MM_TO_M    = 0.001f;
inline constexpr float CM_TO_M    = 0.01f;

// =============================================================================
// ANGULAR UNIT CONVERSIONS
// =============================================================================

// Convert radians to degrees
inline constexpr float RAD_TO_DEG(float radians)
{
    return radians * 180.0f / M_PI_F;
}

// Convert degrees to radians
inline constexpr float DEG_TO_RAD(float degrees)
{
    return degrees * M_PI_F / 180.0f;
}

// Convert RPM to radians per second
inline constexpr float RPM_TO_RADS(float rpm)
{
    return rpm * M_PI_F / 30.0f;
}

// =============================================================================
// VEHICLE-SPECIFIC CONSTANTS
// =============================================================================

inline constexpr float WHEEL_DIAMETER_IN = 18.0f;   // Wheel diameter in inches
inline constexpr float GEAR_RATIO        = 14.3f; // Verified by Noah

// =============================================================================
// VELOCITY CONVERSIONS
// =============================================================================

// Convert motor RPM to vehicle speed (km/h)
inline constexpr float MOTOR_RPM_TO_KMH(float rpm)
{
    return rpm * WHEEL_DIAMETER_IN * M_PI_F * INCH_TO_KM * MIN_TO_HOUR / GEAR_RATIO;
}

// Convert vehicle speed (km/h) to motor RPM
inline constexpr int MOTOR_KMH_TO_RPM(float kmh)
{
    return static_cast<int>(kmh / (WHEEL_DIAMETER_IN * M_PI_F * INCH_TO_KM * MIN_TO_HOUR / GEAR_RATIO));
}

// Convert vehicle speed (km/h) to wheel RPM (no gear ratio)
inline constexpr int WHEEL_KMH_TO_RPM(float kmh)
{
    return static_cast<int>(kmh / (WHEEL_DIAMETER_IN * M_PI_F * INCH_TO_KM * MIN_TO_HOUR));
}

// Convert meters per second to kilometers per hour
inline constexpr float MPS_TO_KMH(float mps)
{
    return mps * 3.6f;
}

// Convert kilometers per hour to meters per second
inline constexpr float KMH_TO_MPS(float kmh)
{
    return kmh / 3.6f;
}