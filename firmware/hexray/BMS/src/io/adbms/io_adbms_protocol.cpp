#include "io_adbms_protocol.hpp"

namespace io::adbms
{
SemaphoreHandle_t spiMutex()
{
    static StaticSemaphore_t storage;
    static SemaphoreHandle_t handle = xSemaphoreCreateRecursiveMutexStatic(&storage);
    return handle;
}
} // namespace io::adbms
