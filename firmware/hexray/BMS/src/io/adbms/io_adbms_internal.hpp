#pragma once

// Umbrella header for the ADBMS driver internals. The implementation is split by concern:
//   - io_adbms_protocol : on-wire framing, PEC, SPI bus lock, and the command opcode set
//   - io_adbms_spilink  : reversible-isoSPI drive mode, break topology, segment ordering
//   - io_adbms_cmdcount : per-segment command-counter bookkeeping
//   - io_adbms_transport: the command / register read / register write primitives
// Existing includers keep including this header to get the whole internal API.

#include "io_adbms_protocol.hpp"
#include "io_adbms_spilink.hpp"
#include "io_adbms_cmdcount.hpp"
#include "io_adbms_transport.hpp"
