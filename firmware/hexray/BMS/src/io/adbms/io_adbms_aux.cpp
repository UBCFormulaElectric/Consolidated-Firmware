#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "io_log.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"
#include "util_retry.hpp"

using namespace std;

namespace
{
constexpr uint8_t GPIOS_PER_GROUP      = 3U;
constexpr uint8_t NUM_THERM_REG_GROUPS = 3U;
constexpr uint8_t NUM_STAT_REG_GROUPS  = 5U;

} // namespace

namespace io::adbms
{

// check this
result<void> clear::aux()
{
    constexpr Segments<RegBuffer> clr_regs{};
    RETURN_IF_ERR(sendCmd(CLRAUX));
    RETURN_IF_ERR(writeRegGroup(CLRFLAG, clr_regs));
    RETURN_IF_ERR(writeRegGroup(CLOVUV, clr_regs));
    return {};
}

// check this
result<void> clear::stat()
{
    constexpr Segments<RegBuffer> clr_regs = []()
    {
        Segments<RegBuffer> r{};
        for (auto &seg : r)
            seg.fill(0xFF);
        return r;
    }();
    RETURN_IF_ERR(writeRegGroup(CLRFLAG, clr_regs));
    RETURN_IF_ERR(writeRegGroup(CLOVUV, clr_regs));
    return {};
}

result<void> command::startAuxAdc()
{
    return sendCmd(ADAX_BASE);
}

result<void> command::pollAuxAdc()
{
    uint32_t attempt = 0;
    return util::retry(
        [&attempt]() -> result<void>
        {
            ++attempt;
            LOG_INFO(
                "pollAuxAdc retry attempt %lu/%lu", static_cast<unsigned long>(attempt),
                static_cast<unsigned long>(POLL_RETRIES));
            const auto rx_res = poll(PLAUX);
            if (!rx_res)
                return unexpected(rx_res.error());
            if (rx_res.value().to_ulong() == POLL_STATUS_READY)
                return {};
            io::time::delay(POLL_RETRY_DELAY_MS);
            return unexpected(ErrorCode::BUSY);
        },
        20);
}

ThermGpios<result<int16_t>> read::thermGpioVoltage()
{
    constexpr array<uint16_t, NUM_THERM_REG_GROUPS> reg_groups{ { RDAUXA, RDAUXB, RDAUXC } };
    ThermGpios<result<int16_t>>                     cell_temp_regs{};

    for (size_t group = 0U; group < NUM_THERM_REG_GROUPS; group++)
    {
        const Segments<result<RegBuffer>> temp_reg_groups = readRegGroup(reg_groups[group]);

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t gpio_in_group = 0U; gpio_in_group < GPIOS_PER_GROUP; gpio_in_group++)
            {
                if (const size_t gpio = group * GPIOS_PER_GROUP + gpio_in_group; gpio < THERM_GPIOS_PER_SEGMENT)
                {
                    if (!temp_reg_groups[seg])
                    {
                        cell_temp_regs[seg][gpio] = unexpected(temp_reg_groups[seg].error());
                        continue;
                    }

                    const uint16_t low  = temp_reg_groups[seg].value()[gpio_in_group * 2U];
                    const uint16_t high = temp_reg_groups[seg].value()[gpio_in_group * 2U + 1U];
                    const auto     aux_register_val =
                        static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

                    if (aux_register_val == DEFAULT_REGISTER_VALUE)
                    {
                        cell_temp_regs[seg][gpio] = std::unexpected(ErrorCode::INVALID_READING);
                        continue;
                    }
                    cell_temp_regs[seg][gpio] = aux_register_val;
                }
            }
        }
    }
    return cell_temp_regs;
}

Segments<result<int16_t>> read::segVoltage()
{
    Segments<result<int16_t>> segment_voltage_regs{};

    const Segments<result<RegBuffer>> raw_seg_voltage = readRegGroup(RDAUXD);
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (!raw_seg_voltage[seg])
        {
            segment_voltage_regs[seg] = unexpected(raw_seg_voltage[seg].error());
            continue;
        }

        // we are reading VPV
        const uint8_t low  = raw_seg_voltage[seg].value()[4U];
        const uint8_t high = raw_seg_voltage[seg].value()[5U];
        const auto    voltage_reg_val =
            static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

        if (voltage_reg_val == DEFAULT_REGISTER_VALUE)
        {
            segment_voltage_regs[seg] = std::unexpected(ErrorCode::INVALID_READING);
            continue;
        }
        segment_voltage_regs[seg] = voltage_reg_val;
    }
    return segment_voltage_regs;
}

Segments<StatusGroupsRes> read::status()
{
    constexpr array<uint16_t, NUM_STAT_REG_GROUPS> reg_groups{ { RDSTATA, RDSTATB, RDSTATC, RDSTATD, RDSTATE } };
    Segments<StatusGroupsRes>                      stat_regs;

    for (size_t group = 0U; group < NUM_STAT_REG_GROUPS; group++)
    {
        const Segments<result<RegBuffer>> status_reg_buf = readRegGroup(reg_groups[group]);
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            switch (group)
            {
                case 0U:
                    stat_regs[seg].stat_a = status_reg_buf[seg]
                                                ? *reinterpret_cast<const STATA *>(status_reg_buf[seg]->data())
                                                : result<STATA>{ unexpected(status_reg_buf[seg].error()) };
                    break;
                case 1U:
                    stat_regs[seg].stat_b = status_reg_buf[seg]
                                                ? *reinterpret_cast<const STATB *>(status_reg_buf[seg]->data())
                                                : result<STATB>{ unexpected(status_reg_buf[seg].error()) };
                    break;
                case 2U:
                    stat_regs[seg].stat_c = status_reg_buf[seg]
                                                ? *reinterpret_cast<const STATC *>(status_reg_buf[seg]->data())
                                                : result<STATC>{ unexpected(status_reg_buf[seg].error()) };
                    break;
                case 3U:
                    stat_regs[seg].stat_d = status_reg_buf[seg]
                                                ? *reinterpret_cast<const STATD *>(status_reg_buf[seg]->data())
                                                : result<STATD>{ unexpected(status_reg_buf[seg].error()) };
                    break;
                case 4U:
                    stat_regs[seg].stat_e = status_reg_buf[seg]
                                                ? *reinterpret_cast<const STATE *>(status_reg_buf[seg]->data())
                                                : result<STATE>{ unexpected(status_reg_buf[seg].error()) };
                    break;
                default:
                    break;
            }
        }
    }
    return stat_regs;
}

} // namespace io::adbms
