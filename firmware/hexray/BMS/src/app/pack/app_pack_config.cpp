#include <cmath>

#include "app_pack.hpp"
#include "app_pack_internal.hpp"

namespace
{

constexpr float UVOV_LSB_V    = 16.0f * 150e-6f;
constexpr float UVOV_OFFSET_V = 1.5f;

constexpr io::adbms::Segments<io::adbms::CFGA> createConfigA()
{
    io::adbms::Segments<io::adbms::CFGA> config{};
    for (io::adbms::CFGA &reg_a : config)
    {
        reg_a.cth       = 0x01;
        reg_a.ref_on    = 0x01;
        reg_a.flag_d    = 0x00;
        reg_a.owa       = 0x00;
        reg_a.owrng     = 0x00;
        reg_a.soak_on   = 0x00;
        reg_a.gpio_1_8  = 0xFF;
        reg_a.gpio_9_10 = 0x03;
        reg_a.fc        = 0x03;
        reg_a.comm_bk   = 0x00;
        reg_a.mute_st   = 0x01;
        reg_a.snap_st   = 0x00;

    }
    return config;
}

io::adbms::Segments<io::adbms::CFGA> config_a = createConfigA();
io::adbms::Segments<io::adbms::CFGB> config_b{};
io::adbms::Segments<io::adbms::PWMA> pwm_a{};
io::adbms::Segments<io::adbms::PWMB> pwm_b{};
} // namespace

namespace app::pack::config
{
result<void> setSegmentConfig(const io::adbms::ThermistorMux mux)
{
    const auto vuv = static_cast<uint16_t>(std::lround((V_FAULT_UV - UVOV_OFFSET_V) / UVOV_LSB_V));
    const auto vov = static_cast<uint16_t>(std::lround((V_FAULT_OV - UVOV_OFFSET_V) / UVOV_LSB_V));

    for (uint8_t seg = 0; seg < io::adbms::NUM_SEGMENTS; seg++)
    {
        config_a[seg].gpio_1_8  = 0xFF;
        config_a[seg].gpio_9_10 = 0x2 | (mux == io::adbms::ThermistorMux::THERM_8_13);
        config_b[seg].vuv_0_7  = static_cast<uint8_t>(vuv & 0xFF);
        config_b[seg].vuv_8_11 = static_cast<uint8_t>(vuv >> 8 & 0x0F);
        config_b[seg].vov_0_3  = static_cast<uint8_t>(vov & 0x0F);
        config_b[seg].vov_4_11 = static_cast<uint8_t>(vov >> 4 & 0xFF);
        config_b[seg].dcto_0_5 = 0x00;
        config_b[seg].dtrng = 0x00;
        config_b[seg].dtmen = 0x00;
        config_b[seg].dcc_1_8 = 0x00;
        config_b[seg].dcc_9_16 = 0x00;
    }

    return io::adbms::write::configReg(config_a, config_b);
}

result<void> setPWMConfig(const io::adbms::PackCells<uint8_t> &duty)
{
    for (uint8_t seg = 0; seg < io::adbms::NUM_SEGMENTS; seg++)
    {
        const auto &d = duty[seg];
        pwm_a[seg]    = { static_cast<uint8_t>(d[0] & 0x0F), static_cast<uint8_t>(d[1] & 0x0F),
                          static_cast<uint8_t>(d[2] & 0x0F), static_cast<uint8_t>(d[3] & 0x0F),
                          static_cast<uint8_t>(d[4] & 0x0F), static_cast<uint8_t>(d[5] & 0x0F),
                          static_cast<uint8_t>(d[6] & 0x0F), static_cast<uint8_t>(d[7] & 0x0F),
                          static_cast<uint8_t>(d[8] & 0x0F), static_cast<uint8_t>(d[9] & 0x0F),
                          static_cast<uint8_t>(d[10] & 0x0F), static_cast<uint8_t>(d[11] & 0x0F) };
        pwm_b[seg] = { static_cast<uint8_t>(d[12] & 0x0F), static_cast<uint8_t>(d[13] & 0x0F), 0, 0, 0xFFFFFFFFu };
    }

    return io::adbms::write::pwmReg(pwm_a, pwm_b);
}

result<void> checkSegmentConfig()
{
    io::adbms::Segments<result<io::adbms::CFGA>> readback_a;
    io::adbms::Segments<result<io::adbms::CFGB>> readback_b;
    RETURN_IF_ERR(io::adbms::read::configReg(readback_a, readback_b));

    for (size_t seg = 0; seg < io::adbms::NUM_SEGMENTS; seg++)
    {
        if (!readback_a[seg] || !readback_b[seg])
        {
            const ErrorCode error = readback_a[seg] ? readback_b[seg].error() : readback_a[seg].error();
            LOG_WARN("Config readback failed on seg %u: %s", static_cast<unsigned>(seg), error_code_to_string(error));
            return std::unexpected(error);
        }
        if (*readback_a[seg] != config_a[seg] || *readback_b[seg] != config_b[seg])
        {
            LOG_WARN("Config mismatch on seg %u", static_cast<unsigned>(seg));
            return std::unexpected(ErrorCode::MISMATCH);
        }
    }

    return {};
}

result<void> checkPWMConfig()
{
    io::adbms::Segments<result<io::adbms::PWMA>> readback_a;
    io::adbms::Segments<result<io::adbms::PWMB>> readback_b;
    RETURN_IF_ERR(io::adbms::read::pwmReg(readback_a, readback_b));

    for (size_t seg = 0; seg < io::adbms::NUM_SEGMENTS; seg++)
    {
        if (!readback_a[seg] || !readback_b[seg])
        {
            const ErrorCode error = readback_a[seg] ? readback_b[seg].error() : readback_a[seg].error();
            LOG_WARN("PWM readback failed on seg %u: %s", static_cast<unsigned>(seg), error_code_to_string(error));
            return std::unexpected(error);
        }
        if (*readback_a[seg] != pwm_a[seg] || *readback_b[seg] != pwm_b[seg])
        {
            LOG_WARN("PWM mismatch on seg %u", static_cast<unsigned>(seg));
            return std::unexpected(ErrorCode::MISMATCH);
        }
    }

    return {};
}
} // namespace app::pack::config
