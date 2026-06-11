#pragma once

// Reversible isoSPI "link" layer: owns which transceiver(s) drive the daisy chain and the break
// topology. The chain has an isoSPI transceiver (LTC6820) on each end — low-side (LS, bottom) and
// high-side (HS, top). A break at an end is survived by driving from the other end; a mid-chain
// break is survived by driving from both ends and merging per physical segment. This module decides
// the drive mode and maps between wire order and a fixed physical segment order.

#include "io_adbms.hpp"

#include "hw_spi.hpp"

#include <algorithm>

namespace io::adbms
{

// Which end of the chain a transaction is driven from.
enum class Side : uint8_t
{
    LS, // low-side (bottom) transceiver
    HS, // high-side (top) transceiver
};

// The SPI device (chip-select) for one end of the chain.
[[nodiscard]] const hw::spi::device &spiDev(Side s);

// Current drive mode (Forward / Reverse / Dual).
[[nodiscard]] IsoSpiMode activeMode();

// Whether the given mode drives from the LS / HS end. Dual drives both.
[[nodiscard]] inline bool usesLs(const IsoSpiMode m) { return m == IsoSpiMode::Forward || m == IsoSpiMode::Dual; }
[[nodiscard]] inline bool usesHs(const IsoSpiMode m) { return m == IsoSpiMode::Reverse || m == IsoSpiMode::Dual; }

// Segment-order normalisation. The LS end delivers/expects segments in physical order (segment 0 ==
// closest to LS); the HS end delivers/expects them reversed. These map a per-side array to/from the
// canonical physical order so the rest of the firmware always sees a fixed physical segment index.
template <typename T> void readToPhysical(const Side s, Segments<T> &a)
{
    if (s == Side::HS)
        std::ranges::reverse(a);
}
template <typename T> void writeToWire(const Side s, Segments<T> &a)
{
    if (s == Side::LS)
        std::ranges::reverse(a);
}

// How many times each physical segment's command counter advances for one tx in the current mode:
// a segment addressed from both ends (Dual overlap) advances twice, single-side once, unreachable 0.
[[nodiscard]] Segments<uint8_t> addressCounts();

// Probes both transceivers to locate a break and select the drive mode. Issues a wake + RDCFGA from
// each end and counts the contiguous PEC-valid run from that end. Safe at init, on failure, or
// periodically; uses raw single-side reads so it never recurses through the transport layer.
void probeTopology();

} // namespace io::adbms
