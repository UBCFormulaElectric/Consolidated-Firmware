from powerSupply import PowerSupply
import time


power_supply = PowerSupply()
print(power_supply.get_id())

power_supply.set_voltage(5.0)  # Set voltage to 5V
power_supply.set_current(1.51)  # Set current to 1.51A

power_supply.enable_output()

time.sleep(5)  # Wait for 5 seconds

voltage = power_supply.measure_voltage()
current = power_supply.measure_current()

print(f"Voltage: {voltage} V, Current: {current} A")

power_supply.disable_output()