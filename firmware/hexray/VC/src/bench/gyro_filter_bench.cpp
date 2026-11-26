/**
 * @file gyro_filter_bench.cpp
 * @brief Board-in-the-loop bench for the sensor filter library on gyro Z. See gyro_filter_bench.hpp and
 * scripts/gyro_filter_bench/README.md.
 *
 * Every 20 ms: pick the input (live IMU1 gyro Z, or a synthetic stimulus), push it through all three filters, and send
 * the input and the three outputs in one VC_GyroFilterBench frame. Because the input rides in the same frame as the
 * outputs, the analysis never needs to know what the stimulus was, and CAN latency cancels out of every delay.
 *
 * Synthetic stimuli are computed on the board, so they are exact and repeatable, unlike twisting the board by hand.
 * Whenever the command changes, the filters are rebuilt and the stimulus restarts from sample 0.
 */

#include "bench/gyro_filter_bench.hpp"

#include <cmath>
#include <cstdint>
#include <optional>

#include "main.h"
#include "cmsis_os2.h"

#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "app_imu.hpp"
#include "app_sensor_filter.hpp"
#include "io_canTx.hpp"
#include "io_log.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "util_units.hpp"

namespace bench::gyro_filter
{
namespace
{
    using app::can_utils::GyroBenchMode;

    constexpr uint32_t PERIOD_MS      = 20U; // torque vectoring rate
    constexpr float    SAMPLE_RATE_HZ = 1000.0f / static_cast<float>(PERIOD_MS);

    // Cutoff used until CANoe sends one (Debug_GyroBenchCutoff = 0). The library asserts 0 < fc < fs / 2.
    constexpr float DEFAULT_CUTOFF_HZ = 10.0f;
    constexpr float MIN_CUTOFF_HZ     = 0.5f;
    constexpr float MAX_CUTOFF_HZ     = 24.5f;

    // Square wave for STEP: 2 s low, 2 s high, so every rising and falling edge is a full settled step.
    constexpr uint32_t STEP_HALF_PERIOD = 100U;
    // One impulse every 2 s, starting 1 s in. The response is ~0 again well within 2 s for any cutoff >= 0.5 Hz.
    constexpr uint32_t IMPULSE_PERIOD = 100U;
    constexpr uint32_t IMPULSE_PHASE  = 50U;
    // Exponential chirp, repeated: 0.2 Hz to 20 Hz in 40 s.
    constexpr float    CHIRP_F0_HZ    = 0.2f;
    constexpr float    CHIRP_F1_HZ    = 20.0f;
    constexpr uint32_t CHIRP_SAMPLES  = 2000U;
    constexpr uint32_t NOISE_SEED     = 0x2545F491U;
    constexpr uint32_t LOG_EVERY_TICK = 50U; // timing summary once a second

    struct Command
    {
        GyroBenchMode mode;
        float         frequency_hz;
        float         amplitude_dps;
        float         cutoff_hz;

        bool operator==(const Command &) const = default;
    };

    struct Filters
    {
        Filters(const float cutoff_hz, const float initial_value)
          : ema(app::ExponentialFilter::withCutoffFrequency(cutoff_hz, SAMPLE_RATE_HZ, initial_value)),
            butterworth(cutoff_hz, SAMPLE_RATE_HZ, initial_value),
            biquad(cutoff_hz, SAMPLE_RATE_HZ, initial_value)
        {
        }

        app::ExponentialFilter       ema;
        app::ButterworthFilter       butterworth;
        app::ButterworthBiquadFilter biquad;
    };

    /** Per-filter process() time over the last LOG_EVERY_TICK ticks, from the DWT cycle counter. */
    struct Timing
    {
        uint32_t sum = 0;
        uint32_t max = 0;

        void add(const uint32_t cycles)
        {
            sum += cycles;
            max = cycles > max ? cycles : max;
        }
    };

    hw::rtos::StaticTask::StaticTaskStack<1024> stack;

    std::optional<Filters> filters;
    std::optional<Command> active;
    uint32_t               n           = 0;    // samples since the command last changed
    float                  sine_phase  = 0.0f; // cycles, kept in [0, 1)
    uint32_t               noise_state = NOISE_SEED;
    float                  last_live   = 0.0f;
    uint8_t                counter     = 0;
    Timing                 timing[3];

    void enableCycleCounter()
    {
        // SystemView may already be using CYCCNT for timestamps, so enable it but never reset it.
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->LAR = 0xC5ACCE55U;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }

    Command readCommand()
    {
        float cutoff = app::can_rx::Debug_GyroBenchCutoff_get();
        cutoff       = cutoff <= 0.0f ? DEFAULT_CUTOFF_HZ : std::fmin(std::fmax(cutoff, MIN_CUTOFF_HZ), MAX_CUTOFF_HZ);
        return { app::can_rx::Debug_GyroBenchMode_get(), app::can_rx::Debug_GyroBenchFrequency_get(),
                 static_cast<float>(app::can_rx::Debug_GyroBenchAmplitude_get()), cutoff };
    }

    float readLiveGyroZ()
    {
        // TaskImu writes the cached result at a higher priority, so it can't be preempted by us mid-write. Copying it
        // with the scheduler suspended stops TaskImu from preempting us mid-read.
        osKernelLock();
        const auto gyro_z = app::imus::getGyroZ();
        osKernelUnlock();
        // Hold the last good value on a bad read, like the controller would, rather than feeding the filters a 0.
        if (gyro_z.has_value())
            last_live = gyro_z.value();
        return last_live;
    }

