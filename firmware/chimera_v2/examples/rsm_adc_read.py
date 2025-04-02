import time
import chimera_v2

# Config.
ADC_CHANNEL = "ADC_SUSP_TRAVEL_RR_3V3"

# Init device.
chimera_device = chimera_v2.RSM()

# Main loop.
while True:
    time.sleep(0.1)
    print(chimera_device.adc_read(ADC_CHANNEL))
