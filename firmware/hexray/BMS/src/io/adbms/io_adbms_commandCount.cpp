#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "io_semaphore.hpp"


namespace {
    using namespace io::adbms;

    uint8_t expected_count = 0U;
    Segments<uint32_t> mismatch_counts{};
    io::semaphore command_count_lock { true };

    uint8_t nextCount(const uint8_t c) { 
        return c >= 63U ? 1U : static_cast<uint8_t>(c + 1U); 
    }

    bool commandIncrements(const uint16_t cmd)
    {
        constexpr uint16_t ADCV_OPT  = RD | CONT | DCP | RSTF | OW1 | OW0;
        constexpr uint16_t ADSV_OPT  = CONT | DCP | OW1 | OW0;
        constexpr uint16_t ADAX_OPT  = OW | PUP | CH4 | CH3 | CH2 | CH1 | CH0;
        constexpr uint16_t ADAX2_OPT = CH3 | CH2 | CH1 | CH0;

        if ((cmd & ~ADCV_OPT) == (ADCV_BASE & ~ADCV_OPT))
            return true;
        if ((cmd & ~ADSV_OPT) == (ADSV_BASE & ~ADSV_OPT))
            return true;
        if ((cmd & ~ADAX_OPT) == (ADAX_BASE & ~ADAX_OPT))
            return true;
        if ((cmd & ~ADAX2_OPT) == (ADAX2_BASE & ~ADAX2_OPT))
            return true;

        switch (cmd)
        {
            // writes
            case WRCFGA:
            case WRCFGB:
            case WRPWMA:
            case WRPWMB:
            // clears
            case CLRCELL:
            case CLRAUX:
            case CLRSPIN:
            case CLRFC:
            case CLRFLAG:
            case CLOVUV:
            // polls
            case PLADC:
            case PLCADC:
            case PLSADC:
            case PLAUX:
            case PLAUX2:
            // discharge mute
            case MUTE:
            case UNMUTE:
            // snapshot
            case SNAP:
            case UNSNAP:
                return true;
            default:
                return false;
        }
    }
}
namespace io::adbms::commandCount {

    result<void> reset() {
        const auto res = sendCmd(RSTCC);
        const io::unique_semaphore lock{ command_count_lock };
        expected_count = 0U;
        return res;
    }

    void increment(const uint16_t cmd) {
        if (!commandIncrements(cmd)) return;
        const io::unique_semaphore lock{ command_count_lock };
        expected_count = nextCount(expected_count);
    }

    bool check(const uint8_t seg, const uint8_t received) {
        const io::unique_semaphore lock{ command_count_lock };
        if (received != expected_count) {
            mismatch_counts[seg]++;
            return false;
        }
        return true;
    }

    Segments<uint32_t> getMismatches() {
        const io::unique_semaphore lock{ command_count_lock };
        return mismatch_counts;
    }
}