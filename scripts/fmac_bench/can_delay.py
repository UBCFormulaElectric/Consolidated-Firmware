#!/usr/bin/env python3
"""
Measure the delay the RSM gyro Z FMAC filter adds, from a CANoe export of RSM_GyroZRaw and RSM_GyroZFiltered.

Both signals ride in the same CAN frame (GyroZFilter, 715) and are set from the same IMU sample, so CAN latency
cancels: the gap between the two curves is the filter alone.

  analyze   CANoe CSV export -> delay report (+ optional plot / JSON)
  simulate  Write a synthetic export from the Q1.15 FMAC model, to sanity-check `analyze` without hardware.

Only numpy is required; matplotlib is needed for --plot.
"""

from __future__ import annotations

import argparse
import json
import math
import sys
from pathlib import Path

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent))
from fmac_bench import Q15, fmac_model, quantized_to_float  # noqa: E402

# Gyro Z filter as configured in app_imu.cpp: 2nd-order Butterworth, fc = 10 Hz at fs = 100 Hz, FMAC Q1.15 convention.
DEFAULT_B = [1105, 2210, 1105]
DEFAULT_A = [18727, -6763]
DEFAULT_R = 1
DEFAULT_FS = 100.0
GYRO_FULL_SCALE_DPS = 250.0
CAN_RESOLUTION_DPS = 0.01
REPORT_FREQS = [0.5, 1.0, 2.0, 3.0, 5.0, 8.0]
MODEL_SKIP = 50  # samples for the model's zero initial state to wash out
MIN_COHERENCE = 0.9


# ----------------------------------------------------------------------------------------------------------------------
# Loading
# ----------------------------------------------------------------------------------------------------------------------


def _to_float(cell: str, decimal_comma: bool) -> float:
    cell = cell.strip().strip('"')
    if decimal_comma:
        cell = cell.replace(",", ".")
    return float(cell)


def load_export(path: Path, raw_key: str, filt_key: str):
    """
    CANoe signal export (Graphics -> Export, or logging-file conversion to CSV). Tolerates metadata lines above the
    header, ';' / ',' / tab delimiters, decimal commas, and one shared time column or one time column per signal.
    Returns (t, raw, filtered) for every frame that carries both signals.
    """
    lines = [ln for ln in path.read_text(errors="replace").splitlines() if ln.strip()]
    # ';' and tab win whenever every data line has one: with decimal commas, ',' would otherwise outnumber them.
    tail = lines[-20:]
    delim = next((d for d in ("\t", ";") if all(d in ln for ln in tail)), ",")
    decimal_comma = delim != "," and any("," in ln for ln in lines[-20:])
    rows = [[c.strip().strip('"') for c in ln.split(delim)] for ln in lines]

    def find(row, key):
        return next((i for i, c in enumerate(row) if key.lower() in c.lower()), None)

    hdr = next((i for i, r in enumerate(rows) if find(r, raw_key) is not None and find(r, filt_key) is not None), None)
    if hdr is None:
        sys.exit(f"{path}: no header row naming both '{raw_key}' and '{filt_key}'")
    header = rows[hdr]
    raw_col, filt_col = find(header, raw_key), find(header, filt_key)

    def time_col(sig_col):
        # Closest column to the left that looks like time; a single shared time column is usually the first one.
        for i in range(sig_col - 1, -1, -1):
            if any(k in header[i].lower() for k in ("time", "zeit", "[s]")) or i == 0:
                return i
        return 0

    t_raw_col, t_filt_col = time_col(raw_col), time_col(filt_col)

    def series(tc, vc):
        out = {}
        for r in rows[hdr + 1 :]:
            try:
                out[round(_to_float(r[tc], decimal_comma), 6)] = _to_float(r[vc], decimal_comma)
            except (ValueError, IndexError):
                continue
        return out

    raw_s, filt_s = series(t_raw_col, raw_col), series(t_filt_col, filt_col)
    common = sorted(set(raw_s) & set(filt_s))
    if len(common) < 200:
        sys.exit(f"{path}: only {len(common)} frames carry both signals; need a longer capture")
    t = np.array(common)
    return t, np.array([raw_s[k] for k in common]), np.array([filt_s[k] for k in common])


# ----------------------------------------------------------------------------------------------------------------------
# Analysis
# ----------------------------------------------------------------------------------------------------------------------


def freq_response(b, a, freqs, fs):
    z = np.exp(-2j * np.pi * np.asarray(freqs, float) / fs)
    return np.polyval(b[::-1], z) / np.polyval(a[::-1], z)


