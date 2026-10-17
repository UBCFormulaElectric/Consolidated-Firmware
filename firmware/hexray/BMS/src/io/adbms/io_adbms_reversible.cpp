#include "io_adbms_internal.hpp"
#include "hw_spis.hpp"

//implement reversible isoSPI here

namespace io::adbms::reversible {
    result<void> sendCmd(const uint16_t cmd) {
        return spi::sendCmd(adbms_spi_ls, cmd);
    }
    
    result<void> poll(const uint16_t cmd) {
        const result<uint32_t> rx_res = spi::poll(adbms_spi_ls, cmd);
        if (!rx_res) return std::unexpected(rx_res.error());
        if (*rx_res == POLL_STATUS_READY) return {};
        return std::unexpected(ErrorCode::POLL_INVALID);
    }
    
    result<void> readRegGroup(const uint16_t cmd, Segments<RegGroup> &rx) {
        return spi::readRegGroup(adbms_spi_ls, cmd, rx);
    }

    result<void> writeRegGroup(const uint16_t cmd, const Segments<RegGroup> &tx) {
        return spi::writeRegGroup(adbms_spi_ls, cmd, tx);
    }
}