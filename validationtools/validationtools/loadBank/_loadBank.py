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

class LoadBank:
    def __init__(self):
        '''
        Represents a Rigol DL3021 programmable DC electronic load.
        '''
        logger = logging.getLogger(self.__class__.__name__)
        resources = visa.ResourceManager()

        resourceTag: str = ''
        for x in resources.list_resources():
            logger.debug(f"Found resource: {x}")
            if 'DL' in x:
                resourceTag = x
                break
            
        if resourceTag == '':
            print("No DL3021 detected.")
            return

        self.load = resources.open_resource(resourceTag)

    def set_current(self, current: float) -> None:
        """
        Sets the current for the load in constant current (CC) mode.

        current: float, desired current in Amps
        """
        self.load.write(f':SOUR:CURR {current}')

    def set_voltage(self, voltage: float) -> None:
        """
        Sets the voltage for the load in constant voltage (CV) mode.

        voltage: float, desired voltage in Volts
        """
        self.load.write(f':SOUR:VOLT {voltage}')

    def set_resistance(self, resistance: float) -> None:
        """
        Sets the resistance for the load in constant resistance (CR) mode.

        resistance: float, desired resistance in Ohms
        """
        self.load.write(f':SOUR:RES {resistance}')

    def set_power(self, power: float) -> None:
        """
        Sets the power for the load in constant power (CP) mode.

        power: float, desired power in Watts
        """
        self.load.write(f':SOUR:POW {power}')

    def measure_voltage(self) -> float:
        """
        Measures the input voltage.

        returns: float, measured voltage in Volts
        """
        return float(self.load.query(':MEAS:VOLT?'))

    def measure_current(self) -> float:
        """
        Measures the input current.

        returns: float, measured current in Amps
        """
        return float(self.load.query(':MEAS:CURR?'))

    def measure_power(self) -> float:
        """
        Measures the input power.

        returns: float, measured power in Watts
        """
        return float(self.load.query(':MEAS:POW?'))

    def enable_load(self) -> None:
        """
        Enables the load input.
        """
        self.load.write(':INP ON')

    def disable_load(self) -> None:
        """
        Disables the load input.
        """
        self.load.write(':INP OFF')

    def reset(self) -> None:
        """
        Resets the load to its default settings.
        """
        self.load.write('*RST')

    def get_id(self) -> str:
        """
        Gets the device identification string.

        returns: string, device identification
        """
        return self.load.query('*IDN?')
