#include "app_soc.hpp"
#include "app_canTx.hpp"
#include "app_math.hpp"
#include "app_tractiveSystem.hpp"
#include "io_irs.hpp"
#include "io_socStorage.hpp"
#include "io_time.hpp"

#include <array>
#include <cmath>
#include <cstdint>

namespace
{
constexpr double MS_TO_S              = 0.001;

constexpr uint32_t V_TO_SOC_LUT_SIZE      = 201U;
constexpr float LUT_BASE_SOC              = 0.0f;
constexpr uint32_t SOC_TENTHS_UNAVAILABLE = UINT32_MAX;

constexpr uint32_t socPercentToTenths(const float soc_percent)
{
    return static_cast<uint32_t>(soc_percent * 10.0f + 0.5f);
}

// Charge in cell in coulombs.
double soc_charge_c = -1.0;

// Charge loss at time t-1.
float soc_prev_current_A = 0.0f;

// Indicates if SOC from SD was corrupt at startup.
bool soc_is_corrupt = true;

uint32_t soc_last_saved_tenths = SOC_TENTHS_UNAVAILABLE;
uint32_t soc_prev_update_ms    = 0U;

// index 0 represents 0.0% SoC, final index represents 100%. 0.5% increments.
constexpr std::array<float, V_TO_SOC_LUT_SIZE> ocv_soc_lut = {
    2.478484f, 2.548948f, 2.555629f, 2.636801f, 2.717973f, 2.795096f, 2.845899f, 2.896701f, 2.947503f, 2.979689f,
    3.009237f, 3.038785f, 3.062674f, 3.083212f, 3.103750f, 3.124092f, 3.144124f, 3.164156f, 3.183431f, 3.197409f,
    3.211386f, 3.225364f, 3.240896f, 3.256680f, 3.272465f, 3.284931f, 3.295543f, 3.306156f, 3.304352f, 3.317410f,
    3.331939f, 3.344696f, 3.356658f, 3.368619f, 3.380526f, 3.391995f, 3.403385f, 3.414583f, 3.421610f, 3.426887f,
    3.432164f, 3.436899f, 3.440721f, 3.444543f, 3.448370f, 3.452317f, 3.456320f, 3.460323f, 3.465373f, 3.470832f,
    3.476372f, 3.482455f, 3.488869f, 3.495323f, 3.501739f, 3.508092f, 3.514425f, 3.520623f, 3.526513f, 3.532403f,
    3.538241f, 3.543478f, 3.548635f, 3.553792f, 3.558754f, 3.563582f, 3.568411f, 3.573168f, 3.577760f, 3.582344f,
    3.586892f, 3.591139f, 3.595281f, 3.599423f, 3.603309f, 3.607144f, 3.610956f, 3.614661f, 3.618294f, 3.621917f,
    3.625508f, 3.629068f, 3.632628f, 3.636199f, 3.639814f, 3.643430f, 3.647046f, 3.650764f, 3.654502f, 3.658240f,
    3.662069f, 3.666019f, 3.669968f, 3.673983f, 3.678140f, 3.682339f, 3.686552f, 3.690932f, 3.695370f, 3.699882f,
    3.704528f, 3.709202f, 3.713828f, 3.718485f, 3.723182f, 3.727879f, 3.732598f, 3.737347f, 3.742095f, 3.746847f,
    3.751637f, 3.756426f, 3.761216f, 3.766030f, 3.770867f, 3.775704f, 3.780582f, 3.785519f, 3.790488f, 3.795490f,
    3.800562f, 3.805651f, 3.810753f, 3.815940f, 3.821127f, 3.826319f, 3.831529f, 3.836744f, 3.841952f, 3.847058f,
    3.852135f, 3.857211f, 3.862209f, 3.867022f, 3.871836f, 3.876640f, 3.881274f, 3.885880f, 3.890486f, 3.894997f,
    3.899466f, 3.903929f, 3.908409f, 3.912903f, 3.917498f, 3.922166f, 3.926984f, 3.931814f, 3.936749f, 3.941997f,
    3.947246f, 3.952528f, 3.958148f, 3.963841f, 3.969533f, 3.975359f, 3.981300f, 3.987241f, 3.993173f, 3.999034f,
    4.004879f, 4.010713f, 4.016239f, 4.021650f, 4.027055f, 4.032055f, 4.036909f, 4.041577f, 4.045854f, 4.049776f,
    4.053605f, 4.057009f, 4.059732f, 4.062456f, 4.065076f, 4.066936f, 4.068797f, 4.070658f, 4.072325f, 4.073932f,
    4.075540f, 4.077227f, 4.079082f, 4.080939f, 4.082910f, 4.085239f, 4.087742f, 4.090254f, 4.093629f, 4.097167f,
    4.100969f, 4.105953f, 4.111315f, 4.117198f, 4.124902f, 4.134033f, 4.143165f, 4.155217f, 4.171970f, 4.188723f,
    4.205476f,
};
} // namespace

