import pyvisa as visa
import time

class loadBank:
    def __init__(self):
        '''
        Represents a Rigol DL3021 programmable DC electronic load.
        '''
        resources = visa.ResourceManager()

        resourceTag = ''
        for x in resources.list_resources():
            print(x)
            if 'DL' in x:
                resourceTag = x

        if resourceTag == '':
            print("No DL3021 detected.")
            return

        self.load = resources.open_resource(resourceTag)

    def set_current(self, current):
        """
        Sets the current for the load in constant current (CC) mode.

        current: float, desired current in Amps
        """
        self.load.write(f':SOUR:CURR {current}')

    def set_voltage(self, voltage):
        """
        Sets the voltage for the load in constant voltage (CV) mode.

        voltage: float, desired voltage in Volts
        """
        self.load.write(f':SOUR:VOLT {voltage}')

    def set_resistance(self, resistance):
        """
        Sets the resistance for the load in constant resistance (CR) mode.

        resistance: float, desired resistance in Ohms
        """
        self.load.write(f':SOUR:RES {resistance}')

    def set_power(self, power):
        """
        Sets the power for the load in constant power (CP) mode.

        power: float, desired power in Watts
        """
        self.load.write(f':SOUR:POW {power}')

    def measure_voltage(self):
        """
        Measures the input voltage.

        returns: float, measured voltage in Volts
        """
        return float(self.load.query(':MEAS:VOLT?'))

    def measure_current(self):
        """
        Measures the input current.

        returns: float, measured current in Amps
        """
        return float(self.load.query(':MEAS:CURR?'))

    def measure_power(self):
        """
        Measures the input power.

        returns: float, measured power in Watts
        """
        return float(self.load.query(':MEAS:POW?'))

    def enable_load(self):
        """
        Enables the load input.
        """
        self.load.write(':INP ON')

    def disable_load(self):
        """
        Disables the load input.
        """
        self.load.write(':INP OFF')

    def get_id(self):
        """
        Gets the device identification string.

        returns: string, device identification
        """
        return self.load.query('*IDN?')

# Example usage
if __name__ == "__main__":
    load_bank = loadBank()
    print(load_bank.get_id())
    load_bank.set_current(1.51)  # Set current to 1.51A
    load_bank.enable_load()
    time.sleep(5)  # Wait for 5 seconds
    voltage = load_bank.measure_voltage()
    current = load_bank.measure_current()
    power = load_bank.measure_power()
    print(f"Voltage: {voltage} V, Current: {current} A, Power: {power} W")
    load_bank.disable_load()