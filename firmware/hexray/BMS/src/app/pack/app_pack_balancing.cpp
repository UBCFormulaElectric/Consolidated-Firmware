#include <cstdint>

#include "app_pack.hpp"
#include "app_timer.hpp"

namespace {
    inline constexpr uint32_t BALANCE_MS = 5000;
    inline constexpr uint32_t SETTLE_MS = 5000;
    inline constexpr float DISCHARGE_THRESHOLD_V = 10e-3f;

    inline constexpr float MIN_BALANCE_V = 2.5f;
    inline constexpr uint8_t DISCHARGE_DUTY = 0x0F;
    
    enum class State : uint8_t
    {
        DISABLED,
        SETTLING,
        BALANCING,
    };

    State state = State::DISABLED;
    app::Timer settle_timer{ SETTLE_MS };
    app::Timer balance_timer{ BALANCE_MS };

    void chooseTargets(const app::pack::VoltStats &volt_stats) {
        Request request{};
        request.kind = RequestKind::SET_BALANCING;

        const auto target = volt_stats.min;

        for (uint8_t seg = 0U; seg < NUM_SEGMENTS; ++seg) {
            if (!volt_stats.valid[seg].all()) continue;

            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; ++cell)
            {
                const float volts = volt_stats.voltages[seg][cell];

                if (seg == target.segment && cell == target.index) continue;

                if (volts <= MIN_BALANCE_V) continue;

                if (volts - target.value < DISCHARGE_THRESHOLD_V) continue;

                request.discharge_enabled[seg][cell] = true;
                request.duty[seg][cell]              = DISCHARGE_DUTY;
            }

        }
        requests::post(request);
    }
}

namespace app::pack::balancing {
    void init() {
        state = State::DISABLED;
        settle_timer.stop();
        balance_timer.stop();
    }

    void disable() {
        state = State::DISABLED;
        Request request{};
        request.kind = RequestKind::STOP_BALANCING;
        requests::post(request);
    }

    void tick (const VoltStats &volts){
        switch (state)
        {
            case State::DISABLED:
                settle_timer.restart();
                state = State::SETTLING;
                break;

            case State::SETTLING:
                if (settle_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED) {
                    chooseTargets(volts);
                    balance_timer.restart();
                    state = State::BALANCING;
                }
                break;
            case State::BALANCING:
                if (balance_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED) {
                    Request request{};
                    request.kind = RequestKind::STOP_BALANCING;
                    requests::post(request);

                    settle_timer.restart();
                    state = State::SETTLING;
                }
                break;
            default:
                break;
        }
    }
}