#pragma once

// Command-counter bookkeeping. Every ADBMS6830 device keeps a per-device command counter that
// advances on each counter-incrementing command. The host mirrors an expected count per physical
// segment; a mismatch on read indicates dropped/duplicated commands and triggers an RSTCC resync.
// Under dual-drive a segment addressed from both ends advances twice, which is why the expected
// count is tracked per segment (see io_adbms_spilink::addressCounts).

#include "io_adbms.hpp"
#include "io_adbms_protocol.hpp" // RegGroupPayload

namespace io::adbms
{

// Snapshot of the per-segment expected command counter.
[[nodiscard]] Segments<uint8_t> getExpectedCmdCount();

// Advance the expected counter after a successful tx, by the number of ends that addressed each
// segment this transaction. RSTCC resets all expected counts to 0.
void postTxUpdateCmdCount(uint16_t cmd);

// Compare a physical-ordered read against the expected counts. rx must already be in physical order.
[[nodiscard]] Segments<bool> detectCmdCountMismatches(const Segments<RegGroupPayload> &rxPhysical);

// Record mismatches for reporting and, if any occurred, trigger RSTCC recovery.
void handleCmdCountMismatches(const Segments<bool> &mismatches);

} // namespace io::adbms
