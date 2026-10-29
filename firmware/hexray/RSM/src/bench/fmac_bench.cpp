/**
 * @file fmac_bench.cpp
 * @brief On-target characterization bench for hw::fmac::FmacIir (see scripts/fmac_bench/README.md).
 *
 * Every test is run twice:
 *   - "drv": through hw::fmac::FmacIir exactly as application code uses it (float in, float out).
 *   - "raw": a minimal register-level reference (WDATA -> wait !YEMPTY -> RDATA) with the same buffer layout but a
 *            zeroed filter history, so driver behaviour can be separated from what the FMAC hardware itself does.
 *
 * Inputs are generated on-chip, every call is timed with the DWT cycle counter, and all results are stored before
 * anything is printed so the RTT traffic can't disturb the measurement. Everything is printed as integers
 * (Q1.15 counts, CPU cycles, float bit patterns) to keep the log exact.
 */
#include "fmac_bench.hpp"
#include "fmac_bench_coeffs.hpp"

#include "hw_fmac.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_utils.hpp"
#include "io_log.hpp"
#include "main.h"
#include "SEGGER_RTT.h"
#include "util_utils.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <optional>
#include <span>

namespace bench::fmac
{
namespace
{
    constexpr unsigned RTT_CHANNEL    = 2; // 0 = logs, 1 = SystemView
    constexpr uint32_t BOOT_SETTLE_MS = 2000;
    constexpr int32_t  NO_OUTPUT      = INT32_MIN;

    constexpr size_t  IMPULSE_LEN = 512;
    constexpr size_t  STEP_LEN    = 512;
    constexpr size_t  NOISE_LEN   = 4096;
    constexpr size_t  MAX_LEN     = NOISE_LEN;
    constexpr int16_t IMPULSE_AMP = 29491; // 0.9 FS
    constexpr int16_t STEP_AMP    = 16384; // 0.5 FS
    constexpr int16_t STEP_FS_AMP = 32440; // 0.99 FS, so any overshoot has to clip
    constexpr size_t  MAX_PRE     = 8;

    // y[n] = 2^1 * (0.5 * x[n]) = x[n]: makes sample alignment and the float <-> Q1.15 conversion directly visible.
    constexpr std::array<int16_t, 2> PASS_B    = { { 16384, 0 } };
    constexpr std::array<int16_t, 1> PASS_A    = { { 0 } };
    constexpr uint8_t                PASS_R    = 1;
    constexpr std::array<float, 16> PASS_INPUT = { { 0.0f, 0.5f, 0.0f, 0.0f, -0.5f, 0.25f, 0.999969f, 1.0f, -1.0f, 1.5f,
                                                     -1.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } };

    struct Filter
    {
        std::span<const int16_t> b;
        std::span<const int16_t> a;
        uint8_t                  r;
    };
    constexpr Filter PASSTHROUGH{ PASS_B, PASS_A, PASS_R };
    constexpr Filter FILTER_UNDER_TEST{ coeffs::B_Q15, coeffs::A_Q15, coeffs::GAIN_EXPONENT };

    struct Run
    {
        size_t                       produced   = 0;  // samples with a recorded result
        int32_t                      err        = -1; // ErrorCode of the first failure, -1 if none
        uint32_t                     sr         = 0;  // FMAC_SR after the last sample
        uint32_t                     initCycles = 0;
        size_t                       numPre     = 0; // outputs the FMAC produced before the first input
        std::array<int16_t, MAX_PRE> pre{};
    };

    FMAC_HandleTypeDef                hfmac_bench;
    std::array<char, 8192>            rtt_buffer;
    std::array<float, MAX_LEN>        inputs;
    std::array<int32_t, MAX_LEN>      outputs;
    std::array<uint32_t, MAX_LEN>     cycles;
    constexpr std::array<int16_t, 64> ZEROS{};

    // ---------------------------------------------------------------------------------------------------------------
    // RTT output
    // ---------------------------------------------------------------------------------------------------------------

