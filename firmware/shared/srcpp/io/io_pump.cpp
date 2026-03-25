#include "io_pump.hpp"

namespace::io{
    // void init(const PumpConfig *pump, size_t count)
    // {    }

    // static const std::Expected<PumpConfig, ErrorCode> lookupPump(PumpID id)
    // {
    //     if (id >= pump_count || pump_table[id] == NULL)
    //     {
    //         assert(false);
    //         return NULL;
    //     }
    //     return pump_table[id];
    // }

    std::Expected<void,ErrorCode> setPercent(uint8_t percent)
    {
        auto status = requirePumpResource(cfg, cfg != NULL ? cfg->pot : NULL, ErrorCode::Error);
        if (status)
        {
            return status;
        }

        const uint8_t hw_percent = logicalToHw(cfg, percent);
        return io_potentiometer_writePercentage(cfg->pot, cfg->wiper, hw_percent);
    }


    std::Expected<void ,ErrorCode> getPercent(PumpID id, uint8_t *percent_out)
    {
        if (percent_out == NULL)
        {
            return EXIT_CODE_INVALID_ARGS;
        }

        const PumpConfig *cfg    = lookupPump(id);
        const auto          status = requirePumpResource(cfg, cfg != NULL ? cfg->pot : NULL, EXIT_CODE_INVALID_ARGS);
        if (status != EXIT_CODE_OK)
        {
            return status;
        }

        uint8_t hw_percent = 0u;
        status             = io_potentiometer_readPercentage(cfg->pot, cfg->wiper, &hw_percent);
        if (status != EXIT_CODE_OK)
        {
            return status;
        }

        *percent_out = hwToLogical(cfg, hw_percent);
        return EXIT_CODE_OK;
    }

            std::Expected<void ,ErrorCode> enable(PumpID id, bool enable)
    {
        const PumpConfig *cfg    = lookupPump(id);
        const auto          status = requirePumpResource(cfg, cfg != NULL ? cfg->efuse : NULL, EXIT_CODE_UNIMPLEMENTED);
        if (status != EXIT_CODE_OK)
        {
            return status;
        }

        io_efuse_setChannel(cfg->efuse, enable);
        return EXIT_CODE_OK;
    }

            bool isEnabled(PumpID id)
    {
        if (enabled_out == NULL)
        {
            return EXIT_CODE_INVALID_ARGS;
        }

        const PumpConfig *cfg    = lookupPump(id);
        auto    const      status = requirePumpResource(cfg, cfg != NULL ? cfg->efuse : NULL, EXIT_CODE_UNIMPLEMENTED);
        if (status != EXIT_CODE_OK)
        {
            return status;
        }

        *enabled_out = io_efuse_isChannelEnabled(cfg->efuse);
        return EXIT_CODE_OK;
    }

    const bool getHealth(PumpID id)
    {
        if (pgood_out == NULL)
        {
            return EXIT_CODE_INVALID_ARGS;
        }

        const PumpConfig *cfg    = lookupPump(id);
        auto const          status = requirePumpResource(cfg, cfg != NULL ? cfg->efuse : NULL, EXIT_CODE_UNIMPLEMENTED);
        if (status != EXIT_CODE_OK)
        {
            return status;
        }

        *pgood_out = io_efuse_pgood(cfg->efuse);
        return EXIT_CODE_OK;
    }

    static std::Expected<void ,ErrorCode> requirePumpResource(const void *resource, ErrorCode err_code)
    {
        if (cfg == NULL || resource == NULL)
        {
            return err_code;
        }
        return;
    }
            void registerConfig(size_t count)
    {
        assert(table != NULL || count == 0u);
        assert(count <= PUMP_COUNT);

        for (size_t i = 0u; i < count; ++i)
        {
            pump_table[i] = table[i];
        }
        for (size_t i = count; i < PUMP_COUNT; ++i)
        {
            pump_table[i] = NULL;
        }

        pump_count = count;
            }        
} // namespace io
