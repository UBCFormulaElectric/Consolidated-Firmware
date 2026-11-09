#!/usr/bin/env python3
"""
Analyze the VC gyro Z filter bench (firmware/hexray/VC/src/bench/gyro_filter_bench.cpp) from a CANoe capture of
VC_GyroFilterBench.

Every frame carries the filter input and the output of all three filter library filters from the same 50 Hz tick, so
everything here is computed per sample: CAN latency and timestamp jitter never enter a delay number.

  analyze   CSV (from GyroFilterBench.can, or a CANoe signal export) -> spec sheet (+ optional plot / JSON)
  theory    Design numbers for given cutoffs, no hardware needed
  simulate  Write a synthetic capture with the same stimuli and float32 filters, to check `analyze` without hardware

Only numpy is required; matplotlib is needed for --plot.
"""

from __future__ import annotations

import argparse
import json
import math
import sys
from pathlib import Path

import numpy as np

FS = 50.0  # bench tick rate = torque vectoring rate
FILTERS = ["ema", "butterworth", "biquad"]
LABELS = {"ema": "EMA", "butterworth": "Butterworth (1st)", "biquad": "Biquad (2nd)"}
COLUMNS = {
    "counter": "GyroBenchCounter",
    "mode": "GyroBenchMode",
    "cutoff": "GyroBenchCutoff",
    "in": "GyroZIn",
    "ema": "GyroZEma",
    "butterworth": "GyroZButterworth",
    "biquad": "GyroZBiquad",
}
MODES = ["LIVE", "SINE", "CHIRP", "STEP", "IMPULSE", "NOISE"]
CAN_RESOLUTION = 0.01  # deg/s
MODEL_MAX_ERR = 0.02  # deg/s: CAN quantization of input and output is +-0.005 each
REPORT_FREQS = [0.5, 1.0, 2.0, 3.0, 5.0, 8.0, 10.0, 15.0, 20.0]
MIN_COHERENCE = 0.95
MIN_SEGMENT = 50  # samples
LIVE_WINDOW = 1500  # samples (30 s): live captures without a Run column are split into windows this long
STATIONARY_STD = 2.0  # deg/s: a live segment with more input motion than this is treated as "board moving"

# Firmware stimulus constants (gyro_filter_bench.cpp)
STEP_HALF_PERIOD = 100
IMPULSE_PERIOD, IMPULSE_PHASE = 100, 50
CHIRP_F0, CHIRP_F1, CHIRP_SAMPLES = 0.2, 20.0, 2000
NOISE_SEED = 0x2545F491


# ----------------------------------------------------------------------------------------------------------------------
# Filter designs: the exact coefficient formulas in app_sensor_filter.cpp
# ----------------------------------------------------------------------------------------------------------------------


def design(kind: str, fc: float, fs: float = FS):
    if kind == "ema":
        alpha = 1.0 - math.exp(-2.0 * math.pi * fc / fs)
        return np.array([alpha]), np.array([1.0, -(1.0 - alpha)])
    if kind == "butterworth":
        gamma = 1.0 / math.tan(math.pi * fc / fs)
        b0 = 1.0 / (1.0 + gamma)
        return np.array([b0, b0]), np.array([1.0, (1.0 - gamma) * b0])
    if kind == "biquad":
        k = math.tan(math.pi * fc / fs)
        q_inv = 1.41421356237
        d = 1.0 + q_inv * k + k * k
        return np.array([k * k / d, 2 * k * k / d, k * k / d]), np.array([1.0, 2 * (k * k - 1) / d, (1 - q_inv * k + k * k) / d])
    raise ValueError(kind)


def freq_response(b, a, f, fs: float = FS):
    z = np.exp(-2j * np.pi * np.asarray(f, float) / fs)
    return np.polyval(b[::-1], z) / np.polyval(a[::-1], z)


def group_delay(b, a, f, fs: float = FS):
    """Exact group delay in samples: Re(sum k c_k z^-k / sum c_k z^-k) for B minus the same for A."""
    z = np.exp(-2j * np.pi * np.asarray(f, float) / fs)

    def gd(c):
        k = np.arange(len(c))
        return np.real(np.polyval((k * c)[::-1], z) / np.polyval(c[::-1], z))

    return gd(b) - gd(a)


def lfilter(b, a, x, init=None, dtype=np.float64):
    """Direct form I, primed like the firmware: every delayed sample = init (steady state for a DC gain of 1)."""
    b, a = b.astype(dtype), a.astype(dtype)
    x = np.asarray(x, dtype)
    init = x[0] if init is None else dtype(init)
    xs = [init] * (len(b) - 1)
    ys = [init] * (len(a) - 1)
    y = np.empty(len(x), dtype)
    for n, xn in enumerate(x):
        acc = b[0] * xn
        for k in range(1, len(b)):
            acc = acc + b[k] * xs[k - 1]
        for k in range(1, len(a)):
            acc = acc - a[k] * ys[k - 1]
        y[n] = acc
        if xs:
            xs = [xn] + xs[:-1]
        if ys:
            ys = [acc] + ys[:-1]
    return y


