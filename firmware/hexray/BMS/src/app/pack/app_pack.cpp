#include "app_pack.hpp"

#include "io_semaphore.hpp"

namespace
{
io::semaphore       snapshot_lock{ true };
app::pack::Snapshot shared_snapshot{};
} // namespace

namespace app::pack
{
void publish(const Snapshot &snapshot)
{
    const io::unique_semaphore lock{ snapshot_lock };
    shared_snapshot = snapshot;
}

BalancingView balancingView()
{
    const io::unique_semaphore lock{ snapshot_lock };
    return BalancingView{
        .voltage_stats = shared_snapshot.voltage_stats,
    };
}

SocView socView()
{
    const io::unique_semaphore lock{ snapshot_lock };
    return SocView{
        .tractive_stats    = shared_snapshot.tractive_stats,
        .voltage_stats     = shared_snapshot.voltage_stats,
        .temperature_stats = shared_snapshot.temperature_stats,
    };
}

TractiveView tractiveView()
{
    const io::unique_semaphore lock{ snapshot_lock };
    return TractiveView{
        .tractive_stats = shared_snapshot.tractive_stats,
    };
}

Snapshot latest()
{
    const io::unique_semaphore lock{ snapshot_lock };
    return shared_snapshot;
}
} // namespace app::pack
