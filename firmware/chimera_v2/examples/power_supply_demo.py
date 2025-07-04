import power_supply
import time

# Config.
CHANNEL = 2
CURRENT = 1.51

# Setup supply.
supply = power_supply.DP821A()
print(supply.get_id())

# Output current for 5s.
supply.set_current(CURRENT, CHANNEL)
supply.enable_output(CHANNEL)
time.sleep(5)

# Measure voltage and current over channel 2 before disabling output.
current = supply.measure_current(CHANNEL)
voltage = supply.measure_voltage(CHANNEL)
print(f"Voltage: {voltage} V, Current: {current} A")

supply.disable_output()
