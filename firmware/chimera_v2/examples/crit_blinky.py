import time
import chimera_v2

# Config.
RATE_HZ = 1.0
DUTY_CYCLE = 0.5
LED_NET_NAME = "GPIO_LED"

# Init device.
chimera_device = chimera_v2.CRIT()

# Main loop.
while True:
    print("On")
    chimera_device.gpio_write(LED_NET_NAME, True)
    time.sleep(DUTY_CYCLE / RATE_HZ)

    print("Off")
    chimera_device.gpio_write(LED_NET_NAME, False)
    time.sleep((1.0 - DUTY_CYCLE) / RATE_HZ)
