#include "io_adbms_transport.hpp"
#include "io_adbms_spilink.hpp"
#include "io_adbms_cmdcount.hpp"
#include "io_adbms_protocol.hpp"
#include "util_errorCodes.hpp"

#include <algorithm>
#include <cstring>

using namespace std;

namespace io::adbms
{

result<void> sendCmd(const uint16_t cmd)
{
    const SpiLock lock;
    const Cmd     tx_cmd{ cmd };

    result<void> last_err{};
    bool         ok = true;
    if (usesLs(activeMode()))
        if (const auto s = spiDev(Side::LS).transmitDma(tx_cmd.into_span()); !s)
        {
            ok       = false;
            last_err = s;
        }
    if (usesHs(activeMode()))
        if (const auto s = spiDev(Side::HS).transmitDma(tx_cmd.into_span()); !s)
        {
            ok       = false;
            last_err = s;
        }

    if (ok)
    {
        postTxUpdateCmdCount(cmd);
        return {};
    }
    probeTopology(); // re-evaluate the break for the next transaction
    return last_err;
}

result<bitset<32>> poll(const uint16_t cmd)
{
    const SpiLock lock;
    const Cmd     tx_cmd{ cmd };

    const auto pollSide = [&](const Side s) -> result<uint32_t>
    {
        uint32_t buf;
        static_assert(sizeof(buf) == 4);
        const auto status =
            spiDev(s).transmitThenReceiveDma(tx_cmd.into_span(), { reinterpret_cast<uint8_t *>(&buf), sizeof(buf) });
        return status ? result<uint32_t>{ buf } : unexpected(status.error());
    };

    uint32_t merged;
    if (activeMode() == IsoSpiMode::Dual)
    {
        // Each end reports its reachable devices' real readiness; devices it can't reach return a null
        // response that decodes as ready (logic 1). So the chain is fully ready iff BOTH ends read ready.
        const auto a = pollSide(Side::LS);
        const auto b = pollSide(Side::HS);
        if (!a || !b)
        {
            probeTopology();
            return unexpected((!a ? a : b).error());
        }
        const bool all = (a.value() == POLL_STATUS_READY) && (b.value() == POLL_STATUS_READY);
        // Any busy bit on either side makes the AND differ from POLL_STATUS_READY, so callers keep polling.
        merged = all ? POLL_STATUS_READY : (a.value() & b.value());
    }
    else
    {
        const Side s = usesLs(activeMode()) ? Side::LS : Side::HS;
        const auto r = pollSide(s);
        if (!r)
        {
            probeTopology();
            return unexpected(r.error());
        }
        merged = r.value(); // all-ready mask is order-independent, so no reversal needed
    }

    postTxUpdateCmdCount(cmd);
    return result<std::bitset<32>>{ merged };
}

Segments<result<RegBuffer>> readRegGroup(const uint16_t cmd)
{
    const SpiLock               lock;
    Segments<result<RegBuffer>> regs;
    const Cmd                   tx_cmd{ cmd };

    const auto readSide = [&](const Side s, Segments<RegGroupPayload> &wire)
    {
        return spiDev(s).transmitThenReceiveDma(
            tx_cmd.into_span(), { reinterpret_cast<uint8_t *>(wire.data()), sizeof(wire) });
    };

    if (activeMode() != IsoSpiMode::Dual)
    {
        const Side                s = usesLs(activeMode()) ? Side::LS : Side::HS;
        Segments<RegGroupPayload> rx_buffer{};
        if (const auto comm_status = readSide(s, rx_buffer); !comm_status)
        {
            regs.fill(std::unexpected(comm_status.error()));
            probeTopology();
            return regs;
        }
        readToPhysical(s, rx_buffer); // normalise to physical order so seg 0 is always physical seg 0

        uint8_t valid = 0;
        for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
        {
            if (not rx_buffer[segment].checksum())
            {
                regs[segment] = unexpected(ErrorCode::CHECKSUM_FAIL);
                continue;
            }
            regs[segment] = rx_buffer[segment].getData();
            ++valid;
        }

        // Detect per-segment command counter mismatches and trigger RSTCC recovery. The app layer reads
        // the mismatch bitmap via misc::getCmdCountMismatches() and broadcasts the SegmentCMDCNT message.
        handleCmdCountMismatches(detectCmdCountMismatches(rx_buffer));

        if (valid == 0) // chain unreachable this direction: re-evaluate the break
            probeTopology();
        return regs;
    }

    // Dual: read from both ends and take each physical segment from whichever side's PEC is valid.
    Segments<RegGroupPayload> rx_ls{};
    Segments<RegGroupPayload> rx_hs{};
    const auto                comm_ls = readSide(Side::LS, rx_ls);
    const auto                comm_hs = readSide(Side::HS, rx_hs);
    if (!comm_ls && !comm_hs)
    {
        regs.fill(std::unexpected(comm_ls.error()));
        probeTopology();
        return regs;
    }
    if (comm_ls)
        readToPhysical(Side::LS, rx_ls);
    if (comm_hs)
        readToPhysical(Side::HS, rx_hs);

    const auto     expected = getExpectedCmdCount();
    Segments<bool> mismatches{};
    for (size_t segment = 0U; segment < NUM_SEGMENTS; ++segment)
    {
        const bool ls_ok = comm_ls && rx_ls[segment].checksum();
        const bool hs_ok = comm_hs && rx_hs[segment].checksum();
        if (ls_ok)
        {
            regs[segment]       = rx_ls[segment].getData();
            mismatches[segment] = rx_ls[segment].cmd_count() != expected[segment];
        }
        else if (hs_ok)
        {
            regs[segment]       = rx_hs[segment].getData();
            mismatches[segment] = rx_hs[segment].cmd_count() != expected[segment];
        }
        else
        {
            // Reachable from neither side (e.g. double break / dead device): report per-segment, keep going.
            regs[segment]       = unexpected(ErrorCode::CHECKSUM_FAIL);
            mismatches[segment] = false; // no command-counter opinion for an unreachable segment
        }
    }
    handleCmdCountMismatches(mismatches);
    return regs;
}

result<void> writeRegGroup(const uint16_t cmd, Segments<RegBuffer> &regs)
{
    const SpiLock lock;

    const auto writeSide = [&](const Side s) -> result<void>
    {
        Segments<RegBuffer> wire = regs; // caller passes physical order; frame per side
        writeToWire(s, wire);
        WriteCmd tx_buffer(cmd, wire);
        return spiDev(s).transmitDma(tx_buffer.into_span());
    };

    result<void> last_err{};
    bool         ok = true;
    if (usesLs(activeMode()))
        if (const auto s = writeSide(Side::LS); !s)
        {
            ok       = false;
            last_err = s;
        }
    if (usesHs(activeMode()))
        if (const auto s = writeSide(Side::HS); !s)
        {
            ok       = false;
            last_err = s;
        }

    if (ok)
    {
        postTxUpdateCmdCount(cmd);
        return {};
    }
    probeTopology();
    return last_err;
}

} // namespace io::adbms
