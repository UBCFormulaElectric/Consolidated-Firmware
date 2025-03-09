import time
import chimera_v2

ssm_usb_device = chimera_v2._UsbDevice("ssm")
while True:
    time.sleep(1)
    ssm_usb_device.read(5)

# # Config.
# RATE_HZ = 10.0
# DUTY_CYCLE = 0.5
# LED_NET_NAME = "DEBUG_LED"

# # Init device.
# chimera_device = chimera_v2.SSM()

# # Main loop.
# while True:
#     print("On")
#     chimera_device.gpio_write(LED_NET_NAME, True)
#     time.sleep(DUTY_CYCLE / RATE_HZ)

#     print("Off")
#     chimera_device.gpio_write(LED_NET_NAME, False)
#     time.sleep((1.0 - DUTY_CYCLE) / RATE_HZ)

