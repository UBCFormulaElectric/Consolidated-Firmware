

import argparse
import csv
from numpy.polynomial import Polynomial
from dataclasses import dataclass
import importlib.util
from pathlib import Path
import statistics
import time


REPO_ROOT = Path(__file__).resolve().parents[2]
POWER_SUPPLY_PATH = REPO_ROOT / "validationtools" / "validationtools" / "powerSupply" / "_powerSupply.py"
POWER_SUPPLY_SPEC = importlib.util.spec_from_file_location("validationtools_power_supply", POWER_SUPPLY_PATH)
if POWER_SUPPLY_SPEC is None or POWER_SUPPLY_SPEC.loader is None:
    raise ImportError(f"Could not load power supply module from {POWER_SUPPLY_PATH}")
POWER_SUPPLY_MODULE = importlib.util.module_from_spec(POWER_SUPPLY_SPEC)
POWER_SUPPLY_SPEC.loader.exec_module(POWER_SUPPLY_MODULE)
PowerSupply = POWER_SUPPLY_MODULE.PowerSupply

import chimera_v2

CHANNEL = 1
MIN_CURRENT_STEP = 0.0001
MAX_CURRENT = 1.04
SUPPLY_VOLTAGE = 5.0

SETTLE_TIME_S = 1

SAMPLES_PER_POINT = 20
SAMPLE_DELAY_S = 0.5

@dataclass(frozen=True)
class SensorConfig:
    name: str
    adc_channel: str
    start_current_a: float
    stop_current_a: float
    step_current_a: float
    csv_path: str


@dataclass(frozen=True)
class CalibrationPoint:
    loop_count: int
    commanded_current_a: float
    measured_supply_current_a: float
    effective_sensor_current_a: float
    adc_voltage_by_sensor: dict[str, float]


SENSOR_CONFIGS = {
    "400a": SensorConfig(
        name="400a",
        adc_channel="ADC_TS_ISENSE_400A",
        start_current_a=0.0,
        stop_current_a=5.0,
        step_current_a=0.25,
        csv_path="current_calibration_400a.csv",
    ),
    "50a": SensorConfig(
        name="50a",
        adc_channel="ADC_TS_ISENSE_50A",
        start_current_a=0.0,
        stop_current_a=5.0,
        step_current_a=0.25,
        csv_path="current_calibration_50a.csv",
    ),
}


def step_list(start: float, stop: float, step: float) -> list[float]:
    values = []
    current = start
    while current <= stop:
        values.append(round(current, 4))
        current += step
    return values


def average_measurements(supply: PowerSupply, bms: chimera_v2.BMS, sensors: list[SensorConfig]) -> tuple[float, dict[str, float]]:
    supply_currents = []
    adc_voltages_by_sensor = {}
    for sensor in sensors:
        adc_voltages_by_sensor[sensor.name] = []

    for _ in range(SAMPLES_PER_POINT):
        supply_currents.append(supply.measure_current(CHANNEL))
        for sensor in sensors:
            adc_voltages_by_sensor[sensor.name].append(bms.adc_read(sensor.adc_channel))
        time.sleep(SAMPLE_DELAY_S)

    average_supply_current = statistics.mean(supply_currents)
    average_adc_voltages = {}

    for sensor_name, adc_voltages in adc_voltages_by_sensor.items():
        average_adc_voltages[sensor_name] = statistics.mean(adc_voltages)

    return average_supply_current, average_adc_voltages


def linear_fit(x_vals: list[float], y_vals: list[float]) -> tuple[float, float]:
    fit = Polynomial.fit(x_vals, y_vals, 1)
    return tuple(fit.convert().coef)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Calibrate tractive-system current sensor")

    parser.add_argument("--sensor", default="both", choices=["400a", "50a", "both"], help="Sensor to calibrate")
    parser.add_argument("--start-current", type=float, default=0, help="Starting power supply set current")
    parser.add_argument("--stop-current", type=float, default=1, help="Ending power supply set current")
    parser.add_argument("--step-current", type=float, default=0.01, help="Power supply set current step")
    parser.add_argument("--max-loops", type=int, default=1, help="Number of wire loops to calibrate. Each loop multiplies the effective sensed current.")

    args = parser.parse_args()

    if args.max_loops < 1:
        parser.error("--max-loops must be at least 1")

    if args.start_current <= 0 or args.start_current >= MAX_CURRENT:
        parser.error(f"--start-current must be between 0 and {MAX_CURRENT}")
                     
    if args.stop_current <= 0 or args.stop_current >= MAX_CURRENT:
        parser.error(f"--stop-current must be between 0 and {MAX_CURRENT}")

    if args.step_current < MIN_CURRENT_STEP:
        parser.error(f"--step-current must be at least {MIN_CURRENT_STEP}")

    if args.start_current >= args.stop_current:
        parser.error("--start-current must be less than --stop-current")

    return args

