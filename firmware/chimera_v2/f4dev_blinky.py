import time;
import chimera_v2;

usb_device = chimera_v2.UsbDevice(0x0483, 0x5740)

chimera_device = chimera_v2.F4Dev(usb_device)

while True:
    print("On")
    chimera_device.gpio_write("GPIO_6", True)
    time.sleep(1)

    print("Off")
    chimera_device.gpio_write("GPIO_6", False)
    time.sleep(1)