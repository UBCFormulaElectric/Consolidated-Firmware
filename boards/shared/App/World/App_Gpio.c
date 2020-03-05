#include "App_Gpio.h"

#define MAX_GPIO_INTERFACES_TO_ALLOC 100

struct App_Gpio_Interface
{
    void (*write_pin)(
        App_Gpio_Peripheral peripheral,
        uint16_t            gpio_pin,
        App_Gpio_PinState   pin_state);
};

App_Gpio_Interface *App_Gpio_Alloc(void (*write_pin)(
    App_Gpio_Peripheral peripheral,
    uint16_t            gpio_pin,
    App_Gpio_PinState   pin_state))
{
    static unsigned int alloc_index = 0;
    static App_Gpio_Interface
        gpio_interfaces_to_allocate[MAX_GPIO_INTERFACES_TO_ALLOC];

    if (alloc_index > MAX_WORLDS_TO_ALLOCATE - 1)
    {
        return NULL;
    }

    App_Gpio_Interface *gpio_interface =
        &gpio_interfaces_to_allocate[alloc_index++];

    gpio_interface->write_pin = write_pin;

    return world;
}

void App_Gpio_WritePin(
    App_Gpio_Interface *gpio_interface,
    App_Gpio_Peripheral peripheral,
    uint16_t            gpio_pin,
    App_Gpio_PinState   pin_state)
{
    gpio_interface->write_pin(peripheral, gpio_pin, pin_state);
}