    /** One output line, written with a single SEGGER_RTT_Write once the host has drained enough space. */
    class Line
    {
      public:
        Line &s(const char *str)
        {
            while (*str != '\0' && len_ < buf_.size() - 1)
                buf_[len_++] = *str++;
            return *this;
        }
        Line &i(int32_t v)
        {
            if (v == NO_OUTPUT)
                return s("-");
            char  digits[12];
            char *p   = std::end(digits);
            auto  mag = v < 0 ? -static_cast<uint32_t>(v) : static_cast<uint32_t>(v);
            *--p      = '\0';
            do
            {
                *--p = static_cast<char>('0' + mag % 10U);
                mag /= 10U;
            } while (mag != 0U);
            if (v < 0)
                *--p = '-';
            return s(p);
        }
        Line &hex(uint32_t v)
        {
            char digits[9];
            for (int n = 7; n >= 0; --n, v >>= 4)
                digits[n] = "0123456789abcdef"[v & 0xFU];
            digits[8] = '\0';
            return s(digits);
        }
        Line &kv(const char *key, int32_t v) { return s(" ").s(key).s("=").i(v); }
        void  send()
        {
            buf_[len_++] = '\n';
            // NO_BLOCK_SKIP drops whole writes when the buffer is full, so wait for room here instead.
            // (BLOCK_IF_FIFO_FULL would spin inside the RTT lock with interrupts masked.)
            while (SEGGER_RTT_GetAvailWriteSpace(RTT_CHANNEL) < len_)
                osDelay(1);
            SEGGER_RTT_Write(RTT_CHANNEL, buf_.data(), len_);
        }

      private:
        std::array<char, 192> buf_{};
        size_t                len_ = 0;
    };

    // ---------------------------------------------------------------------------------------------------------------
    // Helpers
    // ---------------------------------------------------------------------------------------------------------------

    void enableCycleCounter()
    {
        DCB->DEMCR |= DCB_DEMCR_TRCENA_Msk;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }

    inline uint32_t now()
    {
        return DWT->CYCCNT;
    }

    int32_t toCounts(const float x)
    {
        return static_cast<int32_t>(lrintf(x * 32768.0f));
    }

    int16_t toQ15Saturated(const float x)
    {
        return static_cast<int16_t>(std::clamp<int32_t>(toCounts(x), INT16_MIN, INT16_MAX));
    }

    int32_t errorToInt(const ErrorCode e)
    {
        return static_cast<int32_t>(e);
    }

    /** Hard-reset the peripheral so every test starts from the same state. */
    void resetFmac()
    {
        __HAL_RCC_FMAC_CLK_ENABLE();
        __HAL_RCC_FMAC_FORCE_RESET();
        __HAL_RCC_FMAC_RELEASE_RESET();
        hfmac_bench          = {};
        hfmac_bench.Instance = FMAC;
        (void)HAL_FMAC_Init(&hfmac_bench);
    }

    // ---------------------------------------------------------------------------------------------------------------
    // Register-level reference
    // ---------------------------------------------------------------------------------------------------------------

