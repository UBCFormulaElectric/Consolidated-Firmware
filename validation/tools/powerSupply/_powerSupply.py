import datetime
import pyvisa as visa
import time
import numpy as np
from tqdm import tqdm
import pandas as pd
import logging

# Configure logging
logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S')

class PowerSupply:
    def __init__(self):
        '''
        Represents a Rigol DP821A programmable DC power supply.
        '''
        logger = logging.getLogger(self.__class__.__name__)
        resources = visa.ResourceManager()

        resourceTag: str = ''
        for x in resources.list_resources():
            logger.debug(f"Found resource: {x}")
            if 'DP' in x:
                resourceTag = x
                break
            
        if resourceTag == '':
            print("No DP821A detected.")
            return

        self.ps = resources.open_resource(resourceTag)

    def set_voltage(self, voltage: float, channel: int = 1) -> None:
        """
        Sets the voltage for the power supply.

        voltage: float, desired voltage in Volts
        channel: integer, power supply channel number (default is 1)
        """
        self.ps.write(f':INST:NSEL {channel}')
        self.ps.write(f':VOLT {voltage}')

    def set_current(self, current: float, channel: int = 1) -> None:
        """
        Sets the current for the power supply.

        current: float, desired current in Amps
        channel: integer, power supply channel number (default is 1)
        """
        self.ps.write(f':INST:NSEL {channel}')
        self.ps.write(f':CURR {current}')

    def measure_voltage(self, channel: int = 1) -> float:
        """
        Measures the output voltage.

        channel: integer, power supply channel number (default is 1)
        returns: float, measured voltage in Volts
        """
        self.ps.write(f':INST:NSEL {channel}')
        return float(self.ps.query(':MEAS:VOLT?'))

    def measure_current(self, channel: int = 1) -> float:
        """
        Measures the output current.

        channel: integer, power supply channel number (default is 1)
        returns: float, measured current in Amps
        """
        self.ps.write(f':INST:NSEL {channel}')
        return float(self.ps.query(':MEAS:CURR?'))

    def enable_output(self, channel: int = 1) -> None:
        """
        Enables the output for the specified channel.

        channel: integer, power supply channel number (default is 1)
        """
        self.ps.write(f':INST:NSEL {channel}')
        self.ps.write(':OUTP ON')

    def disable_output(self, channel: int = 1) -> None:
        """
        Disables the output for the specified channel.

        channel: integer, power supply channel number (default is 1)
        """
        self.ps.write(f':INST:NSEL {channel}')
        self.ps.write(':OUTP OFF')
    
    def reset(self) -> None:
        """
        Resets the power supply to its default settings.
        """
        self.ps.write('*RST')

    def get_id(self) -> str:
        """
        Gets the device identification string.

        returns: string, device identification
        """
        return self.ps.query('*IDN?')
