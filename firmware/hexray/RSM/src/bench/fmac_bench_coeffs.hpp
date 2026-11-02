#pragma once

/**
 * @file fmac_bench_coeffs.hpp
 * @brief Filter under test for the RSM FMAC bench. Generated, do not edit by hand:
 *   python scripts/fmac_bench/fmac_bench.py design --fs 1000 --fc 50 --order 2
 */

#include <array>
#include <cstdint>

namespace bench::fmac::coeffs
{
inline constexpr const char *DESCRIPTION         = "butterworth order=2 fc=50Hz fs=1000Hz";
inline constexpr uint32_t    SAMPLE_RATE_MILLIHZ = 1000000U; // only used to label the analysis
inline constexpr uint8_t     GAIN_EXPONENT       = 1U;

// FMAC IIR convention: y[n] = 2^R * (sum b_q[k] x[n-k] + sum a_q[j] y[n-j]), so b_q = b / 2^R and a_q = -a[1..] / 2^R.
inline constexpr std::array<int16_t, 3> B_Q15 = { { 329, 658, 329 } };
inline constexpr std::array<int16_t, 2> A_Q15 = { { 25576, -10508 } };

// Unquantized design (a[0] = 1), forwarded to the analysis script as the ideal reference.
inline constexpr std::array<float, 3> B_FLOAT = { { 0.020083366f, 0.0401667319f, 0.020083366f } };
inline constexpr std::array<float, 3> A_FLOAT = { { 1.0f, -1.56101811f, 0.641351521f } };
} // namespace bench::fmac::coeffs