def phase_delay_ms(H, freqs):
    return -np.unwrap(np.angle(H)) / (2 * np.pi * np.asarray(freqs)) * 1e3


def group_delay_ms(b, a, f, fs, df=1e-3):
    p0, p1 = np.angle(freq_response(b, a, [f, f + df], fs))
    return -(p1 - p0) / (2 * np.pi * df) * 1e3


def welch(x, y, fs, nper):
    """Averaged cross-spectrum: H = Sxy / Sxx and magnitude-squared coherence."""
    win = np.hanning(nper)
    sxx = syy = 0.0
    sxy = 0.0j
    for s in range(0, len(x) - nper + 1, nper // 2):
        xs, ys = x[s : s + nper], y[s : s + nper]
        X = np.fft.rfft((xs - xs.mean()) * win)
        Y = np.fft.rfft((ys - ys.mean()) * win)
        sxx = sxx + np.abs(X) ** 2
        syy = syy + np.abs(Y) ** 2
        sxy = sxy + np.conj(X) * Y
    f = np.fft.rfftfreq(nper, 1 / fs)
    with np.errstate(divide="ignore", invalid="ignore"):
        return f, sxy / sxx, np.abs(sxy) ** 2 / (sxx * syy), sxx


def xcorr_lag(x, y, max_lag):
    """Lag (samples, sub-sample by parabolic fit) at which y best matches x shifted later."""
    x, y = x - x.mean(), y - y.mean()
    lags = np.arange(-max_lag, max_lag + 1)
    c = np.array([np.dot(x[max(0, -k) : len(x) - max(0, k)], y[max(0, k) : len(y) - max(0, -k)]) for k in lags])
    i = int(np.argmax(c))
    if 0 < i < len(c) - 1:
        den = c[i - 1] - 2 * c[i] + c[i + 1]
        return lags[i] + (0.5 * (c[i - 1] - c[i + 1]) / den if den else 0.0)
    return float(lags[i])


def crossings(t, x, hyst):
    """Interpolated times of zero crossings (with hysteresis), as (time, direction) pairs."""
    out, state = [], 0
    for n in range(1, len(x)):
        if state <= 0 and x[n] > hyst:
            k = n
            while k > 0 and x[k - 1] > 0:
                k -= 1
            if k > 0:
                out.append((t[k - 1] + (t[k] - t[k - 1]) * (-x[k - 1]) / (x[k] - x[k - 1]), +1))
            state = 1
        elif state >= 0 and x[n] < -hyst:
            k = n
            while k > 0 and x[k - 1] < 0:
                k -= 1
            if k > 0:
                out.append((t[k - 1] + (t[k] - t[k - 1]) * (-x[k - 1]) / (x[k] - x[k - 1]), -1))
            state = -1
    return out


def crossing_delays(t, raw, filt):
    """For each raw zero crossing, time until the filtered signal crosses the same way (what you see on the plot)."""
    hyst = 0.1 * np.std(raw)
    rc, fc = crossings(t, raw - raw.mean(), hyst), crossings(t, filt - filt.mean(), hyst)
    delays = []
    for i, (tr, d) in enumerate(rc):
        t_next = rc[i + 1][0] if i + 1 < len(rc) else math.inf
        match = next((tf for tf, df in fc if df == d and tr <= tf < t_next), None)
        if match is not None:
            delays.append(match - tr)
    return np.array(delays)


def analyze(t, raw, filt, bq, aq, r, fs_design):
    dt = np.diff(t)
    fs = 1.0 / float(np.median(dt))
    b, a = quantized_to_float(bq, aq, r)
    out = {
        "frames": len(t),
        "duration_s": float(t[-1] - t[0]),
        "frame_period_ms": float(np.median(dt) * 1e3),
        "frame_period_p99_ms": float(np.percentile(dt, 99) * 1e3),
        "gaps": int(np.sum(dt > 1.5 * np.median(dt))),
        "repeats": int(np.sum((np.diff(raw) == 0) & (np.diff(filt) == 0))),
        "fs": fs,
    }
    if abs(fs - fs_design) > 0.05 * fs_design:
        out["warning"] = f"frames arrive at {fs:.1f} Hz but the filter runs at {fs_design:g} Hz"

    # 1. Is the filtered signal exactly the designed filter applied to the raw one, and at which sample offset?
    xq = np.clip(np.rint(raw / GYRO_FULL_SCALE_DPS * Q15), -32768, 32767)
    model = fmac_model(bq, aq, r, xq) / Q15 * GYRO_FULL_SCALE_DPS
    best = None
    for lag in range(-3, 4):
        m = model[MODEL_SKIP : len(model) - 3]
        f = filt[MODEL_SKIP + lag : MODEL_SKIP + lag + len(m)]
        rms = float(np.sqrt(np.mean((f - m) ** 2)))
        if best is None or rms < best[1]:
            best = (lag, rms, float(np.max(np.abs(f - m))), int(np.sum(np.abs(f - m) > 0.1)))
    out["model_lag_samples"], out["model_rms_dps"], out["model_max_dps"], out["model_outliers"] = best

    # 2. The delay you see between the curves.
    out["xcorr_ms"] = xcorr_lag(raw, filt, max_lag=int(0.2 * fs)) / fs * 1e3
    cd = crossing_delays(t, raw, filt)
    if len(cd):
        out["crossing_n"] = len(cd)
        out["crossing_med_ms"] = float(np.median(cd) * 1e3)
        out["crossing_p10_ms"], out["crossing_p90_ms"] = (float(v * 1e3) for v in np.percentile(cd, [10, 90]))

    # 3. Delay and gain vs frequency, measured against theory.
    # Long segments resolve slow motion (0.1 Hz bins at 100 Hz); at least ~8 of them keep the estimate averaged.
    nper = int(min(1024, 2 ** math.floor(math.log2(max(64, len(t) // 4)))))
    f, H, coh, sxx = welch(raw, filt, fs, nper)
    n_avg = max(1, (len(t) - nper) // (nper // 2) + 1)
    band = (f > 0.2) & (f < fs / 2)
    fpk = float(f[band][np.argmax(sxx[band])])
    Hpk = H[band][np.argmax(sxx[band])]
    out["dominant_hz"] = fpk
    out["dominant_gain"] = float(abs(Hpk))
    out["dominant_delay_ms"] = float(-np.angle(Hpk) / (2 * np.pi * fpk) * 1e3)
    out["dominant_coherence"] = float(coh[band][np.argmax(sxx[band])])
    out["dominant_theory_ms"] = float(phase_delay_ms(freq_response(b, a, [fpk], fs_design), [fpk])[0])
    out["dominant_theory_gain"] = float(abs(freq_response(b, a, [fpk], fs_design)[0]))

    rows = []
    meas_delay = phase_delay_ms(H[1:], f[1:])
    for fr in REPORT_FREQS:
        if fr >= fs / 2:
            continue
        i = int(np.argmin(np.abs(f[1:] - fr)))
        th = freq_response(b, a, [fr], fs_design)[0]
        ok = coh[1:][i] >= MIN_COHERENCE
        rows.append(
            {
                "hz": fr,
                "theory_delay_ms": float(phase_delay_ms(np.array([th]), [fr])[0]),
                "theory_phase_deg": float(-np.degrees(np.angle(th))),
                "theory_gain_db": float(20 * np.log10(abs(th))),
                "meas_hz": float(f[1:][i]),
                "meas_delay_ms": float(meas_delay[i]) if ok else None,
                # 1-sigma phase error of an averaged cross-spectrum (Bendat & Piersol), as a delay.
                "meas_delay_pm_ms": float(
                    math.sqrt((1 - coh[1:][i]) / (2 * coh[1:][i] * n_avg)) / (2 * math.pi * f[1:][i]) * 1e3
                )
                if ok
                else None,
                "meas_gain_db": float(20 * np.log10(abs(H[1:][i]))) if ok else None,
                "coherence": float(coh[1:][i]),
            }
        )
    out["table"] = rows
    out["gd_dc_ms"] = float(group_delay_ms(b, a, 0.0, fs_design))
    curves = {"f": f, "H": H, "coh": coh, "model": model, "crossings": cd}
    return out, curves


# ----------------------------------------------------------------------------------------------------------------------
# Report
# ----------------------------------------------------------------------------------------------------------------------


def fmt(v, spec="{:.1f}"):
    return "-" if v is None or (isinstance(v, float) and math.isnan(v)) else spec.format(v)


def print_report(s, bq, aq, r, fs_design):
    print(f"\nGyro Z FMAC filter delay, from CAN (b={bq} a={aq} R={r}, designed for fs={fs_design:g} Hz)")
    print(f"  {s['frames']} frames over {s['duration_s']:.1f} s, frame period {s['frame_period_ms']:.2f} ms "
          f"(p99 {s['frame_period_p99_ms']:.2f}), {s['gaps']} gap(s), {s['repeats']} repeated frame(s)")
    if "warning" in s:
        print(f"  WARNING: {s['warning']}")

    print("\nIS IT JUST THE FILTER?")
    print(f"  filtered vs Q1.15 FMAC model run on the raw signal: best at {s['model_lag_samples']:+d} sample(s), "
          f"rms {s['model_rms_dps']:.4f} deg/s, max {s['model_max_dps']:.3f} deg/s, "
          f"{s['model_outliers']} sample(s) off by > 0.1 deg/s")
    if s["model_lag_samples"] == 0 and s["model_rms_dps"] < 0.05:
        print("  -> the filtered signal is exactly the designed filter on the same sample: no extra sample of delay")
    else:
        print("  -> NOT a clean match: extra delay or a different filter than the coefficients given")

    print("\nDELAY YOU SEE BETWEEN THE CURVES")
    print(f"  dominant motion {s['dominant_hz']:.2f} Hz: delay {s['dominant_delay_ms']:.1f} ms measured, "
          f"{s['dominant_theory_ms']:.1f} ms theory; gain {s['dominant_gain']:.3f} "
          f"(theory {s['dominant_theory_gain']:.3f}), coherence {s['dominant_coherence']:.3f}")
    print(f"  cross-correlation lag                 {s['xcorr_ms']:.1f} ms")
    if "crossing_med_ms" in s:
        print(f"  zero-crossing delay, raw -> filtered  median {s['crossing_med_ms']:.1f} ms "
              f"(p10 {s['crossing_p10_ms']:.1f}, p90 {s['crossing_p90_ms']:.1f}, n={s['crossing_n']})")
        print(f"  (delays are read off {1e3 / s['fs']:.0f} ms frames, so single crossings have that resolution)")

    print("\nDELAY VS FREQUENCY (phase delay = how far a sine at that frequency trails)")
    print("    freq   theory delay  phase   gain    |  bin      measured delay        gain    coherence")
    for row in s["table"]:
        print(f"  {row['hz']:5.1f} Hz  {row['theory_delay_ms']:8.1f} ms  {row['theory_phase_deg']:5.1f} deg "
              f"{row['theory_gain_db']:+6.2f} dB  |  {row['meas_hz']:4.2f} Hz  {fmt(row['meas_delay_ms']):>6} "
              f"+-{fmt(row['meas_delay_pm_ms']):>4} ms  "
              f"{fmt(row['meas_gain_db'], '{:+.2f}'):>6} dB  {row['coherence']:6.3f}")
    print(f"  group delay at DC (slow motion limit): {s['gd_dc_ms']:.1f} ms")
    print(f"  measured cells are '-' where coherence < {MIN_COHERENCE}: too little motion there to measure")


def plot(t, raw, filt, s, curves, bq, aq, r, fs_design, out, show):
    import matplotlib

    if not show:
        matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    b, a = quantized_to_float(bq, aq, r)
    fig, ax = plt.subplots(2, 2, figsize=(14, 9))
    fs = s["fs"]

    # Time overlay over ~1 s around the largest motion.
    n0 = int(np.argmax(np.abs(raw - raw.mean())))
    w = slice(max(0, n0 - int(0.5 * fs)), min(len(t), n0 + int(0.5 * fs)))
    ax[0, 0].plot(t[w], raw[w], ".-", label="GyroZRaw")
    ax[0, 0].plot(t[w], filt[w], ".-", label="GyroZFiltered")
    ax[0, 0].plot(t[w], curves["model"][w], "--", label="Q1.15 model of the filter")
    ax[0, 0].set(title="Same frames, same axis", xlabel="time [s]", ylabel="deg/s")
    ax[0, 0].legend()

    f, H, coh = curves["f"], curves["H"], curves["coh"]
    ff = np.linspace(0.05, fs_design / 2 * 0.98, 400)
    th = freq_response(b, a, ff, fs_design)
    good = (coh >= MIN_COHERENCE) & (f > 0)
    ax[0, 1].plot(ff, phase_delay_ms(th, ff), "k", lw=2, zorder=3, label="theory: phase delay")
    ax[0, 1].plot(ff, [group_delay_ms(b, a, x, fs_design) for x in ff], "k:", zorder=3, label="theory: group delay")
    ax[0, 1].plot(f[good], phase_delay_ms(H[good], f[good]), "o", ms=3, alpha=0.6, label="measured (coherence >= 0.9)")
    ax[0, 1].set(title="Delay vs frequency", xlabel="Hz", ylabel="ms", xlim=(0, min(15, fs_design / 2)))
    ax[0, 1].legend()

    ax[1, 1].plot(ff, 20 * np.log10(np.abs(th)), "k", lw=2, zorder=3, label="theory")
    ax[1, 1].plot(f[good], 20 * np.log10(np.abs(H[good])), "o", ms=3, alpha=0.6, label="measured")
    ax[1, 1].set(title="Gain", xlabel="Hz", ylabel="dB", xlim=(0, fs_design / 2), ylim=(-30, 3))
    ax[1, 1].legend()

    cd = curves["crossings"] * 1e3
    if len(cd):
        ax[1, 0].hist(cd, bins=np.arange(math.floor(cd.min()) - 1, math.ceil(cd.max()) + 2, 1.0))
    ax[1, 0].set(title="Zero-crossing delay raw -> filtered", xlabel="ms", ylabel="count")

    for x in ax.flat:
        x.grid(alpha=0.3)
    fig.tight_layout()
    if out:
        fig.savefig(out, dpi=110)
        print(f"\nplot written to {out}")
    if show:
        plt.show()


# ----------------------------------------------------------------------------------------------------------------------
# Commands
# ----------------------------------------------------------------------------------------------------------------------


def cmd_analyze(args):
    t, raw, filt = load_export(Path(args.csv), args.raw, args.filtered)
    s, curves = analyze(t, raw, filt, args.b, args.a, args.r, args.fs)
    print_report(s, args.b, args.a, args.r, args.fs)
    if args.json:
        Path(args.json).write_text(json.dumps(s, indent=2))
        print(f"\njson written to {args.json}")
    if args.plot or args.show:
        plot(t, raw, filt, s, curves, args.b, args.a, args.r, args.fs, args.plot, args.show)


def cmd_simulate(args):
    """Hand-wiggle-like motion: 10 s at each REPORT_FREQS rate plus noise, filtered by the Q1.15 model, CAN-quantized."""
    rng = np.random.default_rng(1)
    fs, hold = args.fs, 10.0
    f_inst = np.repeat(REPORT_FREQS, int(fs * hold))
    n = np.arange(len(f_inst))
    t = n / fs
    raw = 40 * np.sin(2 * np.pi * np.cumsum(f_inst) / fs) + rng.normal(0, 0.5, len(n)) + 0.3
    xq = np.clip(np.rint(raw / GYRO_FULL_SCALE_DPS * Q15), -32768, 32767)
    filt = fmac_model(args.b, args.a, args.r, xq) / Q15 * GYRO_FULL_SCALE_DPS
    raw_c = np.round(raw / CAN_RESOLUTION_DPS) * CAN_RESOLUTION_DPS
    filt_c = np.round(filt / CAN_RESOLUTION_DPS) * CAN_RESOLUTION_DPS
    tt = 10.0 + t + rng.normal(0, 2e-4, len(n))
    lines = ["Exported from CANoe (SIMULATED)", "",
             "Time [s];CAN1::hexray::RSM::GyroZFilter::RSM_GyroZRaw [deg/s];"
             "CAN1::hexray::RSM::GyroZFilter::RSM_GyroZFiltered [deg/s]"]
    lines += [f"{a:.6f};{b:.2f};{c:.2f}" for a, b, c in zip(tt, raw_c, filt_c)]
    Path(args.out).write_text("\n".join(lines) + "\n")
    print(f"wrote {args.out}")


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(required=True)

    def filter_args(sp):
        sp.add_argument("--b", type=int, nargs="+", default=DEFAULT_B, help="FMAC B coefficients (Q1.15)")
        sp.add_argument("--a", type=int, nargs="+", default=DEFAULT_A, help="FMAC A coefficients (Q1.15, FMAC sign)")
        sp.add_argument("--r", type=int, default=DEFAULT_R, help="FMAC gain exponent R")
        sp.add_argument("--fs", type=float, default=DEFAULT_FS, help="rate the filter runs at [Hz]")

    a = sub.add_parser("analyze", help="delay report from a CANoe CSV export")
    a.add_argument("csv")
    a.add_argument("--raw", default="GyroZRaw", help="substring of the raw signal's column name")
    a.add_argument("--filtered", default="GyroZFiltered", help="substring of the filtered signal's column name")
    a.add_argument("--plot", help="write the figure to this PNG")
    a.add_argument("--show", action="store_true", help="open the figure")
    a.add_argument("--json", help="write the numbers to this JSON file")
    filter_args(a)
    a.set_defaults(fn=cmd_analyze)

    s = sub.add_parser("simulate", help="write a synthetic CANoe export from the Q1.15 model")
    s.add_argument("--out", default="gyroz_sim.csv")
    filter_args(s)
    s.set_defaults(fn=cmd_simulate)

    args = p.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
