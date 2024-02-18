"""
Example of how to use the serial debugger.
"""

import serial
import serial.tools.list_ports
import serial_debugger


# List available COM ports.
ports = list(serial.tools.list_ports.comports())
print("Found COM ports:")
for port in ports:
    print(port.device)

# Transmit data.
vc = serial_debugger.VC(ports[0].device)
print(vc.gpio_read("LED_GPIO"))
vc.gpio_write("LED_GPIO", True)
print(vc.gpio_read("LED_GPIO"))
vc.gpio_write("LED_GPIO", False)
print(vc.gpio_read("LED_GPIO"))
print(vc.adc_read("INV_R_PWR_I_SNS"))
