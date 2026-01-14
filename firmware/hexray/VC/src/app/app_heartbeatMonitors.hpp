#pragma once
#include <cstdint>
#include <cstddef>
#include "app_heartbeatMonitor.h"

namespace vc::app::heartbeat_monitors {
class HeartbeatMonitor{
    public: 
    constexpr explicit HeartbeatBoard(const char *name, bool *getter, void *resetter, void *fault_setter, 
        bool *fault_getter, uint32_t timeout_ms): getter_(getter), resetter_(resetter), fault_setter_
        (fault_setter), fault_getter_(fault_getter), timeout_ms_(timeout_ms), name_(name) {}
    HeartBeatMonitor hb_monitor()
    {

    }
    
    private:
    const getter_{};
    const resetter_{};    
    const fault_setter_{};
    const fault_getter_{};
    const uint32_t timeout_ms_{};


}
//extern const HeartbeatMonitor hb_monitor;

} // namespace vc::app::heartbeat_monitors