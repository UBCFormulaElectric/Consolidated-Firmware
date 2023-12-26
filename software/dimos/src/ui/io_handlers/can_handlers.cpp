#include <QtGlobal>
#include <chrono>

#include "can.h"
extern "C"
{
#include <Io_CanRx.h>
}

namespace can_handlers
{
[[noreturn]] void CanRXTask()
{
  forever
  {
    Result<CanMsg, CanReadError> res = Can_Read();
    if (res.index() == 1)
    {
      switch (get<CanReadError>(res))
      {
      case ReadInterfaceNotCreated:
        break;
      case SocketReadError:
      case IncompleteCanFrame:
          continue;
      }
      continue;
    }

    // success
    auto message = get<CanMsg>(res);
    // acquire lock
    Io_CanRx_UpdateRxTableWithMessage(&message);
    // release lock
  }
}

[[noreturn]] void CanPeriodicTXTask()
{
  using namespace std::chrono;
  forever
  {
    auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    Io_CanTx_EnqueueOtherPeriodicMsgs(ms.count());
  }
}
} // namespace can_handlers