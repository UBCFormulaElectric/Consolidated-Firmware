from enum import Enum
from dataclasses import dataclass
import datetime
import time

import pyvisa as visa
import numpy as np
import pandas as pd


class DS1054Z:
    class Channel(Enum):
        """Representation of a scope channel."""

        ONE = 1
        TWO = 2
        THREE = 3
        FOUR = 4

    def __init__(self):
        """Create an abstraction around a Rigol DS1054Z oscilloscope."""
        resources = visa.ResourceManager()

        scope_names = list(
            filter(
                lambda resource_name: "DS1ZA18" in resource_name,
                resources.list_resources(),
            )
        )

        assert len(scope_names) > 0
        if len(scope_names) > 1:
            print("Detected multiple scopes, defaulting to first.")

        self.scope = resources.open_resource(scope_names[0])

    def stop(self):
        """Stop the scope."""
        self.scope.write(":STOP")

    def run(self):
        """Run the scope."""
        self.scope.write(":RUN")

    def single(self):
        """Set a single capture trigger"""
        self.scope.write(":SING")

    def channel_on(self, channel: Channel):
        """
        Turns on the given channel.

        Args:
            channel: Scope channel to turn on.

        """
        self.scope.write(f":CHANnel{channel.value}:DISPlay ON")

    def channel_off(self, channel: Channel):
        """
        Turns off the given channel.

        Args:
            channel: Scope channel to turn off.

        """
        self.scope.write(f"CHANnel{channel.value}:DISPlay OFF")

    def channel_only_on(self, channel: Channel):
        """
        Turns on the given channel and turns off all of the other channels.

        Args:
            channel: Scope channel to keep on.

        """
        for curr in self.Channel:
            self.channel_on(curr) if channel == curr else self.channel_off(curr)

    def channel_all_on(self):
        """Turns on all channels."""
        for curr in self.Channel:
            self.channel_on(curr)

    def offset(self, channel: Channel, value: float):
        """
        Sets the DC offset of the given channel.

        Args:
            channel: Scope channel to target.
            value: Offset to set, in volts.

        """
        self.scope.write(f":CHANnel{channel.value}:OFFSet {value}")

    def screenshot(self, file_path: str | None = None):
        """
        Save a png screenshot.

        Args:
            file_path: Png path to save image. Will default to timestamp based file name if None.

        """

        # Read image bytes, and strip off first 11 bytes.
        self.scope.write(":DISP:DATA? ON,OFF,PNG")
        raw_data = self.scope.read_raw()[11:]

        # Open file.
        file_out = (
            open(file_path, "wb")
            if file_path is not None
            else open(
                rf"rigol_ds1054z_{datetime.datetime.now().strftime(r'%Y-%m-%d_%H-%M-%S')}.png",
                "wb",
            )
        )

        # Write.
        file_out.write(raw_data)
        file_out.close()

    def vertical_scale(self, channel: Channel, scale: float):
        """
        Set the vertical scale of a channel.

        Args:
            channel: Channel to target.
            scale: Scale in Volts, 10mV to 100V.

        """

        assert 0.01 < scale < 100
        self.scope.write(f":CHANnel{channel.value}:SCALe {scale}")

    def vertical_scale_all(self, scale: float):
        """
        Set the vertical scale of all channels.

        Args:
            scale: Scale in Volts, 10mV to 100V.

        """

        for curr in self.Channel:
            self.vertical_scale(curr, scale)

    def horizontal_scale(self, scale: float):
        """
        Sets the horizontal scale.

        Args:
            scale: Seconds per division.

        """

        self.scope.write(f":TIMebase:MAIN:SCALe {scale}")

    def write(self, message: str):
        """
        Write a command directly to the scope.

        Args:
            message: Message to pass to the scope.

        """

        self.scope.write(message)

    @dataclass
    class DataPreamble:
        """Data preamble."""

        format: int
        type: int
        points: int
        count: int
        x_increment: float
        x_origin: float
        x_reference: float
        y_increment: float
        y_origin: float
        y_reference: float

    def get_data_premable(self) -> DataPreamble:
        """
        Get information about oscilloscope axes.

        Returns:
            Info about oscilloscope axes.

        """

        raw_preamble = self.scope.query(":wav:pre?").split(",")

        return self.DataPreamble(
            format=int(raw_preamble[0]),
            type=int(raw_preamble[1]),
            points=int(raw_preamble[2]),
            count=int(raw_preamble[3]),
            x_increment=float(raw_preamble[4]),
            x_origin=float(raw_preamble[5]),
            x_reference=float(raw_preamble[6]),
            y_increment=float(raw_preamble[7]),
            y_origin=float(raw_preamble[8]),
            y_reference=float(raw_preamble[9]),
        )

    def channel_read_data(self, channel: Channel) -> tuple[np.ndarray, np.ndarray]:
        """
        Collect voltage vs. time data from a channel.

        Args:
            channel: Target channel.

        Returns:
            A tuple, whose first item is a Numpy array of times,
            and second item is the corresponding voltage values.

        """

        self.channel_on(channel)
        self.scope.write(":stop")
        self.scope.write(f":wav:sour chan{channel.value}")
        self.scope.write(":wav:mode norm")
        self.scope.write(":wav:form byte")
        info = self.get_data_premable()

        # Internal utility.
        def _read_data(start: int, length: int) -> np.ndarray:
            """
            Read ``length`` data points from a start point.

            Args:
                start: Start point index, starting at 0.
                length: Number of points to read.

            Returns:
                A Numpy array with the data points read.
            """

            # Make request.
            self.scope.write(f":wav:star {start + 1}")
            self.scope.write(f":wav:stop {start + length}")
            self.scope.write(":wav:data?")

            # Time to wait before reading again.
            REQUEST_GRACE_PERIOD_SECS = 0.3
            time.sleep(REQUEST_GRACE_PERIOD_SECS)

            # Trim first 11 bytes.
            data = self.scope.read_raw()[11:]

            # Create numpy array from buffer.
            return np.frombuffer(data, "B")

        # Read at most 250000 points per request chunk.
        MAX_POINTS_PER_CHUNK = 250000
        num_chunks = info.points // MAX_POINTS_PER_CHUNK
        last_chunk_points = info.points % MAX_POINTS_PER_CHUNK

        # Extract data.
        chunks = [
            _read_data(chunk_index * MAX_POINTS_PER_CHUNK, MAX_POINTS_PER_CHUNK)
            for chunk_index in range(num_chunks)
        ]

        # Request data from last chunk as well.
        if last_chunk_points > 0:
            start = num_chunks * MAX_POINTS_PER_CHUNK + 1
            chunks.append(_read_data(start, last_chunk_points))

        # Flatten out chunks.
        data = np.concatenate(chunks)

        # Convert data to volts.
        v = (data - info.y_origin - info.y_reference) * info.y_increment

        # Get timespan.
        t = np.arange(0, info.points * info.x_increment, info.x_increment)

        # Make sure both arrays have the same length.
        cutoff = min(t.size, v.size)
        t = t[:cutoff]
        v = v[:cutoff]

        return (t, v)

    def channel_read_and_save_data(
        self, channel: Channel, file_path: str | None = None
    ):
        """
        Save .csv file with time vs. voltage data for a given channel.

        Args:
            channel: Target channel.
            file_path: CSV path to save data. Will default to timestamp based file name if None.

        """

        (t, v) = self.channel_read_data(channel)
        data_frame = pd.DataFrame({"time": t, "voltage": v})

        if file_path is not None:
            data_frame.to_csv(file_path, index=False)
        else:
            timestamp = datetime.datetime.now().strftime(r"%Y-%m-%d_%H-%M-%S")
            data_frame.to_csv(
                f"rigol_waveform_data_channel_{channel.value}_{timestamp}.csv",
                index=False,
            )

    def channel_read_and_save_all_data(self, file_path: str | None = None):
        """
        Save .csv file with time vs. voltage data for all channels.

        Args:
            file_path: CSV path to save data. Will default to timestamp based file name if None.

        """

        # Collect data.
        collected_data = [self.channel_read_data(channel) for channel in self.Channel]
        voltages = [collected_data[1] for data in collected_data]

        # Use the first channel's time array as the x-axis (all time arrays should be the same).
        times = collected_data[0][0]

        # Pack data frame.
        data_frame = pd.DataFrame(
            {
                "time": times,
                "Channel 1": voltages[0],
                "Channel 2": voltages[1],
                "Channel 3": voltages[2],
                "Channel 4": voltages[3],
            }
        )

        # Save to CSV.
        if file_path is not None:
            data_frame.to_csv(file_path, index=False)
        else:
            timestamp = datetime.datetime.now().strftime(r"%Y-%m-%d_%H-%M-%S")
            data_frame.to_csv(
                f"rigol_waveform_all_channels_{timestamp}.csv", index=False
            )
