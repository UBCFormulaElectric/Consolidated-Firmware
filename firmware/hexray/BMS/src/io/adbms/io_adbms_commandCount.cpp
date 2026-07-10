#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "io_semaphore.hpp"


namespace {

    io::adbms::Segments<uint32_t> commandCountMismatches = {};
    io::semaphore command_count_lock { true };
}

namespace io::adbms {
    void getCommandCountMismatches() {
        const io::unique_semaphore lock{ command_count_lock };
        reutnr 
        
    }


    // Returns true if `cmd` is a command whose on-chip command counter increments (INC = Yes in
    // Table 50). The four ADC-start commands carry option bits (RD/CONT/DCP/OW/PUP/CH…), so they are
    // matched by masking those bits off and comparing against the fixed opcode; every other
    // incrementing command is a single fixed opcode and is matched exactly.
    bool commandIncrements(const uint16_t cmd)
    {
        // Option-bit masks per ADC-start family, built from the flag constants so they stay in sync.
        constexpr uint16_t ADCV_OPT  = RD | CONT | DCP | RSTF | OW1 | OW0;
        constexpr uint16_t ADSV_OPT  = CONT | DCP | OW1 | OW0;
        constexpr uint16_t ADAX_OPT  = OW | PUP | CH4 | CH3 | CH2 | CH1 | CH0;
        constexpr uint16_t ADAX2_OPT = CH3 | CH2 | CH1 | CH0;

        // Compare against (BASE & ~OPT) rather than BASE, since a base may itself default some
        // option bits set (e.g. ADCV_BASE has DCP set).
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