    /** Uniform in [-1, 1). xorshift32, so a run is repeatable sample for sample. */
    float noise()
    {
        noise_state ^= noise_state << 13;
        noise_state ^= noise_state >> 17;
        noise_state ^= noise_state << 5;
        return static_cast<float>(static_cast<int32_t>(noise_state)) / 2147483648.0f;
    }

    float stimulus(const Command &cmd)
    {
        const float a = cmd.amplitude_dps;
        switch (cmd.mode)
        {
            case GyroBenchMode::GYRO_BENCH_SINE:
            {
                const float x = a * std::sin(M_2PI_F * sine_phase);
                sine_phase += cmd.frequency_hz / SAMPLE_RATE_HZ;
                sine_phase -= std::floor(sine_phase);
                return x;
            }
            case GyroBenchMode::GYRO_BENCH_CHIRP:
            {
                // f(t) = f0 k^(t/T) with k = f1/f0, so phase(t) = 2 pi f0 T (k^(t/T) - 1) / ln k.
                constexpr float duration_s = static_cast<float>(CHIRP_SAMPLES) / SAMPLE_RATE_HZ;
                const float     ln_k       = std::log(CHIRP_F1_HZ / CHIRP_F0_HZ);
                const float     t          = static_cast<float>(n % CHIRP_SAMPLES) / SAMPLE_RATE_HZ;
                const float     cycles = CHIRP_F0_HZ * duration_s * (std::exp(ln_k * t / duration_s) - 1.0f) / ln_k;
                return a * std::sin(M_2PI_F * (cycles - std::floor(cycles)));
            }
            case GyroBenchMode::GYRO_BENCH_STEP:
                return ((n / STEP_HALF_PERIOD) % 2U) == 1U ? a : 0.0f;
            case GyroBenchMode::GYRO_BENCH_IMPULSE:
                return (n % IMPULSE_PERIOD) == IMPULSE_PHASE ? a : 0.0f;
            case GyroBenchMode::GYRO_BENCH_NOISE:
                return a * noise();
            case GyroBenchMode::GYRO_BENCH_LIVE:
            default:
                return readLiveGyroZ();
        }
    }

    void restart(const Command &cmd)
    {
        active      = cmd;
        n           = 0;
        sine_phase  = 0.0f;
        noise_state = NOISE_SEED;
        // Live: start from the current reading so there is no startup transient. Every synthetic stimulus starts at 0.
        filters.emplace(cmd.cutoff_hz, cmd.mode == GyroBenchMode::GYRO_BENCH_LIVE ? readLiveGyroZ() : 0.0f);
        LOG_INFO(
            "Gyro filter bench: mode %d, %d.%d Hz, %d deg/s, cutoff %d.%d Hz", static_cast<int>(cmd.mode),
            static_cast<int>(cmd.frequency_hz), static_cast<int>(cmd.frequency_hz * 10.0f) % 10,
            static_cast<int>(cmd.amplitude_dps), static_cast<int>(cmd.cutoff_hz),
            static_cast<int>(cmd.cutoff_hz * 10.0f) % 10);
    }

    void logTiming()
    {
        constexpr const char *names[3] = { "ema", "butterworth", "biquad" };
        for (size_t i = 0; i < 3; i++)
        {
            const uint32_t mean = timing[i].sum / LOG_EVERY_TICK;
            LOG_INFO(
                "Gyro filter bench: %s process() mean %u cycles (%u ns), max %u cycles", names[i],
                static_cast<unsigned>(mean),
                static_cast<unsigned>(static_cast<uint64_t>(mean) * 1000000000ULL / SystemCoreClock),
                static_cast<unsigned>(timing[i].max));
            timing[i] = {};
        }
    }

    void tick()
    {
        if (const Command cmd = readCommand(); not active.has_value() || cmd != active.value())
            restart(cmd);

        const float x = stimulus(active.value());

        uint32_t    t0  = DWT->CYCCNT;
        const float ema = filters->ema.process(x);
        uint32_t    t1  = DWT->CYCCNT;
        timing[0].add(t1 - t0);
        t0                      = DWT->CYCCNT;
        const float butterworth = filters->butterworth.process(x);
        t1                      = DWT->CYCCNT;
        timing[1].add(t1 - t0);
        t0                 = DWT->CYCCNT;
        const float biquad = filters->biquad.process(x);
        t1                 = DWT->CYCCNT;
        timing[2].add(t1 - t0);

        app::can_tx::VC_GyroBenchCounter_set(counter++);
        app::can_tx::VC_GyroBenchMode_set(active->mode);
        app::can_tx::VC_GyroBenchCutoff_set(active->cutoff_hz);
        app::can_tx::VC_GyroZIn_set(x);
        app::can_tx::VC_GyroZEma_set(ema);
        app::can_tx::VC_GyroZButterworth_set(butterworth);
        app::can_tx::VC_GyroZBiquad_set(biquad);
        io::can_tx::VC_GyroFilterBench_sendAperiodic();

        n++;
        if (n % LOG_EVERY_TICK == 0U)
            logTiming();
    }

    [[noreturn]] void run(void *)
    {
        enableCycleCounter();
        LOG_INFO("Gyro filter bench: sending VC_GyroFilterBench every %u ms", static_cast<unsigned>(PERIOD_MS));

        uint32_t start_ticks = osKernelGetTickCount();
        for (;;)
        {
            tick();
            start_ticks += PERIOD_MS;
            osDelayUntil(start_ticks);
        }
    }

    // Below TaskImu (osPriorityHigh), see readLiveGyroZ().
    hw::rtos::StaticTask task(osPriorityAboveNormal, "TaskGyroFilterBench", run, stack);
} // namespace

void start()
{
    task.start();
}
} // namespace bench::gyro_filter
