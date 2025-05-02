from validationtools.loadBank import LoadBank
import time

# Add your code here
l = LoadBank()

l.set_current(0.250) # set it to draw 500mA
l.enable_load()

time.sleep(5) # sleep for 5 seconds before measurment

V_load = l.measure_voltage()
print(V_load)

l.disable_load()