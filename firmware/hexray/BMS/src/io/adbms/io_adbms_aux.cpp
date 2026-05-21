#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
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

//check this 
result<void> clear::aux()
{
    constexpr Segments<RegBuffer> clr_regs{};
    RETURN_IF_ERR(sendCmd(CLRAUX));
    RETURN_IF_ERR(writeRegGroup(CLRFLAG, clr_regs));
    RETURN_IF_ERR(writeRegGroup(CLOVUV, clr_regs));
    return {};
}

//check this
result<void> clear::stat()
{
    constexpr Segments<RegBuffer> clr_regs{};
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
    return util::retry(
        []() -> result<void>
        {
            const auto rx_res = poll(PLAUX);
            if (!rx_res) return unexpected(rx_res.error());
            if (rx_res.value().to_ulong() == POLL_STATUS_READY) return {};
            io::time::delay(POLL_RETRY_DELAY_MS);
            return unexpected(ErrorCode::BUSY);
        },
        MAX_NUM_ATTEMPTS,
        ErrorCode::BUSY,
        ErrorCode::TIMEOUT);
}

ThermGpios<result<uint16_t>> read::thermGpioVoltage()
{
    constexpr array<uint16_t, NUM_THERM_REG_GROUPS> reg_groups{ { RDAUXA, RDAUXB, RDAUXC } };
    ThermGpios<result<uint16_t>>                        cell_temp_regs{};

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
                    const auto     temperature =
                        static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

                    if (temperature == 0xFFFF || temperature == 0x8000)
                    {
                        cell_temp_regs[seg][gpio] = std::unexpected(ErrorCode::INVALID_READING);
                        continue;
                    }
                    cell_temp_regs[seg][gpio] = temperature;
                }
            }
        }
    }
    return cell_temp_regs;
}

Segments<result<uint16_t>> read::segVoltage()
{
    Segments<result<uint16_t>> segment_voltage_regs{};

    const Segments<result<RegBuffer>> raw_seg_voltage = readRegGroup(RDAUXD);
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (!raw_seg_voltage[seg])
        {
            segment_voltage_regs[seg] = unexpected(raw_seg_voltage[seg].error());
            continue;
        }

        // we are reading VPV
        const uint8_t low     = raw_seg_voltage[seg].value()[4U];
        const uint8_t high    = raw_seg_voltage[seg].value()[5U];
        const auto    voltage = static_cast<uint16_t>(static_cast<uint16_t>(low) | static_cast<uint16_t>(high) << 8U);

        if (voltage == 0xFFFF || voltage == 0x8000)
        {
            segment_voltage_regs[seg] = std::unexpected(ErrorCode::INVALID_READING);
            continue;
        }
        segment_voltage_regs[seg] = voltage;
    }
    return segment_voltage_regs;
}

Segments<StatusGroups> read::status()
{
    constexpr array<uint16_t, NUM_STAT_REG_GROUPS> reg_groups{ { RDSTATA, RDSTATB, RDSTATC, RDSTATD, RDSTATE } };
    Segments<StatusGroups>                         stat_regs;

    for (size_t group = 0U; group < NUM_STAT_REG_GROUPS; group++)
    {
        const auto out = readRegGroup(reg_groups[group]);
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++) {

            switch (group)
            {
                case 0U:
                    stat_regs[seg].stat_a = out[seg] ? *reinterpret_cast<const STATA *>(out[seg]->data())
                                                     : result<STATA>{ unexpected(out[seg].error()) };
                    break;
                case 1U:
                    stat_regs[seg].stat_b = out[seg] ? *reinterpret_cast<const STATB *>(out[seg]->data())
                                                     : result<STATB>{ unexpected(out[seg].error()) };
                    break;
                case 2U:
                    stat_regs[seg].stat_c = out[seg] ? *reinterpret_cast<const STATC *>(out[seg]->data())
                                                     : result<STATC>{ unexpected(out[seg].error()) };
                    break;
                case 3U:
                    stat_regs[seg].stat_d = out[seg] ? *reinterpret_cast<const STATD *>(out[seg]->data())
                                                     : result<STATD>{ unexpected(out[seg].error()) };
                    break;
                case 4U:
                    stat_regs[seg].stat_e = out[seg] ? *reinterpret_cast<const STATE *>(out[seg]->data())
                                                     : result<STATE>{ unexpected(out[seg].error()) };
                    break;
                default:
                    break;
            }
        }
    }
    return stat_regs;
}

} // namespace io::adbms
