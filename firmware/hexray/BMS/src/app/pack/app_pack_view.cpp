#include "app_pack.hpp"

#include "io_semaphore.hpp"

namespace
{
io::semaphore       snapshot_lock{ true };
app::pack::Snapshot shared_snapshot{};
} // namespace

namespace app::pack::view
{
void publish(const Snapshot &snapshot)
{
    const io::unique_semaphore lock{ snapshot_lock };
    shared_snapshot = snapshot;
}

Snapshot latest()
{
    const io::unique_semaphore lock{ snapshot_lock };
    return shared_snapshot;
}
} // namespace app::pack
