#include "test_fakes.hpp"

#include <gtest/gtest.h>


#include "io_tractiveSystem.hpp"
#include "io_time.hpp"

#include "app_segments.hpp"

// //fakes.cpp
// io::ts::getVoltage(){
//     return ts_voltage;
// }

// io::ts::setVoltage(){
//     ts_voltage = v
// }

//getters
const float app::segments::shared::getPackVoltage().value(){
    return packVoltage
}

const float ts_voltage{
    return app::ts::getVoltage();
}

const bool {
    return (io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
}

//setters
const float set_pack_voltage(float voltage){
    
}

const float set_ts_voltage(float voltage){
    
}

const bool set_air_negative(bool air_negative_state){
}



    const bool is_air_negative_open = (io::irs::negativeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);

