import time
import chimera_v2

# Init device.
usb_device = chimera_v2.UsbDevice(0x0483, 0x5740)
chimera_device = chimera_v2.F4Dev(usb_device)

# Main loop.
rate_hz = 1000.0
duty_cycle = 0.5
while True:
    print("On")
    chimera_device.gpio_write("GPIO_6", True)
    time.sleep(1. / rate_hz * duty_cycle)

    print("Off")
    chimera_device.gpio_write("GPIO_6", False)
    time.sleep(1. / rate_hz * (1. - duty_cycle))