"""
Interactively calibrate the BMS tractive-system current sensors (400 A and 50 A).

For each point you enter the effective current through the sensor; the script reads
both ADC channels, then fits a line through the collected points and writes the raw
data to current_calibration_both.csv.
"""

from __future__ import annotations
import csv
import math
from numpy.polynomial import Polynomial
from dataclasses import dataclass
import chimera_v2


MIN_POINTS = 2
CSV_PATH = "current_calibration_both.csv"
EPSILON = 1e-9

@dataclass(frozen=True)
class SensorConfig:
    name: str
    adc_channel: str
    output_label: str  # C++ constant prefix, e.g. "OUTPUT1"

@dataclass(frozen=True)
class CalibrationPoint:
    effective_sensor_current: float
    adc_voltage_by_sensor: dict[str, float]


SENSORS: list[SensorConfig] = [
    SensorConfig(name="400a", adc_channel="ADC_TS_ISENSE_400A", output_label="OUTPUT2"),
    SensorConfig(name="50a",  adc_channel="ADC_TS_ISENSE_50A",  output_label="OUTPUT1"),
]


def linear_fit(x_vals: list[float], y_vals: list[float]) -> tuple[float, float]:
    fit = Polynomial.fit(x_vals, y_vals, 1)
    coef = tuple(fit.convert().coef)
    if len(coef) != 2:
        raise ValueError(
            "Could not compute a valid linear fit. Input data is likely constant or poorly conditioned."
        )
    return coef


def validate_fit_inputs(x_vals: list[float], sensor_name: str) -> None:
    if len(x_vals) < MIN_POINTS:
        raise ValueError(f"Need at least {MIN_POINTS} points to fit calibration data for {sensor_name}.")

    if math.isclose(max(x_vals), min(x_vals), rel_tol=0.0, abs_tol=EPSILON):
        raise ValueError(
            f"Cannot calibrate {sensor_name}: all ADC readings are identical "
            f"({x_vals[0]:.6f} V). Check ADC sampling before fitting."
        )


def prompt_effective_current(point_index: int) -> float | None:
    while True:
        prompt = f"\nPoint {point_index}: enter effective current through the sensor in A"
        if point_index > MIN_POINTS:
            prompt += " (or 'q' to finish)"
        prompt += ": "
        raw = input(prompt).strip()

        if point_index > MIN_POINTS and raw.lower() in ("q", "quit", "exit"):
            return None

        try:
            return float(raw)
        except ValueError:
            print("Could not parse a number, try again.")


def compute_error_calibration(
    adc_voltages: list[float],
    effective_currents: list[float],
    sensor_name: str,
    output_label: str,
) -> None:
    """Compute and print error-calibration constants for one sensor.

    Derives OUTPUT<N>_{DISCHARGING,CHARGING}_ERROR_{SLOPE,OFFSET} by fitting
    the residual between the raw sensor current estimate and the true current.
    """
    raw_currents = [(v * TSI_TO_CSIN - OFFSET_V) / HIGH_RES_SENS_VA for v in adc_voltages]
    # calibration_target satisfies: -(raw + calibration) == effective_current
    cal_targets = [-ec - rc for ec, rc in zip(effective_currents, raw_currents)]

    discharge_x = [rc for rc in raw_currents if rc > CURRENT_THRESHOLD]
    discharge_y = [ct for rc, ct in zip(raw_currents, cal_targets) if rc > CURRENT_THRESHOLD]
    charge_x    = [rc for rc in raw_currents if rc <= CURRENT_THRESHOLD]
    charge_y    = [ct for rc, ct in zip(raw_currents, cal_targets) if rc <= CURRENT_THRESHOLD]

    print(f"\n  {sensor_name} error calibration constants ({output_label}):")

    for regime, xs, ys in (("DISCHARGING", discharge_x, discharge_y), ("CHARGING", charge_x, charge_y)):
        if len(xs) < MIN_POINTS:
            print(f"    [WARNING] Not enough {regime.lower()} points ({len(xs)}) — skipping fit")
            continue
        intercept, slope = linear_fit(xs, ys)
        print(f"    constexpr float {output_label}_{regime}_ERROR_SLOPE  = {slope:.4f}f;")
        print(f"    constexpr float {output_label}_{regime}_ERROR_OFFSET = {intercept:.4f}f;")


def save_and_print_results(rows: list[CalibrationPoint]) -> None:
    fieldnames = ["effective_sensor_current"]
    fieldnames.extend(f"adc_voltage_{sensor.name}_v" for sensor in SENSORS)

    with open(CSV_PATH, "w", newline="", encoding="ascii") as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(
            {
                "effective_sensor_current": row.effective_sensor_current,
                **{
                    f"adc_voltage_{sensor.name}_v": row.adc_voltage_by_sensor[sensor.name]
                    for sensor in SENSORS
                },
            }
            for row in rows
        )

    measured_currents = [row.effective_sensor_current for row in rows]

    print("\nCalibration results")
    for sensor in SENSORS:
        adc_voltages = [row.adc_voltage_by_sensor[sensor.name] for row in rows]
        validate_fit_inputs(adc_voltages, sensor.name)
        current_from_adc_slope, current_from_adc_intercept = linear_fit(adc_voltages, measured_currents)
        adc_from_current_slope, adc_from_current_intercept = linear_fit(measured_currents, adc_voltages)

        print(f"\n{sensor.name} sensor")
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
        compute_error_calibration(adc_voltages, measured_currents, sensor.name, sensor.output_label)

    print(f"\nSaved raw data to {CSV_PATH}")


def run_calibration(bms: chimera_v2.BMS) -> None:
    rows: list[CalibrationPoint] = []

    print(f"Calibrating sensors: {', '.join(s.name for s in SENSORS)}")
    print(f"Enter at least {MIN_POINTS} points. After {MIN_POINTS} you can quit by typing 'q'.")

    point_index = 1
    while True:
        effective_current = prompt_effective_current(point_index)
        if effective_current is None:
            break

        adc_voltage_by_sensor = {s.name: bms.adc_read(s.adc_channel) for s in SENSORS}
        rows.append(CalibrationPoint(effective_current, adc_voltage_by_sensor))

        line = f"point {point_index:2d}  effective={effective_current:8.3f} A"
        for sensor in SENSORS:
            line += f"  {sensor.name}={adc_voltage_by_sensor[sensor.name]:7.5f} V"
        print(line)

        point_index += 1

    save_and_print_results(rows)


def main() -> None:
    bms = chimera_v2.BMS()
    try:
        run_calibration(bms)
    except ValueError as exc:
        print(f"Calibration failed: {exc}")


if __name__ == "__main__":
    main()