def get_sensor_configs(sensor_name: str) -> list[SensorConfig]:
    if sensor_name == "both":
        return [SENSOR_CONFIGS["400a"], SENSOR_CONFIGS["50a"]]

    return [SENSOR_CONFIGS[sensor_name]]


def wait_for_next_loop(loop_count: int) -> None:
    input("\n" f"Add loop {loop_count} around the current sensor, then press Enter to continue.")


def run_calibration(supply: PowerSupply, bms: chimera_v2.BMS, sensors: list[SensorConfig], start_current_a: float, stop_current_a: float, step_current_a: float, max_loops: int) -> None:
    default_sensor = sensors[0]
    commanded_currents = frange(
        default_sensor.start_current_a if start_current_a is None else start_current_a,
        default_sensor.stop_current_a if stop_current_a is None else stop_current_a,
        default_sensor.step_current_a if step_current_a is None else step_current_a,
    )
    rows: list[CalibrationPoint] = []
    sensor_label = ", ".join(sensor.cli_name for sensor in sensors)
    csv_path = "current_calibration_both.csv" if len(sensors) > 1 else sensors[0].csv_path

    print(f"\nCalibrating sensor(s): {sensor_label}")
    for sensor in sensors:
        print(f"{sensor.cli_name} ADC channel: {sensor.adc_channel}")

    supply.set_voltage(SUPPLY_VOLTAGE_LIMIT_V, CHANNEL)

    try:
        for loop_count in range(1, max_loops + 1):
            if loop_count > 1:
                wait_for_next_loop(loop_count, sensor_label)

            print(f"\nStarting pass with {loop_count} loop(s) through the sensor")

            for commanded_current in commanded_currents:
                supply.set_current(commanded_current, CHANNEL)
                supply.enable_output(CHANNEL)
                time.sleep(SETTLE_TIME_S)

                measured_current_a, adc_voltage_by_sensor = average_measurements(
                    supply,
                    bms,
                    sensors,
                )
                effective_sensor_current_a = measured_current_a * loop_count

                rows.append(
                    CalibrationPoint(
                        loop_count=loop_count,
                        commanded_current_a=commanded_current,
                        measured_supply_current_a=measured_current_a,
                        effective_sensor_current_a=effective_sensor_current_a,
                        adc_voltage_by_sensor=adc_voltage_by_sensor,
                    )
                )

                line = (
                    f"loops={loop_count:2d}  "
                    f"set={commanded_current:6.3f} A  "
                    f"supply={measured_current_a:6.3f} A  "
                    f"effective={effective_sensor_current_a:6.3f} A"
                )
                for sensor in sensors:
                    line += f"  {sensor.cli_name}={adc_voltage_by_sensor[sensor.cli_name]:7.5f} V"
                print(line)

                supply.disable_output(CHANNEL)
                time.sleep(0.2)

    finally:
        supply.disable_output(CHANNEL)

    fieldnames = [
        "loop_count",
        "commanded_current_a",
        "measured_supply_current_a",
        "effective_sensor_current_a",
    ]
    fieldnames.extend(f"adc_voltage_{sensor.cli_name}_v" for sensor in sensors)

    with open(csv_path, "w", newline="", encoding="ascii") as csv_file:
        writer = csv.DictWriter(
            csv_file,
            fieldnames=fieldnames,
        )
        writer.writeheader()
        writer.writerows(
            {
                "loop_count": row.loop_count,
                "commanded_current_a": row.commanded_current_a,
                "measured_supply_current_a": row.measured_supply_current_a,
                "effective_sensor_current_a": row.effective_sensor_current_a,
                **{
                    f"adc_voltage_{sensor.cli_name}_v": row.adc_voltage_by_sensor[sensor.cli_name]
                    for sensor in sensors
                },
            }
            for row in rows
        )

    measured_currents = [row.effective_sensor_current_a for row in rows]

    print("\nCalibration results")
    for sensor in sensors:
        adc_voltages = [row.adc_voltage_by_sensor[sensor.cli_name] for row in rows]
        current_from_adc_slope, current_from_adc_intercept = linear_fit(
            adc_voltages,
            measured_currents,
        )
        adc_from_current_slope, adc_from_current_intercept = linear_fit(
            measured_currents,
            adc_voltages,
        )

        print(f"\n{sensor.cli_name} sensor")
        print(
            "Current from ADC: "
            f"current_a = {current_from_adc_slope:.8f} * adc_voltage_v "
            f"+ {current_from_adc_intercept:.8f}"
        )
        print(
            "ADC from current: "
            f"adc_voltage_v = {adc_from_current_slope:.8f} * current_a "
            f"+ {adc_from_current_intercept:.8f}"
        )
    print(f"Saved raw data to {csv_path}")


def main() -> None:
    args = parse_args()
    supply = PowerSupply()
    bms = chimera_v2.BMS()

    print(f"Power supply: {supply.get_id().strip()}")
    run_calibration(
        supply,
        bms,
        get_sensor_configs(args.sensor),
        args.start_current,
        args.stop_current,
        args.step_current,
        args.max_loops,
    )


if __name__ == "__main__":
    main()
