import pyvisa as visa


class DL3021:
    def __init__(self):
        """Create an abstraction around a Rigol DL3021 programmable DC electronic load."""
        resources = visa.ResourceManager()

        load_bank_names = list(
            filter(
                lambda resource_name: "DL3021" in resource_name,
                resources.list_resources(),
            )
        )

        assert len(load_bank_names) > 0
        if len(load_bank_names) > 1:
            print("Detected multiple load banks, defaulting to first.")

        self.load = resources.open_resource(load_bank_names[0])

    def set_current(self, current: float):
        """
        Sets the current for the load in constant current (CC) mode.

        Args:
            current: float, desired current in Amps.

        """
        self.load.write(f":SOUR:CURR {current}")

    def set_voltage(self, voltage: float):
        """
        Sets the voltage for the load in constant voltage (CV) mode.

        Args:
            voltage: float, desired voltage in Volts.

        """
        self.load.write(f":SOUR:VOLT {voltage}")

    def set_resistance(self, resistance: float):
        """
        Sets the resistance for the load in constant resistance (CR) mode.

        Args:
            resistance: float, desired resistance in Ohms.

        """
        self.load.write(f":SOUR:RES {resistance}")

    def set_power(self, power: float):
        """
        Sets the power for the load in constant power (CP) mode.

        Args:
            power: float, desired power in Watts.

        """
        self.load.write(f":SOUR:POW {power}")

    def measure_voltage(self) -> float:
        """
        Measures the input voltage.

        Returns:
            Measured voltage in Volts.

        """

        return float(self.load.query(":MEAS:VOLT?"))

    def measure_current(self) -> float:
        """
        Measures the input current.

        Returns:
            Measured current in Amps.

        """
        return float(self.load.query(":MEAS:CURR?"))

    def measure_power(self) -> float:
        """
        Measures the input power.

        Returns:
            Measured power in Watts.

        """
        return float(self.load.query(":MEAS:POW?"))

    def enable_load(self):
        """Enables the load input."""
        self.load.write(":INP ON")

    def disable_load(self):
        """Disables the load input."""
        self.load.write(":INP OFF")

    def reset(self):
        """Resets the load to its default settings."""
        self.load.write("*RST")

    def get_id(self) -> str:
        """
        Gets the device identification string.

        returns:
            Device identification.

        """

        return self.load.query("*IDN?")
