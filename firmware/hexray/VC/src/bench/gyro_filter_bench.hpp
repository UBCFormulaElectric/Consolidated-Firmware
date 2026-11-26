#pragma once

/**
 * @file gyro_filter_bench.hpp
 * @brief Board-in-the-loop bench for the sensor filter library (app_sensor_filter) on gyro Z. Only built with
 * -DVC_GYRO_FILTER_BENCH=ON.
 *
 * Runs ExponentialFilter, ButterworthFilter and ButterworthBiquadFilter side by side at the torque vectoring rate
 * (50 Hz) on either the live IMU1 gyro Z or a synthetic stimulus picked over CAN (Debug_GyroFilterBenchCmd), and sends
 * the filter input and all three outputs from the same tick in VC_GyroFilterBench. Analyzed by
 * scripts/gyro_filter_bench/gyro_filter_bench.py.
 */

namespace bench::gyro_filter
{
/** Spawn the bench task. */
void start();
} // namespace bench::gyro_filter
