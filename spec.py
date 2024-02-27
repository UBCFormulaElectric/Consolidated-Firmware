import serial_debugger

# Initialize connection to the board.
# Just opens a connection to the target. 
# Probably need to tell it the serial port to use.
# Example:
bms = serial_debugger.BMS("COM1")
fsm = serial_debugger.FSM("COM1")

# Write GPIO pin. Format is:
# <board>.gpio.<Altium net label>.write(<True/False>)
# True = high, False = low
# Example:
bms.gpio.PRECHARGE_EN.write(True)
crit.gpio.VC_RED.write(False)

# Read GPIO pin. Format is:
# <board>.gpio.<Altium net label>.read()
# Example:
igntn_gpio_state = crit.gpio.IGNTN_LED.read()
imd_ok = bms.gpio.IMD_OK.read()

# Read from an ADC channel. Format is:
# <board>.adc.<Altium net label>.read()
# Example:
apps_V = fsm.adc.APPS1.read()
bps_V = fsm.adc.BPS_F.read()

