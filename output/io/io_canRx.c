/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

/* ------------------------------- Includes ------------------------------- */

#include "io_canRx.h"
#include "app_canRx.h"
#include "app_canUtils.h"

/* ------------------------- Function Definitions ------------------------- */

bool io_canRx_filterMessageId(uint32_t std_id)
{
    bool is_found = false;
    switch (std_id)
    {
        {
            is_found = true;
            break;
        }
        default:
        {
            // Do nothing
            break;
        }
    }
    
    return is_found;
}

void io_canRx_updateRxTableWithMessage(JsonCanMsg* msg)
{
    switch (msg->std_id)
    {
        default:
        {
            // Do nothing
            break;
        }
    }
}