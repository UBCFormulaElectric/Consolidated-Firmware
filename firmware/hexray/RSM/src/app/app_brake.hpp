#pragma once

/*
* Broadcast brake info on CAN
*/

namespace app::brake
{
    /*
    * Check if the brake is actuated through digital signal from the comparator on the RSM
    * return True if brake is actuated, false if brake is not
    */
    bool isActuated();
    
    void broadcast();
}
