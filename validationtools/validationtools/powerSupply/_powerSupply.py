import datetime
import pyvisa as visa
import time
import numpy as np
from tqdm import tqdm
import pandas as pd

class PowerSupply:
    def __init__(self):
        '''
        Represents a Rigol DP821A programmable DC power supply.
        '''
        resources = visa.ResourceManager()

        resourceTag = ''
        for x in resources.list_resources():
            print(x)
            if 'DP' in x:
                resourceTag = x
                break
            
        if resourceTag == '':
            print("No DP821A detected.")
            return

        self.ps = resources.open_resource(resourceTag)

    def set_voltage(self, voltage, channel=1):
        """
        Sets the voltage for the power supply.

        voltage: float, desired voltage in Volts
        channel: integer, power supply channel number (default is 1)
        """
        self.ps.write(f':INST:NSEL {channel}')
        self.ps.write(f':VOLT {voltage}')

    def set_current(self, current, channel=1):
        """
        Sets the current for the power supply.

        current: float, desired current in Amps
        channel: integer, power supply channel number (default is 1)
        """
        self.ps.write(f':INST:NSEL {channel}')
        self.ps.write(f':CURR {current}')

    def measure_voltage(self, channel=1):
        """
        Measures the output voltage.

        channel: integer, power supply channel number (default is 1)
        returns: float, measured voltage in Volts
        """
        self.ps.write(f':INST:NSEL {channel}')
        return float(self.ps.query(':MEAS:VOLT?'))

    def measure_current(self, channel=1):
        """
        Measures the output current.

        channel: integer, power supply channel number (default is 1)
        returns: float, measured current in Amps
        """
        self.ps.write(f':INST:NSEL {channel}')
        return float(self.ps.query(':MEAS:CURR?'))

    def enable_output(self, channel=1):
        """
        Enables the output for the specified channel.

        channel: integer, power supply channel number (default is 1)
        """
        self.ps.write(f':INST:NSEL {channel}')
        self.ps.write(':OUTP ON')

    def disable_output(self, channel=1):
        """
        Disables the output for the specified channel.

        channel: integer, power supply channel number (default is 1)
        """
        self.ps.write(f':INST:NSEL {channel}')
        self.ps.write(':OUTP OFF')

    def get_id(self):
        """
        Gets the device identification string.

        returns: string, device identification
        """
        return self.ps.query('*IDN?')

# Example usage
if __name__ == "__main__":
    power_supply = PowerSupply()
    print(power_supply.get_id())
    # power_supply.set_voltage(5.0)  # Set voltage to 5V
    power_supply.set_current(1.51, 2)
    power_supply.enable_output(2)
    
    time.sleep(5)

    current = power_supply.measure_current(2)
    voltage = power_supply.measure_voltage(2)
    print(f"Voltage: {voltage} V, Current: {current} A")
    power_supply.disable_output()
