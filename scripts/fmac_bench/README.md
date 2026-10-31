# FMAC characterization bench

Measures `hw::fmac::FmacIir` (`firmware/shared/srcpp/hw/hw_fmac.cpp`) on the RSM (STM32H562) and turns the
results into a spec sheet: per-call latency, sample delay, frequency response, group delay, step response,
noise power reduction, fixed-point error, limit cycles and clipping.

Every test is run twice:

| path  | what it is                                                                                   |
|-------|----------------------------------------------------------------------------------------------|
| `drv` | your driver, called exactly like application code would (`init()` then `process(float)`)     |
| `raw` | the same FMAC config driven straight through registers with zeroed history: the hardware's best case |

If `raw` looks right and `drv` doesn't, the problem is in the driver, not the peripheral.

## Running it

```sh
# 1. (optional) pick the filter under test. Default: 2nd-order Butterworth, fc = 50 Hz, fs = 1 kHz
python scripts/fmac_bench/fmac_bench.py design --fs 1000 --fc 50 --order 2
#    or any b/a:  ... design --fs 1000 --b 0.2 0.2 --a 1 -0.6

# 2. build RSM with the bench and flash it the way you normally do
cmake -S . -B build_fw_dev -DRSM_FMAC_BENCH=ON
cmake --build build_fw_dev --target hexray_RSM_app.elf

# 3. capture RTT channel 2 (channel 0 = normal logs, 1 = SystemView). Stop once the file ends with "#FMAC_BENCH done".
"C:/Program Files/SEGGER/JLink_V870/JLinkRTTLogger.exe" -Device STM32H562RI -If SWD -Speed 4000 -RTTChannel 2 fmac_bench.log

# 4. analyze
python scripts/fmac_bench/fmac_bench.py analyze fmac_bench.log --plot fmac_bench.png --json fmac_bench.json

# 5. turn the bench back off so normal RSM builds are unaffected
cmake -S . -B build_fw_dev -DRSM_FMAC_BENCH=OFF
```

The bench runs once, 2 s after boot, in a low-priority task. It waits for the host to drain RTT, so the logger
can attach late without losing anything. Reset the board to run it again. Use
`python scripts/fmac_bench/fmac_bench.py simulate` to get a synthetic log and see what a bug-free result looks like.

## Tests

| test       | stimulus                                         | gives you                                                         |
|------------|--------------------------------------------------|-------------------------------------------------------------------|
| `passthru` | y = x filter, hand-picked values incl. ±1.0, ±1.5 | sample alignment (lag), float to Q1.15 conversion edge cases      |
| `align_*`  | (raw only) passthrough with different preloads   | how the FMAC aligns outputs vs. how much history is preloaded     |
| `impulse`  | 0.9 FS impulse, 512 samples                      | frequency response, group delay, sum(h²), limit-cycle tail        |
| `step`     | 0.5 FS step                                      | rise / 50 % delay / overshoot / settling, steady-state error      |
| `step_fs`  | 0.99 FS step                                     | clipping behaviour and FMAC_SR overflow/saturation flags          |
| `noise`    | uniform white noise, ±0.5 FS, 4096 samples       | measured noise power ratio, error vs. ideal (rms / max / bias), SNR, **timing** |

## What the specs mean

- **latency [cycles / µs]**: wall time of one `process()` call (float in to float out), from the DWT cycle counter.
  Median is the honest number. Max includes preemption by higher-priority tasks/ISRs.
- **pure delay [samples]**: first sample at which the output reacts. 0 for any IIR with b0 ≠ 0. 1 means the driver
  is returning the previous sample.
- **group delay [samples or ms]**: how far the filter lags slowly varying signals. This is the delay that matters
  for control loops. It comes from the coefficients, not the hardware. Multiply by 1/fs for time.
- **noise power reduction**: sum(h²) = output variance / input variance for white input noise. Reported in dB and
  cross-checked against the measured variance ratio (the "ideal" column for that row uses the same noise realization).
- **error vs ideal / bias / limit cycle**: what Q1.15 arithmetic costs you. The FMAC truncates, so expect a small
  negative bias and a few LSB of deadband in the tail of the impulse response.

## Delay on the car signal (gyro Z over CAN)

`can_delay.py` measures the lag between `RSM_GyroZRaw` and `RSM_GyroZFiltered` (message 715) from a CANoe export.
Both signals sit in the same frame and come from the same IMU sample, so CAN latency cancels and the gap is the filter.

```powershell
# 1. RSM running the gyro Z FMAC integration, CANoe measuring. Twist the board about Z, holding each rate ~10 s:
#    very slow, ~1 Hz, ~2 Hz, ~3 Hz, ~5 Hz, as fast as you can. Stay under +-250 deg/s (gyro full scale).
# 2. Export both signals from the Graphics window to CSV (whole measurement).
# 3. Analyze (defaults are the app_imu.cpp filter: b=[1105 2210 1105] a=[18727 -6763] R=1, fs=100 Hz)
python scripts/fmac_bench/can_delay.py analyze gyroz.csv --plot gyroz.png
```

It reports whether the filtered signal is exactly the Q1.15 filter applied to the raw one at zero sample offset,
the delay you see between the curves (dominant frequency, cross-correlation, zero crossings), and delay and gain
against theory at 0.5-8 Hz. `simulate` writes a synthetic export to check the analysis without hardware.
