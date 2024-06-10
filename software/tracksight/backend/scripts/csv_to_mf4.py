import os
import argparse
import pandas as pd
import numpy as np
from asammdf import MDF, Signal
import logging
from datetime import datetime

logging.basicConfig(level=logging.INFO)

NUM_SIGNALS_BEFORE_LOG = 100

logger = logging.getLogger(__name__)
script_dir = os.path.dirname(os.path.abspath(__file__))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--input",
        "-i",
        type=str,
        help="Path to input directory of CSV files.",
        default=os.path.join(script_dir, "..", "data"),
    )
    parser.add_argument(
        "--output",
        "-o",
        type=str,
        help="Path to output directory for MDF files.",
        default=os.path.join(script_dir, "..", "data_mf4"),
    )
    parser.add_argument(
        "--file",
        "-f",
        type=str,
        help="Files to decode (pass multiple with a comma-seperated string)",
        default=None,
    )
    args = parser.parse_args()

    files = args.file.split(",") if args.file is not None else None

    for csv_name in sorted(os.listdir(path=args.input)):
        if files is not None and csv_name not in files:
            logger.info(f"Skipping '{csv_name}', not in requested files.")
            continue

        if os.path.splitext(csv_name)[-1] != ".csv":
            logger.info(f"Skipping '{csv_name}', not a CSV file.")
            continue

        logger.info(f"Converting CSV to MDF: {csv_name}")

        # Load CSV as a Pandas dataframe.
        csv_path = os.path.join(args.input, csv_name)
        df = pd.read_csv(csv_path, keep_default_na=False)

        # Group signals by signal name.
        df_signals = (
            df.groupby("signal")[["time", "value", "unit"]].agg(list).reset_index()
        )

        mdf = MDF()
        for i, signal in df_signals.iterrows():
            # TODO: Support enums/value tables.
            signal_name = signal["signal"]
            signal_values = np.array(signal["value"], dtype=float)
            signal_datetimes = pd.to_datetime(
                np.array(signal["time"]), format="ISO8601"
            )
            signal_unit = signal["unit"][0] if len(signal["unit"]) > 0 else ""

            # Round timestamp to nearest millisecond.
            signal_timestamps_ms = (signal_datetimes.astype("int64") // 1e6).to_numpy()
            signal_timestamps_s = signal_timestamps_ms.astype("float64") / 1e3
            signal_timestamps_ms

            mdf.append(
                signals=Signal(
                    samples=signal_values,
                    timestamps=signal_timestamps_s,
                    name=signal_name,
                    unit=signal_unit,
                ),
                comment=signal_name,
                common_timebase=False,
            )

            # Log an update.
            if i % NUM_SIGNALS_BEFORE_LOG == 0:
                percent_converted = i / len(df_signals) * 100
                logger.info(f"Converted {int(percent_converted)}% signals in CSV file.")

        # Write out MDF file.
        csv_name_no_extension = os.path.splitext(csv_name)
        out_path = os.path.join(args.output, csv_name_no_extension[0] + ".mf4")

        if not os.path.exists(os.path.dirname(out_path)):
            # Create output path if it doesn't already exist.
            os.makedirs(os.path.dirname(out_path))

        mdf.save(out_path, overwrite=True)
