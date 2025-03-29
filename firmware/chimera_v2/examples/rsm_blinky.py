import time
import chimera_v2

# Config.
RATE_HZ = 10.0
DUTY_CYCLE = 0.5
LED_NET_NAME = "DEBUG_LED"

# Init device.
chimera_device = chimera_v2.RSM()


# Main loop.
while True:
   
   print(chimera_device.adc_read("ADC_SUSP_TRAVEL_RR_3V3"))
