#include "App_World.h"

#define MAX_WORLDS_TO_ALLOCATE 100

struct World_Interface
{
    App_Can_Interface * can_interface;
    App_Adc_Interface * adc_interface;
    App_Gpio_Interface *gpio_interface;
};

World_Interface *App_World_Alloc(
    App_Can_Interface * can_interface,
    App_Adc_Interface * adc_interface,
    App_Gpio_interface *gpio_interface)
{
    static unsigned int alloc_index = 0;
    static World_Interface        worlds_to_allocate[MAX_WORLDS_TO_ALLOCATE];

    if (alloc_index > MAX_WORLDS_TO_ALLOCATE - 1)
    {
        return NULL;
    }

    World_Interface *world = &worlds_to_allocate[alloc_index++];

    world->can_interface  = can_interface;
    world->adc_interface  = adc_interface;
    world->gpio_interface = gpio_interface;

    return world;
}

App_Can_Interface *App_World_GetCanInterface(World_Interface *world)
{
    return world->can_interface;
}

App_Adc_Interface *App_World_GetAdcInterface(World_Interface *world)
{
    return world->adc_interface;
}

App_Gpio_Interface *App_World_GetGpioInterface(World_Interface *world)
{
    return world->gpio_interface;
}
