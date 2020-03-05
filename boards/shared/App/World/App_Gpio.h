#ifndef APP_GPIO_H
#define APP_GPIO_H

struct App_Gpio_Interface;

enum App_Gpio_Peripheral
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H
};

enum App_Gpio_PinState
{
    HIGH,
    LOW
};

App_Gpio_Interface *App_Gpio_Alloc(void (*write_pin)(
    App_Gpio_Peripheral peripheral,
    uint16_t            gpio_pin,
    App_Gpio_PinState   pin_state));

void App_Gpio_WritePin(
    App_Gpio_Interface* gpio_interface,
    App_Gpio_Peripheral peripheral,
    uint16_t            gpio_pin,
    App_Gpio_PinState   pin_state);

#endif /* APP_GPIO_H */
