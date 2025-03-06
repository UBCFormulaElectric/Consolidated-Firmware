import time
import chimera_v2

# Config.
READ_INTERVAL_SECS = 0.01
READ_NET_NAME = "GPIO_5"

# Init device.
chimera_device = chimera_v2.F4Dev()

# Main loop.
while True:
    print("GPIO:", "HIGH" if chimera_device.gpio_read(READ_NET_NAME) else "LOW")
    time.sleep(READ_INTERVAL_SECS)
