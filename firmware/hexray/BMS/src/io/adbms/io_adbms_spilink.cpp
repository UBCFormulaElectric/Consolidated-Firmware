#include "io_adbms_spilink.hpp"
#include "io_adbms_protocol.hpp"
#include "hw_spis.hpp"
#include "io_time.hpp"

#include <algorithm>

using namespace std;

namespace io::adbms
{
namespace
{
    // Settle delay after a wake pulse before the chain is ready to read. tWAKE/tREADY are sub-ms;
    // 1 ms gives margin for a full daisy chain.
    constexpr uint32_t WAKE_SETTLE_MS = 1U;

    IsoSpiMode active_mode      = IsoSpiMode::Forward;
    uint8_t    ls_reach         = NUM_SEGMENTS; // physical segments reachable from LS, from seg 0 up
    uint8_t    hs_reach         = NUM_SEGMENTS; // physical segments reachable from HS, from seg N-1 down
    uint32_t   dual_drive_count = 0U;

    // Raw single-side primitives — bypass the per-mode transport dispatcher so probeTopology() can't
    // recurse back into it.
    result<void> rawSend(const Side s, const Cmd &c) { return spiDev(s).transmitDma(c.into_span()); }

    result<void> rawRead(const Side s, const Cmd &c, Segments<RegGroupPayload> &wire)
    {
        return spiDev(s).transmitThenReceiveDma(
            c.into_span(), { reinterpret_cast<uint8_t *>(wire.data()), sizeof(wire) });
    }

    // Sends a wake pulse (CSB toggle + dummy command) on one transceiver and waits for the chain to settle.
    void wakeSide(const Side s)
    {
        (void)rawSend(s, Cmd{ RDCFGA });
        io::time::delay(WAKE_SETTLE_MS);
    }

    // Counts the contiguous PEC-valid run starting at the wire end (index 0) of a freshly-read array.
    uint8_t leadingValid(const Segments<RegGroupPayload> &wire)
    {
        uint8_t n = 0;
        for (size_t i = 0; i < NUM_SEGMENTS; ++i)
        {
            if (!wire[i].checksum())
                break;
            ++n;
        }
        return n;
    }
} // namespace

const hw::spi::device &spiDev(const Side s)
{
    return s == Side::LS ? adbms_spi_ls : adbms_spi_hs;
}

IsoSpiMode activeMode()
{
    return active_mode;
}

Segments<uint8_t> addressCounts()
{
    Segments<uint8_t> counts{};
    if (usesLs(active_mode))
        for (size_t i = 0; i < ls_reach && i < NUM_SEGMENTS; ++i)
            counts[i]++;
    if (usesHs(active_mode))
        for (size_t i = 0; i < hs_reach && i < NUM_SEGMENTS; ++i)
            counts[NUM_SEGMENTS - 1 - i]++;
    return counts;
}

void probeTopology()
{
    const SpiLock lock;
    const Cmd     probe{ RDCFGA }; // a read that doesn't advance the command counter

    uint8_t ls = 0;
    uint8_t hs = 0;

    Segments<RegGroupPayload> wire_ls{};
    wakeSide(Side::LS);
    if (rawRead(Side::LS, probe, wire_ls)) // LS delivers physical order: count valid run from seg 0 up
        ls = leadingValid(wire_ls);

    Segments<RegGroupPayload> wire_hs{};
    wakeSide(Side::HS);
    if (rawRead(Side::HS, probe, wire_hs)) // HS delivers reversed: wire index 0 == physical N-1
        hs = leadingValid(wire_hs);

    ls_reach = ls;
    hs_reach = hs;

    if (ls == NUM_SEGMENTS)
    {
        active_mode = IsoSpiMode::Forward;
    }
    else if (hs == NUM_SEGMENTS)
    {
        active_mode = IsoSpiMode::Reverse;
    }
    else
    {
        if (active_mode != IsoSpiMode::Dual)
            ++dual_drive_count;
        active_mode = IsoSpiMode::Dual; // mid-chain break: drive both ends and merge
    }
}

namespace misc
{
SpiBusReach getSpiBusReach()
{
    return { active_mode, ls_reach, hs_reach, dual_drive_count };
}
} // namespace misc

} // namespace io::adbms
