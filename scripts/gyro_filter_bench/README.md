# Gyro Z filter bench (VC, board in the loop)

Characterizes the sensor filter library (`firmware/shared/srcpp/app/app_sensor_filter.cpp`) on the VC, at the torque
vectoring rate (50 Hz), on the real IMU1 gyro Z and on exact synthetic stimuli. The results are a spec sheet
for each filter: typical and max group delay, phase lag in the yaw band, gain, step response, noise power reduction.
The bench also checks that the board computes exactly the designed filter.

```
 CANoe (GyroFilterBench.can)                    VC (gyro_filter_bench.cpp, every 20 ms)
 ---------------------------                    ----------------------------------------
 Debug_GyroFilterBenchCmd (1008)  ----------->  input = live IMU1 gyro Z | sine | chirp | step | impulse | noise
   mode, frequency, amplitude, cutoff           EMA, 1st-order Butterworth, biquad: same input, same cutoff
                                  <-----------  VC_GyroFilterBench (512): counter, mode, cutoff,
 checks + gyro_filter_bench.csv                   input, EMA out, Butterworth out, biquad out
```

The input travels in the same frame as the outputs, so the analysis works per sample. CAN latency and timestamp
jitter never enter a delay number, and the analysis doesn't need to know what the stimulus was.

## Running it

```powershell
# 1. Build the VC with the bench task and flash it the way you normally do
cmake -S . -B build_fw_dev -DVC_GYRO_FILTER_BENCH=ON
cmake --build build_fw_dev --target hexray_VC_app.elf

# 2. CANoe: load can_bus/dbcs/hexray.dbc (the build regenerates it) on the FDCAN channel, add
#    scripts/gyro_filter_bench/canoe/GyroFilterBench.can as a test module, start the measurement, run the test module.
#    Keep the board still except in TC2 (twist it about Z by hand when the test report says so). About 8 minutes.
#    Output: gyro_filter_bench.csv in the configuration folder, plus pass/fail in the test report.

# 3. Analyze
python scripts/gyro_filter_bench/gyro_filter_bench.py analyze gyro_filter_bench.csv --plot gfb.png --json gfb.json

# 4. Turn the bench back off so normal VC builds are unaffected
cmake -S . -B build_fw_dev -DVC_GYRO_FILTER_BENCH=OFF
```

Without the test module, the bench filters live gyro Z at 10 Hz and sends `VC_GyroFilterBench` all the time. You can
drive it by hand by sending `Debug_GyroFilterBenchCmd` from an IG block, and export the seven `VC_GyroFilterBench`
signals from the Graphics window to CSV. `analyze` reads that too; live stretches are cut into 30 s windows since
there is no Run column. The board also logs each filter's `process()` time (mean / max CPU cycles) once a second
over RTT.

`python scripts/gyro_filter_bench/gyro_filter_bench.py simulate` writes a synthetic capture (same stimuli, float32
filters, CAN-quantized) to check the analysis without hardware. `theory --fc 2 3 5 8 10` prints the design numbers.

## Test steps (GyroFilterBench.can)

| test | stimulus                                   | gives you                                                                 |
|------|--------------------------------------------|---------------------------------------------------------------------------|
| TC1  | live gyro Z, board still, 5 x 30 s         | **IMU noise power reduction**, mean and spread over runs                   |
| TC2  | live gyro Z, board twisted by hand, 30 s   | gain / phase on real motion                                               |
| TC3  | impulse, fc = 2, 3, 5, 8, 10 Hz            | **typical (DC) and max group delay** at each cutoff, DC gain, sum(h²)      |
| TC4  | 2 s square wave                            | 50 % delay, rise, overshoot, 2 % settling; DC gain checked live in CANoe  |
| TC5  | exponential chirp 0.2-20 Hz, 2 sweeps      | gain and phase vs frequency                                               |
| TC6  | uniform white noise                        | output/input noise power vs the white-noise theory                        |
| TC7  | sines at 1, 2, 3, 5, 8 Hz                  | gain and **phase lag in the yaw band**                                    |

CANoe checks every step on the spot: a frame every 20 ms, no counter gaps, the board echoes the commanded mode and
cutoff, and on the step every filter settles to the input within 0.05 deg/s.

## What the numbers mean

- **Is it exactly the designed filter**: the analysis reruns the `app_sensor_filter.cpp` coefficient formulas on the
  logged input and compares that with the logged output at sample offsets -2 to +2. Pass means offset 0 and an error
  under 0.02 deg/s (the CAN signal steps in 0.01 deg/s). If this fails, the firmware is not running the filter you
  think it is, or it adds a sample of delay.
- **Group delay, typical**: the centroid of the measured impulse response (sum n·h / sum h). That is exactly the
  group delay at DC, i.e. how far the filter lags slow yaw motion.
- **Group delay, max**: the peak of the group delay computed from the measured impulse response, up to the cutoff. For
  the EMA and the 1st-order Butterworth this is the DC value; the biquad peaks at about 0.65-0.9 fc.
- **Phase lag at 1-3 Hz**: what the yaw controller actually loses in phase margin.
- **Noise power reduction (TC1)**: 1 - var(output) / var(input) on the still board, with the linear trend removed.
  This is measured on the real IMU noise as it comes out of the IAM-20680HP's own 20 Hz DLPF at 100 Hz, sampled at
  50 Hz, so it is the number to quote, not the white-noise theory.

Every delay here is the filter's alone. Torque vectoring also sees up to one 20 ms tick of sample-and-hold, and
whatever age the IMU sample has when TV reads it.
