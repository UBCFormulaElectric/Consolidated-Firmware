#include <algorithm>
#include <cmath>
#include <limits>

#include "app_canRx.hpp"
#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "io_semaphore.hpp"

namespace {
    const io::semaphore request_lock{ true };
    app::pack::Request shared_request{};
    app::pack::PackChannel<app::pack::VoltStats>::Subscription voltage_sub{ "balancing_volts" };

    constexpr float DISCHARGE_THRESHOLD_V = 10e-3f;
    constexpr uint8_t MAX_DUTY = 0x0F;

    io::adbms::Cells<uint8_t> determineBalance(const io::adbms::Cells<float> &voltages, const app::pack::CellFlags &valid) {
        io::adbms::Cells<uint8_t> duty{};

        const auto commanded_duty = static_cast<uint8_t>(std::lround(app::can_rx::Debug_CellBalancing_DutyCycle_get() / 100.0f * MAX_DUTY));

        if (commanded_duty == 0)
            return duty;

        float leader = std::numeric_limits<float>::max();
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++) {
                if (valid[seg][cell])
                    leader = std::min(leader, voltages[seg][cell]);
            }
        }

        if (leader >= std::numeric_limits<float>::max())
            return duty;

        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            if (!valid[seg].all())
                continue;

            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++) {
                const float volts = voltages[seg][cell];

                if (volts <= V_FAULT_UV)
                    continue;

                if (volts - leader < DISCHARGE_THRESHOLD_V)
                    continue;

                duty[seg][cell] = commanded_duty;
            }
        }
        return duty;
    }
}

namespace app::pack::balancing {
    void init() {
        voltage_channel.subscribe(voltage_sub);
        setRequest({});
    }

    void tick() {
        while (const auto stats = voltage_sub.pop(0)) {
            setRequest({
                .start_balance = true,
                .duty          = determineBalance(stats->voltages, stats->valid),
            });
        }
    }

    void stop() {
        setRequest({});
    }

    Request getRequest() {
        const io::unique_semaphore s{ request_lock };
        return shared_request;
    }

    void setRequest(const Request &r) {
        const io::unique_semaphore s{ request_lock };
        shared_request = r;
    }
}
