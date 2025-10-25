#pragma once

namespace io::brake
{
    /*
    * Check if the brake is actuated through digital signal from the comparator on the RSM
    * return True if brake is actuated, false if brake is not
    */
    bool isActuated(); 

    /*
    * Get pressure in psi from the ___BrakePressureSensorName___
    * return the pressure of primary brake pressure sensor in psi
    */
    float getRearPressurePsi(); 

    /*
    * return the hardware overcurrent/short-circuit status for the brake system
    */
    bool OCSC(); 
}
