extern "C" {
    #include "states/app_states.h"

    #include "io_irs.h"
    #include "app_canTx.h"
    #include "app_canRx.h"
    #include "io_ltc6813.h"
}

namespace {
    void balancingStateRunOnEntry() {
        app_canTx_BMS_State_set(BMS_BALANCING_STATE);
    }

    void balancingStateRunOnTick100Hz() {
        const bool
    }


}