    /** Same buffer layout and filter config as hw::fmac::FmacIir, but with explicit history preload. */
    class RawFmac
    {
      public:
        result<void>
            init(const Filter &f, std::span<const int16_t> x1Preload, std::span<const int16_t> yPreload, Run &run)
        {
            resetFmac();
            const auto     p      = static_cast<uint8_t>(f.b.size());
            const auto     q      = static_cast<uint8_t>(f.a.size());
            const uint16_t x1size = p + 1;
            const uint16_t x2size = p + q;
            const uint16_t ysize  = q + 1;

            FMAC_FilterConfigTypeDef cfg = {
                .InputBaseAddress  = 0,
                .InputBufferSize   = static_cast<uint8_t>(x1size),
                .InputThreshold    = FMAC_THRESHOLD_1,
                .CoeffBaseAddress  = static_cast<uint8_t>(x1size),
                .CoeffBufferSize   = static_cast<uint8_t>(x2size),
                .OutputBaseAddress = static_cast<uint8_t>(x1size + x2size),
                .OutputBufferSize  = static_cast<uint8_t>(ysize),
                .OutputThreshold   = FMAC_THRESHOLD_1,
                .pCoeffA           = const_cast<int16_t *>(f.a.data()),
                .CoeffASize        = q,
                .pCoeffB           = const_cast<int16_t *>(f.b.data()),
                .CoeffBSize        = p,
                .InputAccess       = FMAC_BUFFER_ACCESS_POLLING,
                .OutputAccess      = FMAC_BUFFER_ACCESS_POLLING,
                .Clip              = FMAC_CLIP_ENABLED,
                .Filter            = FMAC_FUNC_IIR_DIRECT_FORM_1,
                .P                 = p,
                .Q                 = q,
                .R                 = f.r,
            };
            RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_FMAC_FilterConfig(&hfmac_bench, &cfg)));
            if (!x1Preload.empty() || !yPreload.empty())
            {
                RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_FMAC_FilterPreload(
                    &hfmac_bench, x1Preload.empty() ? nullptr : const_cast<int16_t *>(x1Preload.data()),
                    static_cast<uint8_t>(x1Preload.size()),
                    yPreload.empty() ? nullptr : const_cast<int16_t *>(yPreload.data()),
                    static_cast<uint8_t>(yPreload.size()))));
            }
            RETURN_IF_ERR(hw::utils::convertHalStatus(HAL_FMAC_FilterStart(&hfmac_bench, nullptr, nullptr)));

            // Give the FMAC time to compute anything it can from the preloaded history alone, then drain it.
            const uint32_t t0 = now();
            while (now() - t0 < 2000U)
            {
            }
            while ((FMAC->SR & FMAC_SR_YEMPTY) == 0U)
            {
                const auto y = static_cast<int16_t>(FMAC->RDATA);
                if (run.numPre < run.pre.size())
                    run.pre[run.numPre] = y;
                run.numPre++;
            }
            return {};
        }

        static std::optional<int16_t> step(const int16_t x)
        {
            FMAC->WDATA = static_cast<uint16_t>(x);
            for (uint32_t guard = 0; guard < 20000U; ++guard)
            {
                if ((FMAC->SR & FMAC_SR_YEMPTY) == 0U)
                    return static_cast<int16_t>(FMAC->RDATA);
            }
            return std::nullopt;
        }

        static void stop() { (void)HAL_FMAC_FilterStop(&hfmac_bench); }
    };

    // ---------------------------------------------------------------------------------------------------------------
    // Runners. Results land in outputs[] / cycles[]; nothing is printed while the filter runs.
    // ---------------------------------------------------------------------------------------------------------------

    Run runDriver(const Filter &f, std::span<const float> in)
    {
        Run run;
        resetFmac();
        hw::fmac::FmacIir iir(hfmac_bench);

        const uint32_t t0      = now();
        const auto     initRes = iir.init(f.b, f.a, f.r);
        run.initCycles         = now() - t0;
        if (!initRes)
        {
            run.err = errorToInt(initRes.error());
            return run;
        }

        for (size_t n = 0; n < in.size(); ++n)
        {
            const uint32_t t = now();
            const auto     y = iir.process(in[n]);
            cycles[n]        = now() - t;
            if (!y)
            {
                run.err = errorToInt(y.error());
                break;
            }
            outputs[n]   = toCounts(y.value());
            run.produced = n + 1;
        }
        run.sr = FMAC->SR;
        (void)iir.stop();
        return run;
    }

    Run runRaw(
        const Filter            &f,
        std::span<const int16_t> x1Preload,
        std::span<const int16_t> yPreload,
        std::span<const float>   in)
    {
        Run     run;
        RawFmac fmac;
        if (const auto res = fmac.init(f, x1Preload, yPreload, run); !res)
        {
            run.err = errorToInt(res.error());
            return run;
        }

        for (size_t n = 0; n < in.size(); ++n)
        {
            // Time the same float -> Q1.15 -> float round trip the driver does, so the numbers are comparable.
            const uint32_t t  = now();
            const auto     y  = RawFmac::step(toQ15Saturated(in[n]));
            const float    yf = y ? static_cast<float>(*y) / 32768.0f : 0.0f;
            cycles[n]         = now() - t;
            outputs[n]        = y ? toCounts(yf) : NO_OUTPUT;
        }
        run.produced = in.size();
        run.sr       = FMAC->SR;
        RawFmac::stop();
        return run;
    }

    /** Zeroed history for the filter's own shape: P-1 input samples so the first write produces y[0]. */
    Run runRawZeroState(const Filter &f, std::span<const float> in)
    {
        return runRaw(f, std::span(ZEROS).first(f.b.size() - 1), std::span(ZEROS).first(f.a.size()), in);
    }

    void emit(const char *path, const char *test, std::span<const float> in, const Run &run)
    {
        Line()
            .s("begin ")
            .s(path)
            .s(" ")
            .s(test)
            .kv("n", static_cast<int32_t>(run.produced))
            .kv("len", static_cast<int32_t>(in.size()))
            .kv("err", run.err)
            .s(" sr=0x")
            .hex(run.sr)
            .kv("init_cyc", static_cast<int32_t>(run.initCycles))
            .kv("pre", static_cast<int32_t>(run.numPre))
            .send();
        if (run.numPre > 0)
        {
            Line line;
            line.s("pre");
            for (size_t k = 0; k < std::min(run.numPre, run.pre.size()); ++k)
                line.s(" ").i(run.pre[k]);
            line.send();
        }
        for (size_t n = 0; n < run.produced; ++n)
            Line().i(toCounts(in[n])).s(" ").i(outputs[n]).s(" ").i(static_cast<int32_t>(cycles[n])).send();
        Line().s("end ").s(path).s(" ").s(test).send();
    }

    void emitHeader()
    {
        Line().s("#FMAC_BENCH v1").send();
        Line()
            .s("meta")
            .kv("sysclk_hz", static_cast<int32_t>(SystemCoreClock))
            .kv("fs_millihz", static_cast<int32_t>(coeffs::SAMPLE_RATE_MILLIHZ))
            .kv("R", coeffs::GAIN_EXPONENT)
            .kv("P", static_cast<int32_t>(coeffs::B_Q15.size()))
            .kv("Q", static_cast<int32_t>(coeffs::A_Q15.size()))
            .send();
        Line().s("desc ").s(coeffs::DESCRIPTION).send();

        Line b, a, bf, af;
        b.s("coef_b_q15");
        for (const auto v : coeffs::B_Q15)
            b.s(" ").i(v);
        a.s("coef_a_q15");
        for (const auto v : coeffs::A_Q15)
            a.s(" ").i(v);
        bf.s("coef_b_f32");
        for (const auto v : coeffs::B_FLOAT)
            bf.s(" ").hex(std::bit_cast<uint32_t>(v));
        af.s("coef_a_f32");
        for (const auto v : coeffs::A_FLOAT)
            af.s(" ").hex(std::bit_cast<uint32_t>(v));
        b.send();
        a.send();
        bf.send();
        af.send();
    }

    /** Run one stimulus through both paths. */
    void runTest(const char *test, const Filter &f, std::span<const float> in)
    {
        emit("drv", test, in, runDriver(f, in));
        emit("raw", test, in, runRawZeroState(f, in));
    }

    std::span<const float> fill(const size_t len, float (*gen)(size_t n))
    {
        for (size_t n = 0; n < len; ++n)
            inputs[n] = gen(n);
        return std::span(inputs).first(len);
    }

    uint32_t xorshift_state;
    float    noiseSample(size_t)
    {
        xorshift_state ^= xorshift_state << 13;
        xorshift_state ^= xorshift_state >> 17;
        xorshift_state ^= xorshift_state << 5;
        // Uniform in [-0.5, 0.5) FS, exactly representable in Q1.15.
        return static_cast<float>(static_cast<int16_t>(xorshift_state >> 16) / 2) / 32768.0f;
    }

    void runAll()
    {
        emitHeader();

        const auto pass = std::span(PASS_INPUT);
        runTest("passthru", PASSTHROUGH, pass);

        // How the FMAC aligns outputs for different amounts of preloaded history (P = 2 for the passthrough).
        emit("raw", "align_none", pass, runRaw(PASSTHROUGH, {}, {}, pass));
        emit(
            "raw", "align_x1_p-1", pass,
            runRaw(PASSTHROUGH, std::span(ZEROS).first(1), std::span(ZEROS).first(1), pass));
        emit(
            "raw", "align_x1_p", pass, runRaw(PASSTHROUGH, std::span(ZEROS).first(2), std::span(ZEROS).first(1), pass));
        // What hw::fmac::FmacIir::init() preloads (B into X1, A into Y), read back without the driver's poll path.
        emit("raw", "align_coef", pass, runRaw(PASSTHROUGH, PASS_B, PASS_A, pass));

        runTest(
            "impulse", FILTER_UNDER_TEST,
            fill(IMPULSE_LEN, [](size_t n) { return n == 0 ? IMPULSE_AMP / 32768.0f : 0.0f; }));
        runTest("step", FILTER_UNDER_TEST, fill(STEP_LEN, [](size_t) { return STEP_AMP / 32768.0f; }));
        runTest("step_fs", FILTER_UNDER_TEST, fill(STEP_LEN, [](size_t) { return STEP_FS_AMP / 32768.0f; }));
        xorshift_state = 0x12345678U;
        runTest("noise", FILTER_UNDER_TEST, fill(NOISE_LEN, noiseSample));

        Line().s("#FMAC_BENCH done").send();
    }

    [[noreturn]] void benchTask(void *)
    {
        SEGGER_RTT_ConfigUpBuffer(
            RTT_CHANNEL, "FMAC", rtt_buffer.data(), rtt_buffer.size(), SEGGER_RTT_MODE_NO_BLOCK_SKIP);
        enableCycleCounter();
        osDelay(BOOT_SETTLE_MS);

        LOG_INFO("FMAC bench: streaming results on RTT channel %d", RTT_CHANNEL);
        runAll();
        LOG_INFO("FMAC bench: done, reset the board to run again");

        forever
        {
            osDelay(1000);
        }
    }

    hw::rtos::StaticTask::StaticTaskStack<1024> bench_stack;
    hw::rtos::StaticTask                        bench_task(osPriorityLow, "TaskFmacBench", benchTask, bench_stack);
} // namespace

void start()
{
    bench_task.start();
}
} // namespace bench::fmac
