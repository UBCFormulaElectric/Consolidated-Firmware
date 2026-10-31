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
};

/*
Getters in precharge: 

// ts voltage
app::ts::getVoltage();

// pack voltage
app::segments::shared::getPackVoltage().value();

// air -
io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN)

//precharge for charging
set_precharge_for_charging{

}



- precharge failure state based on timings
- fake accumulator voltage and ts voltage
- feed into precharge functions
- takes in as inputs : Air condiiton, accumulator and ts voltage
- set TS voltage, use same getter to set

float ts_voltage 

//bms/io
io::ts::getVoltage(){

}

//fakes.cpp
io::ts::getVoltage(){
    return ts_voltage;
}

io::ts::setVoltage(){
    ts_voltage = v
}


//tests
io::ts::setVoltage(400)

runTime()

*/






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