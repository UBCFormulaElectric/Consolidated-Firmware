extern "C" {
    #include "app_canUtils.h"
}

namespace io::irs {
    /**
    * Check if the IR- is closed
    * @returns the state of the IR- relay.
    */
    ContactorState io_irs_negativeState(void);

    /**
    * Sets the IR- relay to the desired state
    */
    void io_irs_setPositive(ContactorState state);

    /**
    * Check if the IR+ is closed
    * @returns the state of the IR+ relay.
    */
    ContactorState io_irs_positiveState(void);

    /**
    * Set the precharge relay to the desired state
    */
    void io_irs_setPrecharge(ContactorState state);

    /**
    * Check if the precharge relay is closed
    * @returns the state of the precharge relay.
    */
    ContactorState io_irs_prechargeState(void);
}