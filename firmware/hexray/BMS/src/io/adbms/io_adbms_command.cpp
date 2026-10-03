#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "app_pack.hpp"
#include "hw_spis.hpp"

namespace {

    // Start
    constexpr uint16_t ADCV_BASE = 0x0270U;
    constexpr uint16_t ADSV_BASE = 0x0168U;
    constexpr uint16_t ADAX_BASE = 0x0410U;

    // ADCV and ADSV
    constexpr uint16_t RD   = 1U << 8; // redundant C + S ADC
    constexpr uint16_t CONT = 1U << 7; // continuous mode
    constexpr uint16_t OW1  = 1U << 1; // open wire odd channels
    constexpr uint16_t OW0  = 1U << 0; // open wire even channels

    // Poll
    constexpr uint16_t PLSADC = 0x071DU;
    constexpr uint16_t PLAUX  = 0x071EU;

    // Snapshot
    constexpr uint16_t SNAP   = 0x002DU;
    constexpr uint16_t UNSNAP = 0x002FU;

    // Discharge
    constexpr uint16_t MUTE   = 0x0028U;
    constexpr uint16_t UNMUTE = 0x0029U;

}

namespace io::adbms::write {
    result<void> configReg(const Segments<SegmentConfig> &config) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }

    result<void> pwmReg(const Segments<PWMConfig> &pwm_config) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }
}

namespace io::adbms::read {
    Segments<result<SegmentConfig>> configReg() {
        Segments<result<SegmentConfig>> out;
        out.fill(std::unexpected(ErrorCode::UNIMPLEMENTED));
        return out;
    }

    Segments<result<PWMConfig>> pwmReg() {
        Segments<result<PWMConfig>> out;
        out.fill(std::unexpected(ErrorCode::UNIMPLEMENTED));
        return out;
    }

    result<void> Cadc(const bool redundant, app::pack::VoltStats &stats) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }

    result<void> Sadc(const OpenWireParity parity, app::pack::OwcStats &stats) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }

    result<void> Xadc(const ThermistorMux mux, app::pack::TempStats &stats) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }

    result<void> flags(app::pack::ADBMS6830Diag &diag) {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }
}

namespace io::adbms::command {
    result<void> startCadc(const bool redundant) {
        return spi::sendCmd(adbms_spi_ls, ADCV_BASE | CONT | (redundant ? RD : 0U));
    }

    result<void> startSadc(const bool continuous, const OpenWireParity parity) {
        const uint16_t ow = parity == OpenWireParity::EVEN ? OW0 : parity == OpenWireParity::ODD ? OW1 : 0U;
        return spi::sendCmd(adbms_spi_ls, ADSV_BASE | (continuous ? CONT : 0U) | ow);
    }

    result<void> startAuxadc() {
        return spi::sendCmd(adbms_spi_ls, ADAX_BASE);
    }

    result<void> pollSadc() {
        return spi::poll(adbms_spi_ls, NUM_SEGMENTS, PLSADC);
    }

    result<void> pollXadc() {
        return spi::poll(adbms_spi_ls, NUM_SEGMENTS, PLAUX);
    }

    result<void> snap() {
        return spi::sendCmd(adbms_spi_ls, SNAP);
    }

    result<void> unsnap() {
        return spi::sendCmd(adbms_spi_ls, UNSNAP);
    }

    result<void> startBalance() {
        return spi::sendCmd(adbms_spi_ls, UNMUTE);
    }

    result<void> stopBalance() {
        return spi::sendCmd(adbms_spi_ls, MUTE);
    }

    result<void> clearFlags() {
        return std::unexpected(ErrorCode::UNIMPLEMENTED);
    }
}