def impulse_response(b, a, n=400):
    x = np.zeros(n)
    x[0] = 1.0
    return lfilter(b, a, x, init=0.0)


def settle_samples(b, a, tol=1e-4):
    h = np.abs(impulse_response(b, a, 2000))
    above = np.nonzero(h > tol * h.max())[0]
    return int(above[-1]) + 1 if len(above) else 1


def theory(kind, fc, fs=FS):
    b, a = design(kind, fc, fs)
    f = np.linspace(1e-3, fc, 2000)
    gd = group_delay(b, a, f, fs)
    h = impulse_response(b, a, 2000)
    out = {
        "gd_typ_ms": float(gd[0] / fs * 1e3),
        "gd_max_ms": float(gd.max() / fs * 1e3),
        "gd_max_hz": float(f[np.argmax(gd)]),
        "noise_power_ratio": float(np.sum(h**2)),
    }
    for fr in (1.0, 2.0, 3.0):
        out[f"phase_{fr:g}hz_deg"] = float(np.degrees(np.angle(freq_response(b, a, [fr], fs)[0])))
    for fr in (10.0, 20.0):
        if fr < fs / 2:
            out[f"gain_{fr:g}hz_db"] = float(20 * np.log10(abs(freq_response(b, a, [fr], fs)[0])))
    return out


# ----------------------------------------------------------------------------------------------------------------------
# Loading
# ----------------------------------------------------------------------------------------------------------------------


def _num(cell: str, decimal_comma: bool) -> float:
    cell = cell.strip().strip('"')
    if decimal_comma:
        cell = cell.replace(",", ".")
    return float(cell)


def load(path: Path) -> dict[str, np.ndarray]:
    """
    CSV written by GyroFilterBench.can, or a CANoe signal export (Graphics -> Export / logging file conversion).
    Tolerates metadata above the header, ';' / ',' / tab delimiters, decimal commas, one shared time column or one per
    signal, and value-table names (GYRO_BENCH_SINE) instead of numbers for the mode.
    """
    lines = [ln for ln in path.read_text(errors="replace").splitlines() if ln.strip()]
    tail = lines[-20:]
    delim = next((d for d in ("\t", ";") if all(d in ln for ln in tail)), ",")
    decimal_comma = delim != "," and any("," in ln for ln in tail)
    rows = [[c.strip().strip('"') for c in ln.split(delim)] for ln in lines]

    def find(row, key):
        hits = [i for i, c in enumerate(row) if key.lower() in c.lower()]
        # Prefer the VC_ signal if the Debug_ command was exported too (Debug_GyroBenchMode, Debug_GyroBenchCutoff).
        vc = [i for i in hits if "vc_" in row[i].lower()]
        return (vc or hits or [None])[0]

    hdr = next((i for i, r in enumerate(rows) if all(find(r, k) is not None for k in COLUMNS.values())), None)
    if hdr is None:
        sys.exit(f"{path}: no header row naming all of {', '.join(COLUMNS.values())}")
    header = rows[hdr]
    cols = {k: find(header, v) for k, v in COLUMNS.items()}
    # GyroFilterBench.can numbers each test step, so repeated runs with the same command stay separate.
    run_col = next((i for i, c in enumerate(header) if c.strip().lower() == "run"), None)
    if run_col is not None:
        cols["run"] = run_col

    def time_col(sig_col):
        for i in range(sig_col - 1, -1, -1):
            if any(k in header[i].lower() for k in ("time", "zeit", "[s]")) or i == 0:
                return i
        return 0

    def value(cell):
        try:
            return _num(cell, decimal_comma)
        except ValueError:
            name = cell.upper().replace("GYRO_BENCH_", "")
            if name in MODES:
                return float(MODES.index(name))
            raise

    series = {}
    for k, c in cols.items():
        tc, s = time_col(c), {}
        for r in rows[hdr + 1 :]:
            try:
                s[round(_num(r[tc], decimal_comma), 6)] = value(r[c])
            except (ValueError, IndexError):
                continue
        series[k] = s
    common = sorted(set.intersection(*(set(s) for s in series.values())))
    if len(common) < MIN_SEGMENT:
        sys.exit(f"{path}: only {len(common)} complete VC_GyroFilterBench frames")
    out = {"t": np.array(common)}
    for k, s in series.items():
        out[k] = np.array([s[t] for t in common])
    return out