namespace app::soc
{

#ifndef TARGET_EMBEDDED
void setPrevCurrent(const float current)
{
    soc_prev_current_A = current;
}
#endif

float getSocFromOcv(const float voltage)
{
    uint32_t lut_index = 0U;

    while ((lut_index < V_TO_SOC_LUT_SIZE) && (voltage > ocv_soc_lut[lut_index]))
    {
        ++lut_index;
    }

    if (lut_index == V_TO_SOC_LUT_SIZE)
    {
        lut_index = V_TO_SOC_LUT_SIZE - 1U;
    }

    return LUT_BASE_SOC + static_cast<float>(lut_index) * 0.5f;
}

float getOcvFromSoc(const float soc_percent)
{
    uint32_t lut_index = 0U;

    while ((lut_index < V_TO_SOC_LUT_SIZE) && ((LUT_BASE_SOC + static_cast<float>(lut_index) * 0.5f) < soc_percent))
    {
        ++lut_index;
    }

    if (lut_index == V_TO_SOC_LUT_SIZE)
    {
        lut_index = V_TO_SOC_LUT_SIZE - 1U;
    }

    return ocv_soc_lut[lut_index];
}

void init()
{
    soc_prev_current_A = 0.0f;
    soc_is_corrupt     = true;
    soc_charge_c       = -1.0;

    soc_last_saved_tenths = SOC_TENTHS_UNAVAILABLE;

    float saved_soc_percent = -1.0f;
    if (readSocFromSd(saved_soc_percent) && IS_IN_RANGE(0.0f, 100.0f, saved_soc_percent))
    {
        soc_charge_c          = static_cast<double>(saved_soc_percent / 100.0f * SERIES_ELEMENT_FULL_CHARGE_C);
        soc_is_corrupt        = false;
        soc_last_saved_tenths = socPercentToTenths(saved_soc_percent);
    }

    soc_prev_update_ms = io::time::getCurrentMs();
}

bool getCorrupt()
{
    return soc_is_corrupt;
}

void updateSocStats()
{
    const float current = app::ts::getCurrent();

    const uint32_t current_time_ms = io::time::getCurrentMs();
    const uint32_t elapsed_time_ms = current_time_ms - soc_prev_update_ms;
    soc_prev_update_ms             = current_time_ms;

    const double elapsed_time_s = static_cast<double>(elapsed_time_ms) * MS_TO_S;

    app::math::trapezoidalRule(soc_charge_c, soc_prev_current_A, current, elapsed_time_s);
}

float getMinSocCoulombs()
{
    return static_cast<float>(soc_charge_c);
}

float getMinSocPercent()
{
    return (static_cast<float>(soc_charge_c) / SERIES_ELEMENT_FULL_CHARGE_C) * 100.0f;
}

float getMinOcvFromSoc()
{
    return getOcvFromSoc(getMinSocPercent());
}

void resetSocFromVoltage()
{
    // TODO: Use minimum cell voltage once app::segments is migrated in Hexray BMS.
    soc_is_corrupt = true;
}

void resetSocCustomValue(const float soc_percent)
{
    soc_charge_c = static_cast<double>(soc_percent / 100.0f * SERIES_ELEMENT_FULL_CHARGE_C);

    soc_is_corrupt = true;
}

bool readSocFromSd(float &saved_soc_percent)
{
    return io::socStorage::read(saved_soc_percent);
}

bool writeSocToSd(const float soc_percent)
{
    const bool write_success = io::socStorage::write(soc_percent);
    if (write_success && IS_IN_RANGE(0.0f, 100.0f, soc_percent))
    {
        soc_last_saved_tenths = socPercentToTenths(soc_percent);
    }

    return write_success;
}

uint32_t getLastWrittenSocTenths()
{
    if (soc_last_saved_tenths != SOC_TENTHS_UNAVAILABLE)
    {
        return soc_last_saved_tenths;
    }

    float saved_soc_percent = -1.0f;
    if (readSocFromSd(saved_soc_percent) && IS_IN_RANGE(0.0f, 100.0f, saved_soc_percent))
    {
        soc_last_saved_tenths = socPercentToTenths(saved_soc_percent);
        return soc_last_saved_tenths;
    }

    return SOC_TENTHS_UNAVAILABLE;
}

void broadcast()
{
    if (io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED &&
        io::irs::positiveState() == app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED)
    {
        updateSocStats();
    }
}

void saveToSd()
{
    const float min_soc_percent = getMinSocPercent();
    if (IS_IN_RANGE(0.0f, 100.0f, min_soc_percent))
    {
        (void)writeSocToSd(min_soc_percent);
    }
}

} // namespace app::soc

