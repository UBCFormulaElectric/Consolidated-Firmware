import pyvisa as visa


class DP821A:
    def __init__(self):
        """Create an abstraction around a Rigol DP821A programmable DC power supply."""
        resources = visa.ResourceManager()

        power_supply_names = list(
            filter(
                lambda resource_name: "DP821A" in resource_name,
                resources.list_resources(),
            )
        )

        assert len(power_supply_names) > 0
        if len(power_supply_names) > 1:
            print("Detected multiple power supplies, defaulting to first.")

        self.ps = resources.open_resource(power_supply_names[0])

    def set_voltage(self, voltage: float, channel: int = 1):
        """
        Sets the voltage for the power supply.

        Args:
            voltage: Desired voltage in Volts.
            channel: Power supply channel number (default is 1).

        """

        self.ps.write(f":INST:NSEL {channel}")
        self.ps.write(f":VOLT {voltage}")

    def set_current(self, current: float, channel: int = 1):
        """
        Sets the current for the power supply.

        Args:
            current: Desired current in Amps
            channel: Power supply channel number (default is 1).

        """

        self.ps.write(f":INST:NSEL {channel}")
        self.ps.write(f":CURR {current}")

    def measure_voltage(self, channel: int = 1) -> float:
        """
        Measures the output voltage.

        Args:
            channel: Power supply channel number (default is 1).

        Returns:
            Measured voltage in Volts.

        """
        self.ps.write(f":INST:NSEL {channel}")
        return float(self.ps.query(":MEAS:VOLT?"))

    def measure_current(self, channel: int = 1) -> float:
        """
        Measures the output current.

        Args:
            channel: Power supply channel number (default is 1).

        Returns:
            Measured current in Amps.

        """
        self.ps.write(f":INST:NSEL {channel}")
        return float(self.ps.query(":MEAS:CURR?"))

    def enable_output(self, channel: int = 1):
        """
        Enables the output for the specified channel.

        Args:
            channel: Power supply channel number (default is 1).

        """
        self.ps.write(f":INST:NSEL {channel}")
        self.ps.write(":OUTP ON")

    def disable_output(self, channel: int = 1):
        """
        Disables the output for the specified channel.

        Args:
            channel: Power supply channel number (default is 1).

        """
        self.ps.write(f":INST:NSEL {channel}")
        self.ps.write(":OUTP OFF")

    def reset(self):
        """Resets the power supply to its default settings."""
        self.ps.write("*RST")

    def get_id(self) -> str:
        """
        Gets the device identification string.

        returns:
            Device identification.

        """
        return self.ps.query("*IDN?")
