#pragma once

#include "util_errorCodes.hpp"
#include "hw_i2cs.hpp"
#include "io_efuses.h"
#include "io_pump.h"

namespace io{
    class pump
    {
    public:
        constexpr explicit pump(
        const Wiper wiper,
        bool invert, 
        io::Efuse *efuse)
        : pot_{&pumps_pot}, wiper_{wiper}, invert_{invert}, efuse_{efuse}{}

        struct PumpConfig{
            const Potentiometer* pot;
            Wiper wiper;
            bool invert;
            io::Efuse* efuse;
        };
        void init(const PumpConfig *pump, size_t count);
        static const std::Expected<PumpConfig, ErrorCode> lookupPump(PumpID id);
        std::Expected<void,ErrorCode> setPercent(PumpID id, uint8_t percent);
        std::Expected<,ErrorCode> getPercent(PumpID id, uint8_t *percent_out);
        std::Expected<void ,ErrorCode> enable(PumpID id, bool enable);
        const bool isEnabled(PumpID id);
        const bool getHealth(PumpID id);

        private:
        const Potentiometer *pot_;
        Wiper wiper_;
        bool invert_;
        io::Efuse* efuse_;
        static std::Expected<,ErrorCode> requirePumpResource(const PumpConfig *cfg, const void *resource, ErrorCode err_code);                
        void registerConfig(PumpConfig *const table[], size_t count);

        static inline uint8_t logicalToHw(const PumpConfig *cfg, uint8_t logical_percent)
        {
            return cfg->invert ? (uint8_t)(100u - logical_percent) : logical_percent;
        }

        static inline uint8_t hwToLogical(const PumpConfig *cfg, uint8_t hw_percent)
        {
            return cfg->invert ? (uint8_t)(100u - hw_percent) : hw_percent;
        }

    }
}// namespace io::pump
static const PumpConfig *pump_table[PUMP_COUNT] = { NULL };
static size_t            pump_count             = 0u;

