#pragma once

namespace app::plim {
    
    /**
    * @brief runs the power and current limit checking algorithm and updates all the current limit CAN messages
    */
    void broadcast();
}