#include <stdlib.h>
#include <stdint.h>
#include "module.h"
#include "App_SharedMacros.h"

extern module_s tractive_system;

// Modules belonging to the particular ECU are placed within this arr
static module_s *modules[] = { &tractive_system };

void module_1HzTick(void)
{
    for (uint16_t i = 0U; i < NUM_ELEMENTS_IN_ARRAY(modules); i++)
    {
        if (modules[i]->tick_1Hz != NULL)
        {
            modules[i]->tick_1Hz();
        }
    }
}

void module_10HzTick(void)
{
    for (uint16_t i = 0U; i < NUM_ELEMENTS_IN_ARRAY(modules); i++)
    {
        if (modules[i]->tick_10Hz != NULL)
        {
            modules[i]->tick_10Hz();
        }
    }
}

void module_100HzTick(void)
{
    for (uint16_t i = 0U; i < NUM_ELEMENTS_IN_ARRAY(modules); i++)
    {
        if (modules[i]->tick_100Hz != NULL)
        {
            modules[i]->tick_100Hz();
        }
    }
}

void module_1kHzTick(void)
{
    for (uint16_t i = 0U; i < NUM_ELEMENTS_IN_ARRAY(modules); i++)
    {
        if (modules[i]->tick_1kHz != NULL)
        {
            modules[i]->tick_1kHz();
        }
    }
}
