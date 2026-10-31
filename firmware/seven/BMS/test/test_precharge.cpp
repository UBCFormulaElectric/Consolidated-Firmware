#include "app_precharge.hpp"
#include "test_fakes.hpp"
#include "app_states.hpp"
#include "test_BMSBase.hpp"
#include "app_segments.hpp"
#include "io_irs.hpp"
#include "app_canAlerts.hpp"

using namespace app::can_utils;
using namespace app::states;
using namespace app::precharge;

class prechargeTest : public BMSBaseTest
{
  protected:
    // Drives the charge state into a "clean" precondition: charger connected, user enabled,
    // contactors closed, all Elcon faults cleared, and cells in a mid-charge state.
    // Individual tests override whichever knobs they care about.
    void EnterChargeStateClean()
    {
        fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
        fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_CONNECTED_WALL);
        app::can_rx::Debug_StartCharging_update(true);

        app::can_rx::Elcon_HardwareFailure_update(false);
        app::can_rx::Elcon_ChargerOverTemperature_update(false);
        app::can_rx::Elcon_InputVoltageError_update(false);
        app::can_rx::Elcon_ChargingDisabled_update(false);
        app::can_rx::Elcon_CommunicationTimeout_update(false);

        //arbitrary for charigng start
        app::can_rx::Elcon_OutputCurrent_update(5.0f);
        app::can_rx::Elcon_OutputVoltage_update(400.0f);

        fakes::adbms::setCellVoltage(0, 0, 3.8f);
        // fakes::segments::setMaxCellTemp(25.0f);

        app::StateMachine::set_current_state(&charge_state);
    }
};
/*
Tests I wanna run:

Cooldown state (when timer state running)

Defining HV voltage or not

Test different pack voltages for setting threshold voltage (pack voltage * precharge threshold)





Set arbitrary voltage threshold:
- Voltage gets above threshold before timer runs out:
should move to success state

- Voltage below threshold when timer runs out
goes to failed state

- Voltage under threshold whne timer runs out too many times
goes to failed critical state

-AIR+ latch open, fault  --  do we test AIR+? We dont check currently
-AIR- latch open, fault
- closed, runs

- number of precharge faults > precharge limit, reset




*/