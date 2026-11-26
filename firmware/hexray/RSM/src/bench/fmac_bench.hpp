#pragma once

/**
 * @file fmac_bench.hpp
 * @brief On-target characterization bench for hw::fmac::FmacIir. Only built with -DRSM_FMAC_BENCH=ON.
 *
 * Results are streamed on RTT up-channel 2 and turned into specs by scripts/fmac_bench/fmac_bench.py.
 */

namespace bench::fmac
{
/** Spawn the (low priority) bench task. It runs every test once, then idles. */
void start();
} // namespace bench::fmac