def segments(d):
    """
    Split at every mode/cutoff change, every dropped frame (counter gap) and every Run change. Without a Run column,
    live stretches are cut into LIVE_WINDOW pieces so repeated stationary runs still give a spread.
    Returns (start, stop) index pairs.
    """
    change = (
        (np.diff(d["mode"]) != 0)
        | (np.abs(np.diff(d["cutoff"])) > 1e-6)
        | ((np.diff(d["counter"]) % 256) != 1)
    )
    if "run" in d:
        change |= np.diff(d["run"]) != 0
    edges = [0, *(np.nonzero(change)[0] + 1), len(d["t"])]
    out = []
    for s, e in zip(edges[:-1], edges[1:]):
        if "run" not in d and d["mode"][s] == 0 and e - s >= 2 * LIVE_WINDOW:
            cuts = list(range(s, e - LIVE_WINDOW + 1, LIVE_WINDOW))
            out += [(c, c + LIVE_WINDOW) for c in cuts[:-1]] + [(cuts[-1], e)]
        elif e - s >= MIN_SEGMENT:
            out.append((s, e))
    return out


# ----------------------------------------------------------------------------------------------------------------------
# Measurements
# ----------------------------------------------------------------------------------------------------------------------


def model_check(x, y, b, a):
    """Does the board output equal the designed filter run on the board input, at zero sample offset?"""
    skip = min(settle_samples(b, a), len(x) // 2)
    # A flat stretch (e.g. between impulses) fits every lag equally, so it says nothing about the offset.
    m = lfilter(b, a, x)
    if len(x) - skip < 20 or np.std(m[skip:]) < 2 * CAN_RESOLUTION:
        return None
    best = None
    for lag in (0, -1, 1, -2, 2):  # ties go to 0
        lo, hi = skip + max(0, lag), len(x) - max(0, -lag)
        err = y[lo:hi] - m[lo - lag : hi - lag]
        r = (lag, float(np.sqrt(np.mean(err**2))), float(np.max(np.abs(err))))
        if best is None or r[1] < 0.9 * best[1]:
            best = r
    return {"lag": best[0], "rms": best[1], "max": best[2]}


def welch(x, y, nper):
    win = np.hanning(nper)
    sxx = syy = 0.0
    sxy = 0.0j
    n_avg = 0
    for s in range(0, len(x) - nper + 1, nper // 2):
        xs, ys = x[s : s + nper], y[s : s + nper]
        X = np.fft.rfft((xs - xs.mean()) * win)
        Y = np.fft.rfft((ys - ys.mean()) * win)
        sxx, syy, sxy = sxx + np.abs(X) ** 2, syy + np.abs(Y) ** 2, sxy + np.conj(X) * Y
        n_avg += 1
    f = np.fft.rfftfreq(nper, 1 / FS)
    with np.errstate(divide="ignore", invalid="ignore"):
        return f, sxy / sxx, np.abs(sxy) ** 2 / (sxx * syy), n_avg


def sine_fit(x, y):
    """Frequency from the input's spectrum, then least-squares amplitude/phase of both at that frequency."""
    n = np.arange(len(x))
    spec = np.abs(np.fft.rfft((x - x.mean()) * np.hanning(len(x)), 16 * len(x)))
    f = float(np.fft.rfftfreq(16 * len(x), 1 / FS)[np.argmax(spec)])
    basis = np.column_stack([np.cos(2 * np.pi * f / FS * n), np.sin(2 * np.pi * f / FS * n), np.ones(len(n))])

    def phasor(v):
        c, s, _ = np.linalg.lstsq(basis, v, rcond=None)[0]
        return c - 1j * s

    return f, phasor(y) / phasor(x)


def averaged_events(x, y, kind, length):
    """Align y on every step edge / impulse in x and average, normalized by the step / impulse size."""
    amp = np.max(np.abs(x))
    if amp == 0:
        return None
    if kind == "step":
        idx = np.nonzero(np.abs(np.diff(x)) > 0.5 * amp)[0] + 1
    else:
        idx = np.nonzero((np.abs(x) > 0.5 * amp) & (np.abs(np.roll(x, 1)) < 0.01 * amp))[0]
    out = []
    for i in idx:
        if i < 1 or i + length > len(x):
            continue
        size = x[i] - x[i - 1] if kind == "step" else x[i]
        out.append((y[i : i + length] - y[i - 1]) / size)
    return np.mean(out, axis=0) if out else None


def step_metrics(r):
    def cross(level):
        i = int(np.argmax(r >= level))
        return i - 1 + (level - r[i - 1]) / (r[i] - r[i - 1]) if i > 0 else 0.0

    outside = np.nonzero(np.abs(r - 1.0) > 0.02)[0]
    return {
        "t50_ms": cross(0.5) / FS * 1e3,
        "rise_ms": (cross(0.9) - cross(0.1)) / FS * 1e3,
        "overshoot_pct": float(max(0.0, r.max() - 1.0) * 100),
        "settle_ms": float((outside[-1] + 1 if len(outside) else 0) / FS * 1e3),
    }


def impulse_metrics(h, fc):
    n = np.arange(len(h))
    nfft = 8192
    H, nH = np.fft.rfft(h, nfft), np.fft.rfft(n * h, nfft)
    f = np.fft.rfftfreq(nfft, 1 / FS)
    with np.errstate(divide="ignore", invalid="ignore"):  # the biquad has a zero at Nyquist
        gd = np.real(nH / H)
    band = (f > 0) & (f <= fc)
    return {
        "gd_typ_ms": float(np.sum(n * h) / np.sum(h) / FS * 1e3),
        "gd_max_ms": float(gd[band].max() / FS * 1e3),
        "gd_max_hz": float(f[band][np.argmax(gd[band])]),
        "dc_gain": float(np.sum(h)),
        "noise_power_ratio": float(np.sum(h**2)),
    }


def analyze_segment(d, s, e):
    mode = MODES[int(d["mode"][s])] if 0 <= int(d["mode"][s]) < len(MODES) else str(int(d["mode"][s]))
    fc = float(d["cutoff"][s])
    x = d["in"][s:e]
    seg = {"mode": mode, "cutoff_hz": fc, "samples": e - s, "t0_s": float(d["t"][s]), "filters": {}}
    if "run" in d:
        seg["run"] = int(d["run"][s])
    for k in FILTERS:
        y = d[k][s:e]
        b, a = design(k, fc)
        r = {"model": model_check(x, y, b, a)}
        skip = min(settle_samples(b, a), (e - s) // 2)
        if mode == "SINE":
            f, H = sine_fit(x[skip:], y[skip:])
            th = freq_response(b, a, [f])[0]
            r["sine"] = {
                "hz": f,
                "gain_db": float(20 * np.log10(abs(H))),
                "phase_deg": float(np.degrees(np.angle(H))),
                "delay_ms": float(-np.angle(H) / (2 * np.pi * f) * 1e3),
                "theory_gain_db": float(20 * np.log10(abs(th))),
                "theory_phase_deg": float(np.degrees(np.angle(th))),
                "theory_delay_ms": float(-np.angle(th) / (2 * np.pi * f) * 1e3),
            }
        elif mode in ("CHIRP", "NOISE", "LIVE"):
            nper = int(min(512, 2 ** math.floor(math.log2(max(64, (e - s - skip) // 4)))))
            rows = []
            # Too short for even one window (e.g. cut up by dropped frames): no spectrum, power ratio still works.
            f, H, coh = (welch(x[skip:], y[skip:], nper)[:3]) if e - s - skip >= nper else (np.zeros(0),) * 3
            if len(f):
                r["spectrum"] = {"f": f, "H": H, "coh": coh}
            for fr in REPORT_FREQS if len(f) else []:
                i = int(np.argmin(np.abs(f - fr)))
                if f[i] <= 0 or coh[i] < MIN_COHERENCE:
                    continue
                th = freq_response(b, a, [f[i]])[0]
                rows.append(
                    {
                        "hz": float(f[i]),
                        "gain_db": float(20 * np.log10(abs(H[i]))),
                        "phase_deg": float(np.degrees(np.angle(H[i]))),
                        "theory_gain_db": float(20 * np.log10(abs(th))),
                        "theory_phase_deg": float(np.degrees(np.angle(th))),
                        "coherence": float(coh[i]),
                    }
                )
            r["table"] = rows
            # Linear trend removed, so a slow bias drift doesn't count as noise.
            idx = np.arange(e - s - skip)
            xd = x[skip:] - np.polyval(np.polyfit(idx, x[skip:], 1), idx)
            yd = y[skip:] - np.polyval(np.polyfit(idx, y[skip:], 1), idx)
            r["power_ratio"] = float(np.var(yd) / np.var(xd)) if np.var(xd) > 0 else None
            r["input_std_dps"] = float(np.std(xd))
            if mode == "LIVE":
                seg["stationary"] = r["input_std_dps"] < STATIONARY_STD
        elif mode == "STEP":
            resp = averaged_events(x, y, "step", 90)
            if resp is not None:
                th = lfilter(b, a, np.ones(90), init=0.0)
                r["step"] = {"resp": resp, "theory": th, **step_metrics(resp), "theory_metrics": step_metrics(th)}
        elif mode == "IMPULSE":
            h = averaged_events(x, y, "impulse", IMPULSE_PERIOD)
            if h is not None:
                r["impulse"] = {"h": h, **impulse_metrics(h, fc), "theory": theory(k, fc)}
        seg["filters"][k] = r
    return seg


def analyze(d):
    # Within a run only: GyroFilterBench.can doesn't log the frames sent while it switches between steps.
    same = np.diff(d["run"]) == 0 if "run" in d else np.ones(len(d["t"]) - 1, bool)
    dt = np.diff(d["t"])[same]
    lost = int(np.sum(((np.diff(d["counter"]) % 256) - 1)[same]))
    summary = {
        "frames": len(d["t"]),
        "duration_s": float(d["t"][-1] - d["t"][0]),
        "frame_period_ms": float(np.median(dt) * 1e3),
        "frame_period_p99_ms": float(np.percentile(dt, 99) * 1e3),
        "lost_frames": lost,
    }
    return summary, [analyze_segment(d, s, e) for s, e in segments(d)]


# ----------------------------------------------------------------------------------------------------------------------
# Report
# ----------------------------------------------------------------------------------------------------------------------


def fmt(v, spec="{:.1f}"):
    return "-" if v is None else spec.format(v)


def print_report(summary, segs):
    print("\nVC gyro Z filter bench (sensor filter library at fs = 50 Hz)")
    print(f"  {summary['frames']} frames over {summary['duration_s']:.1f} s, frame period "
          f"{summary['frame_period_ms']:.2f} ms (p99 {summary['frame_period_p99_ms']:.2f}), "
          f"{summary['lost_frames']} lost frame(s)")

    print("\nIS THE BOARD OUTPUT EXACTLY THE DESIGNED FILTER? (worst segment per filter)")
    for k in FILTERS:
        checks = [s["filters"][k]["model"] for s in segs if s["filters"][k]["model"]]
        if not checks:
            continue
        worst = max(checks, key=lambda c: c["max"])
        lags = sorted({c["lag"] for c in checks})
        ok = worst["max"] < MODEL_MAX_ERR and lags == [0]
        print(f"  {LABELS[k]:18s} max err {worst['max']:.4f} deg/s, rms {worst['rms']:.4f}, sample offset(s) {lags}"
              f"  -> {'OK' if ok else 'MISMATCH'}")
    print(f"  (CAN carries 0.01 deg/s steps, so anything under {MODEL_MAX_ERR} deg/s is an exact match)")

    for s in segs:
        what = s["mode"] + {True: " (stationary)", False: " (board moving)"}.get(s.get("stationary"), "")
        run = f"run {s['run']}, " if "run" in s else ""
        print(f"\n--- {what} at fc = {s['cutoff_hz']:g} Hz, {run}{s['samples']} samples ({s['samples'] / FS:.1f} s) "
              f"from t = {s['t0_s']:.1f} s")
        for k in FILTERS:
            r = s["filters"][k]
            if "impulse" in r:
                m, th = r["impulse"], r["impulse"]["theory"]
                print(f"  {LABELS[k]:18s} group delay typ {m['gd_typ_ms']:5.1f} ms (theory {th['gd_typ_ms']:5.1f}), "
                      f"max {m['gd_max_ms']:5.1f} ms at {m['gd_max_hz']:.1f} Hz (theory {th['gd_max_ms']:5.1f}), "
                      f"DC gain {m['dc_gain']:.3f}, noise power x{m['noise_power_ratio']:.3f} "
                      f"(theory x{th['noise_power_ratio']:.3f})")
            if "step" in r:
                m, th = r["step"], r["step"]["theory_metrics"]
                print(f"  {LABELS[k]:18s} 50% delay {m['t50_ms']:5.1f} ms (theory {th['t50_ms']:5.1f}), "
                      f"rise {m['rise_ms']:5.1f} ms ({th['rise_ms']:5.1f}), overshoot {m['overshoot_pct']:4.1f}% "
                      f"({th['overshoot_pct']:4.1f}), 2% settle {m['settle_ms']:5.0f} ms ({th['settle_ms']:5.0f})")
            if "sine" in r:
                m = r["sine"]
                print(f"  {LABELS[k]:18s} {m['hz']:5.2f} Hz: gain {m['gain_db']:+6.2f} dB ({m['theory_gain_db']:+6.2f}), "
                      f"phase {m['phase_deg']:6.1f} deg ({m['theory_phase_deg']:6.1f}), "
                      f"delay {m['delay_ms']:5.1f} ms ({m['theory_delay_ms']:5.1f})")
            if "table" in r:
                ratio = r["power_ratio"]
                if s["mode"] == "NOISE" or s.get("stationary"):
                    extra = f", input std {r['input_std_dps']:.3f} deg/s" if s["mode"] == "LIVE" else ""
                    print(f"  {LABELS[k]:18s} output/input power {fmt(ratio, '{:.3f}')} (noise power reduction "
                          f"{fmt(None if ratio is None else (1 - ratio) * 100, '{:.1f}')}%){extra}")
                else:
                    print(f"  {LABELS[k]:18s} gain / phase where the input has enough motion (coherence >= "
                          f"{MIN_COHERENCE}):")
                for row in r["table"]:
                    print(f"      {row['hz']:5.2f} Hz  gain {row['gain_db']:+6.2f} dB ({row['theory_gain_db']:+6.2f})  "
                          f"phase {row['phase_deg']:6.1f} deg ({row['theory_phase_deg']:6.1f})  "
                          f"coherence {row['coherence']:.3f}")
            if s["mode"] == "NOISE" and "table" in r:
                th = theory(k, s["cutoff_hz"])["noise_power_ratio"]
                print(f"      white-noise theory: output/input power {th:.3f}")

    live = [s for s in segs if s.get("stationary")]
    if live:
        print("\nSTATIONARY LIVE SEGMENTS (this is the IMU noise power reduction)")
        for k in FILTERS:
            red = [(1 - s["filters"][k]["power_ratio"]) * 100 for s in live if s["filters"][k].get("power_ratio")]
            if red:
                print(f"  {LABELS[k]:18s} {np.mean(red):5.1f}% mean, {np.std(red):4.1f}% sd over {len(red)} segment(s)")
    print("\n(theory in brackets: app_sensor_filter.cpp coefficient formulas at the same cutoff and fs)")


def plot(d, segs, out, show):
    import matplotlib

    if not show:
        matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    colors = {"ema": "tab:green", "butterworth": "tab:orange", "biquad": "tab:blue"}
    fig, ax = plt.subplots(2, 3, figsize=(18, 10))
    fcs = sorted({s["cutoff_hz"] for s in segs})
    fc_main = 10.0 if 10.0 in fcs else (fcs[0] if fcs else 10.0)
    main = [s for s in segs if s["cutoff_hz"] == fc_main]

    # Time overlay: the first step (or anything) at the main cutoff.
    pick = next((s for s in main if s["mode"] == "STEP"), main[0] if main else None)
    if pick:
        i0 = int(np.searchsorted(d["t"], pick["t0_s"]))
        w = slice(i0 + 60, min(i0 + 260, len(d["t"])))
        n = np.arange(w.stop - w.start) / FS * 1e3
        ax[0, 0].plot(n, d["in"][w], "k.-", lw=1, label="input")
        for k in FILTERS:
            ax[0, 0].plot(n, d[k][w], ".-", color=colors[k], label=LABELS[k])
        ax[0, 0].set(title=f"Board output, {pick['mode']} at fc = {fc_main:g} Hz", xlabel="ms", ylabel="deg/s")
        ax[0, 0].legend()

    # Gain and phase vs frequency: theory lines, measured points from sines / chirp / noise.
    ff = np.linspace(0.05, FS / 2 * 0.99, 500)
    for k in FILTERS:
        b, a = design(k, fc_main)
        th = freq_response(b, a, ff)
        ax[0, 1].plot(ff, 20 * np.log10(np.abs(th)), color=colors[k], lw=2, label=f"{LABELS[k]} theory")
        ax[0, 2].plot(ff, -np.degrees(np.unwrap(np.angle(th))), color=colors[k], lw=2, label=f"{LABELS[k]} theory")
        for s in main:
            r = s["filters"][k]
            if "spectrum" in r and (s["mode"] in ("CHIRP", "NOISE") or s.get("stationary") is False):
                sp = r["spectrum"]
                good = (sp["coh"] >= MIN_COHERENCE) & (sp["f"] > 0)
                ax[0, 1].plot(sp["f"][good], 20 * np.log10(np.abs(sp["H"][good])), ".", ms=3, color=colors[k], alpha=0.4)
                ax[0, 2].plot(sp["f"][good], -np.degrees(np.angle(sp["H"][good])), ".", ms=3, color=colors[k], alpha=0.4)
            if "sine" in r:
                ax[0, 1].plot(r["sine"]["hz"], r["sine"]["gain_db"], "o", color=colors[k], mec="k")
                ax[0, 2].plot(r["sine"]["hz"], -r["sine"]["phase_deg"], "o", color=colors[k], mec="k")
    ax[0, 1].set(title=f"Gain at fc = {fc_main:g} Hz (lines theory, dots board)", xlabel="Hz", ylabel="dB", ylim=(-40, 3))
    ax[0, 2].set(title="Phase lag (lines theory, dots board)", xlabel="Hz", ylabel="deg", xlim=(0, 10), ylim=(0, 120))
    ax[0, 1].legend(fontsize=8)

    # Averaged step responses.
    for s in main:
        for k in FILTERS:
            if "step" in s["filters"][k]:
                st = s["filters"][k]["step"]
                t = np.arange(len(st["resp"])) / FS * 1e3
                ax[1, 0].plot(t, st["theory"], color=colors[k], lw=2)
                ax[1, 0].plot(t, st["resp"], ".", color=colors[k], label=LABELS[k])
    ax[1, 0].set(title="Step response (lines theory, dots board)", xlabel="ms after the step", ylabel="output / step",
                 xlim=(0, 600))
    ax[1, 0].legend()

    # Group delay vs cutoff, measured from the impulse runs.
    for k in FILTERS:
        pts = sorted((s["cutoff_hz"], s["filters"][k]["impulse"]) for s in segs if "impulse" in s["filters"][k])
        if not pts:
            continue
        fc_ = np.array([p[0] for p in pts])
        ax[1, 1].plot(fc_, [p[1]["gd_typ_ms"] for p in pts], "o", color=colors[k], label=f"{LABELS[k]} typ")
        ax[1, 1].plot(fc_, [p[1]["gd_max_ms"] for p in pts], "^", color=colors[k], label=f"{LABELS[k]} max")
        grid = np.linspace(min(fc_.min(), 2.0), max(fc_.max(), 10.0), 60)
        ax[1, 1].plot(grid, [theory(k, g)["gd_typ_ms"] for g in grid], "-", color=colors[k], lw=1)
        ax[1, 1].plot(grid, [theory(k, g)["gd_max_ms"] for g in grid], "--", color=colors[k], lw=1)
    ax[1, 1].set(title="Group delay vs cutoff (lines theory, markers board)", xlabel="cutoff [Hz]", ylabel="ms")
    ax[1, 1].legend(fontsize=8)

    # Noise power reduction on live (stationary) segments.
    live = [s for s in segs if s.get("stationary")]
    if live:
        for j, k in enumerate(FILTERS):
            red = [(1 - s["filters"][k]["power_ratio"]) * 100 for s in live if s["filters"][k].get("power_ratio")]
            ax[1, 2].bar(j, np.mean(red), yerr=np.std(red), color=colors[k], capsize=6)
        ax[1, 2].set_xticks(range(3), [LABELS[k] for k in FILTERS])
    ax[1, 2].set(title="Live gyro Z noise power reduction (mean +- sd)", ylabel="%")

    for x in ax.flat:
        x.grid(alpha=0.3)
    fig.tight_layout()
    if out:
        fig.savefig(out, dpi=110)
        print(f"\nplot written to {out}")
    if show:
        plt.show()


def to_json(summary, segs):
    def clean(o):
        if isinstance(o, dict):
            return {k: clean(v) for k, v in o.items() if k not in ("spectrum", "resp", "theory", "h")}
        if isinstance(o, list):
            return [clean(v) for v in o]
        return o

    segs_out = []
    for s in segs:
        s2 = clean(s)
        for k in FILTERS:
            r = s["filters"][k]
            if "impulse" in r:
                s2["filters"][k]["impulse"]["theory"] = r["impulse"]["theory"]
            if "step" in r:
                s2["filters"][k]["step"]["theory_metrics"] = r["step"]["theory_metrics"]
        segs_out.append(s2)
    return {"summary": summary, "segments": segs_out}


# ----------------------------------------------------------------------------------------------------------------------
# Simulation: the firmware's stimuli and filters in float32, CAN-quantized
# ----------------------------------------------------------------------------------------------------------------------


def sim_stimulus(mode, n, freq, amp, rng, twist, live_k):
    f32 = np.float32
    if mode == "SINE":
        x = np.empty(n, f32)
        phase = f32(0)
        for i in range(n):
            x[i] = f32(amp) * np.sin(f32(2 * np.pi) * phase, dtype=f32)
            phase = f32(phase + f32(freq) / f32(FS))
            phase = f32(phase - np.floor(phase))
        return x
    if mode == "CHIRP":
        dur = CHIRP_SAMPLES / FS
        t = (np.arange(n) % CHIRP_SAMPLES) / FS
        cyc = CHIRP_F0 * dur * (np.exp(np.log(CHIRP_F1 / CHIRP_F0) * t / dur) - 1) / np.log(CHIRP_F1 / CHIRP_F0)
        return (amp * np.sin(2 * np.pi * (cyc - np.floor(cyc)))).astype(f32)
    if mode == "STEP":
        return np.where((np.arange(n) // STEP_HALF_PERIOD) % 2 == 1, amp, 0.0).astype(f32)
    if mode == "IMPULSE":
        return np.where(np.arange(n) % IMPULSE_PERIOD == IMPULSE_PHASE, amp, 0.0).astype(f32)
    if mode == "NOISE":
        s, x = NOISE_SEED, np.empty(n, f32)
        for i in range(n):
            s ^= (s << 13) & 0xFFFFFFFF
            s ^= s >> 17
            s ^= (s << 5) & 0xFFFFFFFF
            x[i] = f32(amp) * f32((s - (1 << 32) if s & 0x80000000 else s) / 2147483648.0)
        return x
    # LIVE: stationary IMU after its 20 Hz DLPF (bias + white noise + some aliased vibration), plus any hand motion.
    k = np.arange(live_k, live_k + n)
    x = 0.3 + rng.normal(0, 0.4, n) + 0.5 * np.sin(2 * np.pi * 23.0 / FS * k)
    if twist:
        x += 60 * np.sin(2 * np.pi * 1.5 / FS * k)
    return x.astype(f32)


def cmd_simulate(args):
    """Same test steps as GyroFilterBench.can. Like the firmware, the filters only restart when the command changes."""
    rng = np.random.default_rng(1)
    # (mode, frequency, amplitude, cutoff, seconds, board twisted by hand)
    plan = [("LIVE", 0, 0, 10, 30, False)] * 5 + [("LIVE", 0, 0, 10, 30, True)]
    plan += [("IMPULSE", 0, 100, fc, 20, False) for fc in (2, 3, 5, 8, 10)]
    plan += [("STEP", 0, 100, 10, 20, False), ("CHIRP", 0, 100, 10, 80, False), ("NOISE", 0, 100, 10, 60, False)]
    plan += [("SINE", f, 100, 10, 10, False) for f in (1, 2, 3, 5, 8)]
    gap = int(0.5 * FS)  # the board keeps sending while CANoe switches; those frames aren't logged (run 0)

    # Group consecutive steps with the same command: one continuous filter run each, like the firmware.
    blocks = []
    for run, (mode, freq, amp, fc, hold, twist) in enumerate(plan, start=1):
        cmd = (mode, freq, amp, fc)
        if not blocks or blocks[-1]["cmd"] != cmd:
            blocks.append({"cmd": cmd, "parts": []})
        blocks[-1]["parts"] += [(0, gap, twist), (run, int(hold * FS), twist)]

    q = lambda v: np.clip(np.round((v + 250) / CAN_RESOLUTION) * CAN_RESOLUTION - 250, -250, 250)
    lines = ["Time [s];Run;VC_GyroBenchCounter;VC_GyroBenchMode;VC_GyroBenchCutoff [Hz];VC_GyroZIn [deg/s];"
             "VC_GyroZEma [deg/s];VC_GyroZButterworth [deg/s];VC_GyroZBiquad [deg/s]"]
    n_total, live_k = 0, 0
    for blk in blocks:
        mode, freq, amp, fc = blk["cmd"]
        n = sum(p[1] for p in blk["parts"])
        if mode == "LIVE":
            x = np.concatenate([sim_stimulus(mode, m, freq, amp, rng, tw, live_k + sum(p[1] for p in blk["parts"][:i]))
                                for i, (_, m, tw) in enumerate(blk["parts"])])
            live_k += n
        else:
            x = sim_stimulus(mode, n, freq, amp, rng, False, 0)
        init = float(x[0]) if mode == "LIVE" else 0.0
        ys = [lfilter(*design(k, fc), x, init=init, dtype=np.float32) for k in FILTERS]
        runs = np.concatenate([np.full(m, r) for r, m, _ in blk["parts"]])
        for i in range(n):
            counter = (n_total + i) % 256
            if runs[i] == 0 or (args.drop and rng.random() < args.drop):
                continue
            vals = ";".join(f"{q(float(v[i])):.2f}" for v in (x, *ys))
            t = 5.0 + (n_total + i) / FS + rng.normal(0, 2e-4)
            lines.append(f"{t:.6f};{runs[i]};{counter};{MODES.index(mode)};{fc:.1f};{vals}")
        n_total += n
    Path(args.out).write_text("\n".join(lines) + "\n")
    print(f"wrote {args.out}")


# ----------------------------------------------------------------------------------------------------------------------
# Commands
# ----------------------------------------------------------------------------------------------------------------------


def cmd_analyze(args):
    d = load(Path(args.csv))
    summary, segs = analyze(d)
    print_report(summary, segs)
    if args.json:
        np_scalar = lambda o: o.item() if isinstance(o, np.generic) else str(o)
        Path(args.json).write_text(json.dumps(to_json(summary, segs), indent=2, default=np_scalar))
        print(f"\njson written to {args.json}")
    if args.plot or args.show:
        plot(d, segs, args.plot, args.show)


def cmd_theory(args):
    print(f"\nDesign numbers at fs = {args.fs:g} Hz (from the app_sensor_filter.cpp coefficient formulas)")
    print("  cutoff  filter              typ GD   max GD (at)        phase @1/2/3 Hz          gain @10/20 Hz     "
          "white-noise power")
    for fc in args.fc:
        for k in FILTERS:
            t = theory(k, fc, args.fs)
            print(f"  {fc:5.1f}   {LABELS[k]:18s} {t['gd_typ_ms']:5.1f} ms {t['gd_max_ms']:5.1f} ms ({t['gd_max_hz']:4.1f} Hz)  "
                  f"{t['phase_1hz_deg']:6.1f} {t['phase_2hz_deg']:6.1f} {t['phase_3hz_deg']:6.1f} deg  "
                  f"{t.get('gain_10hz_db', float('nan')):6.1f} {t.get('gain_20hz_db', float('nan')):6.1f} dB    "
                  f"x{t['noise_power_ratio']:.3f} ({(1 - t['noise_power_ratio']) * 100:.1f}% less)")


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(required=True)

    a = sub.add_parser("analyze", help="spec sheet from a VC_GyroFilterBench capture")
    a.add_argument("csv")
    a.add_argument("--plot", help="write the figure to this PNG")
    a.add_argument("--show", action="store_true", help="open the figure")
    a.add_argument("--json", help="write the numbers to this JSON file")
    a.set_defaults(fn=cmd_analyze)

    t = sub.add_parser("theory", help="design numbers, no hardware")
    t.add_argument("--fc", type=float, nargs="+", default=[2, 3, 5, 8, 10])
    t.add_argument("--fs", type=float, default=FS)
    t.set_defaults(fn=cmd_theory)

    s = sub.add_parser("simulate", help="write a synthetic capture")
    s.add_argument("--out", default="gyro_filter_bench_sim.csv")
    s.add_argument("--drop", type=float, default=0.0, help="fraction of frames to drop, to exercise segmenting")
    s.set_defaults(fn=cmd_simulate)

    args = p.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
