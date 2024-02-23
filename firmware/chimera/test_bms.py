"""
Example of how to use the serial debugger.
"""

import serial
import serial.tools.list_ports
import time
import chimera


# List available COM ports.
ports = list(serial.tools.list_ports.comports())
print("Found COM ports:")
for port in ports:
    print(port.device)

# Transmit data.
fsm = chimera.FSM(ports[1].device)

# Note: All testable FSM pin names are listed in Consolidated-Firmware/chimera/board_protos/FSM.proto

####### WRITE YOUR SCRIPT HERE

for i in range(4):
    fsm.gpio_write("LED", True)
    print("write true", fsm.gpio_read("LED"))
    time.sleep(0.1)
    fsm.gpio_write("LED", False)
    print("write false", fsm.gpio_read("LED"))
    time.sleep(0.1)


# Read ADC value: VBAT_SENSE
print("read adc", fsm.adc_read("APPS2_3V3"))

#######
