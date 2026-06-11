#pragma once

// ADBMS daisy-chain data transport: the command / register read / register write primitives. Each
// dispatches over the current reversible-isoSPI drive mode (see io_adbms_spilink) and updates the
// command counter (see io_adbms_cmdcount). All register arrays are in canonical physical segment
// order regardless of which transceiver served the transaction.

#include "io_adbms.hpp"

#include <bitset>

namespace io::adbms
{

/**
 * Sends a command (no data). Issued from whichever end(s) the current mode uses.
 * @return ok if acknowledged, else the SPI error.
 */
[[nodiscard]] result<void> sendCmd(uint16_t cmd);

/**
 * Sends a poll command and returns a packed readiness bitmap. Compare against POLL_STATUS_READY for
 * "all segments ready". In Dual mode both ends are polled and combined.
 */
[[nodiscard]] result<std::bitset<32>> poll(uint16_t cmd);

/**
 * Reads a register group from every segment, in physical order. Each segment's result is the data or
 * a per-segment error (PEC fail / unreachable). Triggers command-counter recovery and break re-probe.
 */
[[nodiscard]] Segments<result<RegBuffer>> readRegGroup(uint16_t cmd);

/**
 * Writes a register group to every segment. regs is in physical order; framed per end internally.
 */
[[nodiscard]] result<void> writeRegGroup(uint16_t cmd, Segments<RegBuffer> &regs);

} // namespace io::adbms
