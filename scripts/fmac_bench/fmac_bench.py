#!/usr/bin/env python3
"""
Characterization bench helper for hw::fmac::FmacIir (firmware/shared/srcpp/hw/hw_fmac.cpp).

  design    Generate firmware/hexray/RSM/src/bench/fmac_bench_coeffs.hpp (Butterworth low-pass or custom b/a).
  analyze   Turn the RTT channel-2 log streamed by the RSM bench into a spec sheet (+ optional plots / JSON).
  simulate  Write a synthetic log from a Q1.15 model of the FMAC, to sanity-check `analyze` without hardware.

Only numpy is required; matplotlib is needed for --plot.
"""

from __future__ import annotations

import argparse
import json
import math
import re
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path

import numpy as np

Q15 = 32768.0
REPO_ROOT = Path(__file__).resolve().parents[2]
HEADER_PATH = REPO_ROOT / "firmware/hexray/RSM/src/bench/fmac_bench_coeffs.hpp"

# Mirrors enum class ErrorCode in firmware/shared/srcpp/util/util_errorCodes.hpp.
ERROR_CODES = [
    "INVALID_ARGS",
    "OUT_OF_RANGE",
    "TIMEOUT",
    "ERROR",
    "BUSY",
    "UNIMPLEMENTED",
    "RETRY_FAILED",
    "CHECKSUM_FAIL",
    "INVALID_READING",
    "ERROR_INDETERMINATE",
    "POLL_INVALID",
]
# FMAC_SR bits (RM0481).
SR_FLAGS = {0: "YEMPTY", 1: "X1FULL", 8: "OVFL", 9: "UNFL", 10: "SAT"}


# ----------------------------------------------------------------------------------------------------------------------
# Filter math (numpy only)
# ----------------------------------------------------------------------------------------------------------------------


def butterworth_lowpass(order: int, fc: float, fs: float) -> tuple[np.ndarray, np.ndarray]:
    """Digital Butterworth low-pass via the bilinear transform with pre-warping (same result as scipy.signal.butter)."""
    if not 0 < fc < fs / 2:
        raise SystemExit(f"cutoff {fc} Hz must be between 0 and fs/2 = {fs / 2} Hz")
    k = np.arange(1, order + 1)
    analog_poles = np.exp(1j * np.pi * (2 * k + order - 1) / (2 * order)) * 2 * fs * np.tan(np.pi * fc / fs)
    a = np.real(np.poly((2 * fs + analog_poles) / (2 * fs - analog_poles)))
    b = np.real(np.poly(-np.ones(order)))
    b *= np.sum(a) / np.sum(b)  # unity DC gain
    return b, a


def lfilter(b, a, x) -> np.ndarray:
    """Direct-form IIR in float64: a[0] y[n] = sum b[k] x[n-k] - sum_{j>=1} a[j] y[n-j]."""
    b = np.asarray(b, float) / a[0]
    a = np.asarray(a, float) / a[0]
    y = np.zeros(len(x))
    for n in range(len(x)):
        acc = 0.0
        for k in range(min(len(b), n + 1)):
            acc += b[k] * x[n - k]
        for j in range(1, min(len(a), n + 1)):
            acc -= a[j] * y[n - j]
        y[n] = acc
    return y


def fmac_model(bq, aq, r: int, xq) -> np.ndarray:
    """
    Q1.15 model of the FMAC IIR: y[n] = sat(floor(2^R * (sum bq x + sum aq y) / 2^15)).
    The real accumulator is 26 bits wide, so expect up to ~1 LSB disagreement from intermediate truncation.
    """
    bq, aq = [int(v) for v in bq], [int(v) for v in aq]
    xh, yh, out = [0] * len(bq), [0] * len(aq), []
    for x in xq:
        xh = [int(x)] + xh[:-1]
        acc = sum(b * v for b, v in zip(bq, xh)) + sum(a * v for a, v in zip(aq, yh))
        y = max(-32768, min(32767, (acc << r) >> 15))
        yh = [y] + yh[:-1]
        out.append(y)
    return np.array(out, float)


def quantized_to_float(bq, aq, r: int) -> tuple[np.ndarray, np.ndarray]:
    """FMAC coefficients back to a standard (b, a) pair, a[0] = 1."""
    scale = 2.0**r / Q15
    return np.asarray(bq, float) * scale, np.concatenate(([1.0], -np.asarray(aq, float) * scale))


def analytic_response(b, a, freqs, fs):
    """H(e^jw) and group delay (samples) evaluated directly from the coefficients."""
    w = 2 * np.pi * np.asarray(freqs) / fs
    nb, na = np.arange(len(b)), np.arange(len(a))
    eb, ea = np.exp(-1j * np.outer(w, nb)), np.exp(-1j * np.outer(w, na))
    B, A = eb @ b, ea @ a
    with np.errstate(divide="ignore", invalid="ignore"):
        gd = np.real((eb @ (nb * b)) / B) - np.real((ea @ (na * a)) / A)
    return B / A, gd


def impulse_response(h, freqs, fs):
    """H(e^jw) and group delay (samples) from a measured impulse response."""
    w = 2 * np.pi * np.asarray(freqs) / fs
    n = np.arange(len(h))
    e = np.exp(-1j * np.outer(w, n))
    H = e @ h
    with np.errstate(divide="ignore", invalid="ignore"):
        gd = np.real((e @ (n * h)) / H)
    return H, gd


# ----------------------------------------------------------------------------------------------------------------------
# design
# ----------------------------------------------------------------------------------------------------------------------


def quantize_for_fmac(b, a):
    """Pick the smallest gain exponent R so every coefficient fits Q1.15, and convert to the FMAC sign convention."""
    b = np.asarray(b, float) / a[0]
    a = np.asarray(a, float) / a[0]
    if len(b) < 2:
        b = np.append(b, 0.0)  # FMAC IIR needs P >= 2
    feedback = -a[1:] if len(a) > 1 else np.zeros(1)  # and Q >= 1
    peak = max(np.max(np.abs(b)), np.max(np.abs(feedback)))
    r = next((r for r in range(8) if peak / 2**r <= 32767 / Q15), None)
    if r is None:
        raise SystemExit("coefficients are too large for the FMAC even with R = 7")
    bq = np.clip(np.round(b / 2**r * Q15), -32768, 32767).astype(int)
    aq = np.clip(np.round(feedback / 2**r * Q15), -32768, 32767).astype(int)
    return b, a, bq, aq, r


def cmd_design(args):
    if args.b is not None:
        if args.a is None:
            raise SystemExit("--b needs --a (use --a 1 for an FIR)")
        b, a = np.array(args.b, float), np.array(args.a, float)
        desc = f"custom b={list(args.b)} a={list(args.a)} fs={args.fs:g}Hz"
    else:
        b, a = butterworth_lowpass(args.order, args.fc, args.fs)
        desc = f"butterworth order={args.order} fc={args.fc:g}Hz fs={args.fs:g}Hz"
    b, a, bq, aq, r = quantize_for_fmac(b, a)

    # Report what Q1.15 quantization alone does to the filter, before any hardware is involved.
    bqf, aqf = quantized_to_float(bq, aq, r)
    poles = np.roots(aqf)
    freqs = np.linspace(0, args.fs / 2, 4001)
    H, _ = analytic_response(b, a, freqs, args.fs)
    Hq, _ = analytic_response(bqf, aqf, freqs, args.fs)
    print(desc)
    print(f"  R = {r} (coefficients scaled by 2^-{r}), P = {len(bq)}, Q = {len(aq)}")
    print(f"  B_Q15 = {bq.tolist()}   A_Q15 = {aq.tolist()}   (A is -a[1..] / 2^R)")
    print(f"  DC gain    ideal {db(abs(H[0])):+.4f} dB   quantized {db(abs(Hq[0])):+.4f} dB")
    print(f"  -3 dB      ideal {cutoff(freqs, H):.3f} Hz   quantized {cutoff(freqs, Hq):.3f} Hz")
    print(f"  max |pole| quantized {np.max(np.abs(poles)):.6f}" + ("  <-- UNSTABLE" if np.max(np.abs(poles)) >= 1 else ""))
    small = [v for v in np.concatenate((bq, aq)) if v != 0 and abs(v) < 64]
    if small:
        print(f"  warning: coefficients {small} have < 7 bits of resolution; expect a visible gain/cutoff error")

    out = Path(args.out)
    out.write_text(render_header(desc, args.fs, r, bq, aq, b, a, " ".join(sys.argv[1:])))
    print(f"wrote {out}")


def render_header(desc, fs, r, bq, aq, b, a, cmdline) -> str:
    def ints(v):
        return ", ".join(str(int(x)) for x in v)

    def floats(v):
        literals = (f"{np.float32(x):.9g}" for x in v)
        return ", ".join(s + ("f" if any(c in s for c in ".e") else ".0f") for s in literals)

    return f"""#pragma once

/**
 * @file fmac_bench_coeffs.hpp
 * @brief Filter under test for the RSM FMAC bench. Generated, do not edit by hand:
 *   python scripts/fmac_bench/fmac_bench.py {cmdline}
 */

#include <array>
#include <cstdint>

namespace bench::fmac::coeffs
{{
inline constexpr const char *DESCRIPTION         = "{desc}";
inline constexpr uint32_t    SAMPLE_RATE_MILLIHZ = {int(round(fs * 1000))}U; // only used to label the analysis
inline constexpr uint8_t     GAIN_EXPONENT       = {r}U;

// FMAC IIR convention: y[n] = 2^R * (sum b_q[k] x[n-k] + sum a_q[j] y[n-j]), so b_q = b / 2^R and a_q = -a[1..] / 2^R.
inline constexpr std::array<int16_t, {len(bq)}> B_Q15 = {{ {{ {ints(bq)} }} }};
inline constexpr std::array<int16_t, {len(aq)}> A_Q15 = {{ {{ {ints(aq)} }} }};

// Unquantized design (a[0] = 1), forwarded to the analysis script as the ideal reference.
inline constexpr std::array<float, {len(b)}> B_FLOAT = {{ {{ {floats(b)} }} }};
inline constexpr std::array<float, {len(a)}> A_FLOAT = {{ {{ {floats(a)} }} }};
}} // namespace bench::fmac::coeffs
"""


# ----------------------------------------------------------------------------------------------------------------------
# Log parsing
# ----------------------------------------------------------------------------------------------------------------------


@dataclass
class Run:
    path: str
    test: str
    meta: dict
    pre: list[int] = field(default_factory=list)
    x: np.ndarray = field(default_factory=lambda: np.zeros(0))
    y: np.ndarray = field(default_factory=lambda: np.zeros(0))  # NaN where the FMAC produced no output
    cyc: np.ndarray = field(default_factory=lambda: np.zeros(0))
    complete: bool = True

    @property
    def failure(self) -> str | None:
        err = self.meta.get("err", -1)
        if err < 0:
            return None
        name = ERROR_CODES[err] if err < len(ERROR_CODES) else f"code {err}"
        return f"{name} at sample {self.meta.get('n', '?')}"


def f32(hex_bits: str) -> float:
    return struct.unpack("<f", struct.pack("<I", int(hex_bits, 16)))[0]


def parse_kv(tokens):
    out = {}
    for t in tokens:
        if "=" in t:
            k, v = t.split("=", 1)
            out[k] = int(v, 0)
    return out


def finish_run(cur) -> Run:
    rows = cur.pop("rows")
    cols = np.array([[np.nan if v == "-" else float(v) for v in row] for row in rows]).reshape(-1, 3)
    return Run(x=cols[:, 0], y=cols[:, 1], cyc=cols[:, 2], **cur)


def parse_log(text: str):
    meta, runs, cur = {}, {}, None
    for line in text.splitlines():
        tok = line.split()
        if not tok:
            continue
        head = tok[0]
        if head == "meta":
            meta.update(parse_kv(tok[1:]))
        elif head == "desc":
            meta["desc"] = line.split(" ", 1)[1].strip()
        elif head in ("coef_b_q15", "coef_a_q15"):
            meta[head] = [int(v) for v in tok[1:]]
        elif head in ("coef_b_f32", "coef_a_f32"):
            meta[head] = [f32(v) for v in tok[1:]]
        elif head == "begin":
            if cur is not None:
                runs[(cur["path"], cur["test"])] = finish_run(cur | {"complete": False})
            cur = {"path": tok[1], "test": tok[2], "meta": parse_kv(tok[3:]), "pre": [], "rows": []}
        elif head == "pre" and cur is not None:
            cur["pre"] = [int(v) for v in tok[1:]]
        elif head == "end" and cur is not None:
            runs[(cur["path"], cur["test"])] = finish_run(cur)
            cur = None
        elif cur is not None and len(tok) == 3 and re.fullmatch(r"-?\d+", tok[0]):
            cur["rows"].append(tok)
    if cur is not None:
        runs[(cur["path"], cur["test"])] = finish_run(cur | {"complete": False})
    for key in ("sysclk_hz", "fs_millihz", "R", "coef_b_q15", "coef_a_q15", "coef_b_f32", "coef_a_f32"):
        if key not in meta:
            raise SystemExit(f"log is missing '{key}' - is this the RTT channel 2 output of the RSM FMAC bench?")
    if "#FMAC_BENCH done" not in text:
        print("warning: log has no '#FMAC_BENCH done' marker; it may be truncated", file=sys.stderr)
    return meta, runs


# ----------------------------------------------------------------------------------------------------------------------
# analyze
# ----------------------------------------------------------------------------------------------------------------------


def db(x):
    with np.errstate(divide="ignore"):
        return 20 * np.log10(x)


def cutoff(freqs, H) -> float:
    """First frequency where |H| drops 3.01 dB below its DC value (linear interpolation)."""
    mag = np.abs(H) / abs(H[0])
    idx = np.nonzero(mag < 1 / math.sqrt(2))[0]
    if len(idx) == 0 or idx[0] == 0:
        return float("nan")
    i = idx[0]
    m0, m1 = mag[i - 1], mag[i]
    return float(freqs[i - 1] + (m0 - 1 / math.sqrt(2)) / (m0 - m1) * (freqs[i] - freqs[i - 1]))


def interp_at(freqs, values, f):
    return float(np.interp(f, freqs, values))


def crossing(y, level):
    """Fractional index where y first crosses `level` (rising or falling towards it)."""
    above = (y >= level) if level >= 0 else (y <= level)
    idx = np.nonzero(above)[0]
    if len(idx) == 0:
        return float("nan")
    i = idx[0]
    if i == 0:
        return 0.0
    return float(i - 1 + (level - y[i - 1]) / (y[i] - y[i - 1]))


def best_lag(x_expected, y, max_lag=3):
    """Sample offset that makes y match x_expected, and the mismatching indices at that offset."""
    best = None
    for lag in range(max_lag + 1):
        ref = np.concatenate((np.full(lag, 0.0), x_expected[: len(x_expected) - lag]))
        bad = [i for i in range(len(y)) if np.isnan(y[i]) or abs(y[i] - ref[i]) > 1]
        if best is None or len(bad) < len(best[1]):
            best = (lag, bad, ref)
    return best


def analyze(meta, runs):
    fs = meta["fs_millihz"] / 1000.0
    sysclk = meta["sysclk_hz"]
    r = meta["R"]
    bq, aq = meta["coef_b_q15"], meta["coef_a_q15"]
    b, a = np.array(meta["coef_b_f32"], float), np.array(meta["coef_a_f32"], float)
    bqf, aqf = quantized_to_float(bq, aq, r)

    freqs = np.linspace(0, fs / 2, 2001)
    cols = ["ideal", "quantized", "raw", "drv"]
    spec = {c: {} for c in cols}
    curves = {"freqs": freqs}
    notes = []

    # --- Coefficient-only references ----------------------------------------------------------------------------------
    for col, (bb, aa) in (("ideal", (b, a)), ("quantized", (bqf, aqf))):
        H, gd = analytic_response(bb, aa, freqs, fs)
        curves[col] = {"H": H, "gd": gd}
        h = lfilter(bb, aa, np.r_[1.0, np.zeros(4095)])
        step = lfilter(bb, aa, np.ones(512))
        spec[col].update(freq_specs(freqs, H, gd, fs))
        spec[col].update(step_specs(step, 1.0, abs(H[0])))
        spec[col]["first_response"] = 0 if abs(bb[0]) > 0 else int(np.argmax(np.abs(h) > 0))
        spec[col]["npr_theory"] = float(np.sum(h**2))
        spec[col]["npr_db"] = 10 * math.log10(spec[col]["npr_theory"])
        spec[col]["headroom"] = 1.0 / np.max(np.abs(step))
        curves[col]["impulse"] = h[:512]
        curves[col]["step"] = step

    if np.sum(curves["ideal"]["impulse"][-64:] ** 2) > 1e-6 * np.sum(curves["ideal"]["impulse"] ** 2):
        notes.append("impulse response does not decay within 512 samples; measured frequency response is truncated")

    # --- Measured paths ----------------------------------------------------------------------------------------------
    for path in ("raw", "drv"):
        s = spec[path]
        curves[path] = {}
        failures = {t: run.failure for (p, t), run in runs.items() if p == path and run.failure}
        s["failures"] = failures

        if run := runs.get((path, "noise")):
            ok = run.cyc[~np.isnan(run.cyc)][10:]
            if len(ok):
                s["cyc_min"], s["cyc_med"] = float(np.min(ok)), float(np.median(ok))
                s["cyc_p99"], s["cyc_max"] = float(np.percentile(ok, 99)), float(np.max(ok))
                s["rate_max_khz"] = sysclk / s["cyc_med"] / 1e3
                curves[path]["cycles"] = ok
            if path == "drv":
                s["init_cyc"] = run.meta.get("init_cyc", float("nan"))

        if run := runs.get((path, "passthru")):
            expected = np.clip(run.x, -32768, 32767)
            lag, bad, ref = best_lag(expected, run.y)
            s["passthru_lag"] = lag
            s["passthru_bad"] = [(i, int(run.x[i]), None if np.isnan(run.y[i]) else int(run.y[i]), int(ref[i])) for i in bad]

        if (run := runs.get((path, "impulse"))) and len(run.y) > 1 and not np.isnan(run.y[: min(len(run.y), 64)]).any():
            amp = run.x[0]
            y = np.nan_to_num(run.y)
            h = y / amp
            # Only use the samples where the linear response is still above 1/2 LSB. Past that the output is just
            # quantization deadband / limit cycle (reported as tail_lsb), which would otherwise dominate H(0).
            above = np.nonzero(np.abs(curves["quantized"]["impulse"]) * abs(amp) >= 0.5)[0]
            n_lin = min(len(h), int(above[-1]) + 8 if len(above) else len(h))
            H, gd = impulse_response(h[:n_lin], freqs, fs)
            curves[path].update(H=H, gd=gd, impulse=h)
            s.update(freq_specs(freqs, H, gd, fs))
            nz = np.nonzero(np.abs(y) >= 1)[0]
            s["first_response"] = int(nz[0]) if len(nz) else float("nan")
            s["npr_theory"] = float(np.sum(h[:n_lin] ** 2))
            s["npr_db"] = 10 * math.log10(s["npr_theory"])
            if len(y) >= 256:
                s["tail_lsb"] = float(np.max(np.abs(y[-128:])))
            model = fmac_model(bq, aq, r, run.x)
            n = len(y)
            s["model_err_impulse"] = float(np.max(np.abs(y - model[:n])))
            if path == "raw" and s["model_err_impulse"] > 4:
                notes.append(convention_hint(bq, aq, r, run.x, y))

        if (run := runs.get((path, "step"))) and len(run.y) >= 64:
            amp = run.x[0]
            y = np.nan_to_num(run.y)
            curves[path]["step"] = y / amp
            s.update(step_specs(y / amp, 1.0, abs(curves["quantized"]["H"][0])))
            ideal_final = amp * np.sum(bqf) / np.sum(aqf)
            s["ss_err_lsb"] = float(np.mean(y[-64:]) - ideal_final)

        if (run := runs.get((path, "step_fs"))) and len(run.y):
            s["fs_clipped"] = int(np.sum(np.abs(np.nan_to_num(run.y)) >= 32767))
            s["fs_sr"] = run.meta.get("sr", 0)

        if (run := runs.get((path, "noise"))) and len(run.y) > 256 and not np.isnan(run.y).any():
            skip = 100
            x, y = run.x, run.y
            ideal = lfilter(b, a, x / Q15) * Q15
            model = fmac_model(bq, aq, r, x)
            e = (y - ideal)[skip:]
            s["npr_meas"] = float(np.var(y[skip:]) / np.var(x[skip:]))
            # Ideal filter on this exact input realization, so the "measured" row compares like with like.
            spec["ideal"]["npr_meas"] = float(np.var(ideal[skip:]) / np.var(x[skip:]))
            s["err_rms_lsb"] = float(np.sqrt(np.mean(e**2)))
            s["err_max_lsb"] = float(np.max(np.abs(e)))
            s["err_mean_lsb"] = float(np.mean(e))
            s["snr_db"] = 10 * math.log10(np.var(ideal[skip:]) / np.mean(e**2))
            s["model_err_noise"] = float(np.max(np.abs(y - model)))
            curves[path]["noise_err"] = e

        align = {}
        for (p, t), run in runs.items():
            if p == path and t.startswith("align_"):
                lag, bad, _ = best_lag(np.clip(run.x, -32768, 32767), run.y)
                align[t[len("align_") :]] = {"pre": run.pre, "lag": lag, "bad": len(bad)}
        if align:
            s["align"] = align

    return {"fs": fs, "sysclk": sysclk, "desc": meta.get("desc", ""), "P": len(bq), "Q": len(aq), "R": r}, spec, curves, notes


def freq_specs(freqs, H, gd, fs):
    mag = np.abs(H)
    fc = cutoff(freqs, H)
    out = {
        "dc_gain_db": float(db(mag[0])),
        "f3db": fc,
        "gd_dc": float(gd[0]),
        "gd_fc": interp_at(freqs, gd, fc) if not math.isnan(fc) else float("nan"),
    }
    band = freqs <= (fc if not math.isnan(fc) else fs / 2)
    i = int(np.nanargmax(np.where(band, gd, -np.inf)))
    out["gd_peak"], out["gd_peak_f"] = float(gd[i]), float(freqs[i])
    out["att_fs4"] = float(db(interp_at(freqs, mag, fs / 4)))
    out["att_045"] = float(db(interp_at(freqs, mag, 0.45 * fs)))
    return out


def step_specs(step, amp, dc_gain):
    final = amp * dc_gain
    y = step
    t10, t50, t90 = crossing(y, 0.1 * final), crossing(y, 0.5 * final), crossing(y, 0.9 * final)
    outside = np.nonzero(np.abs(y - final) > 0.02 * abs(final))[0]
    return {
        "rise": t90 - t10,
        "t50": t50,
        "overshoot": float((np.max(y) - final) / final * 100),
        "settle": float(outside[-1] + 1) if len(outside) else 0.0,
    }


def convention_hint(bq, aq, r, x, y) -> str:
    """Raw FMAC output disagrees with the model: check whether a different coefficient convention explains it."""
    variants = {
        "as configured": (bq, aq),
        "A sign flipped": (bq, [-v for v in aq]),
        "B reversed": (bq[::-1], aq),
        "A reversed": (bq, aq[::-1]),
        "B and A reversed": (bq[::-1], aq[::-1]),
    }
    errs = {k: float(np.max(np.abs(y - fmac_model(bb, aa, r, x)[: len(y)]))) for k, (bb, aa) in variants.items()}
    best = min(errs, key=errs.get)
    listing = ", ".join(f"{k}: {v:.0f}" for k, v in errs.items())
    return f"raw FMAC does not match the Q15 model (max error LSB -> {listing}); best match is '{best}'"


# ----------------------------------------------------------------------------------------------------------------------
# Report
# ----------------------------------------------------------------------------------------------------------------------


def fmt(v, spec_fmt="{:.3f}"):
    if v is None or (isinstance(v, float) and math.isnan(v)):
        return "-"
    return spec_fmt.format(v)


def print_report(info, spec, notes):
    fs, sysclk = info["fs"], info["sysclk"]
    ms = 1000.0 / fs
    print(f"\nFMAC characterization: {info['desc']}")
    print(f"  P={info['P']} Q={info['Q']} R={info['R']}  fs={fs:g} Hz  sysclk={sysclk / 1e6:g} MHz")
    print("  columns: ideal = float design, quantized = Q1.15 coefficients in float math,")
    print("           raw = FMAC hardware via registers, drv = hw::fmac::FmacIir\n")

    cols = ["ideal", "quantized", "raw", "drv"]
    rows = [
        ("TIMING (per process() call)", None, None),
        ("latency min [cycles]", "cyc_min", "{:.0f}"),
        ("latency median [cycles]", "cyc_med", "{:.0f}"),
        ("latency p99 [cycles]", "cyc_p99", "{:.0f}"),
        ("latency max [cycles]", "cyc_max", "{:.0f}"),
        ("latency median [us]", lambda s: s.get("cyc_med", math.nan) / sysclk * 1e6, "{:.3f}"),
        ("max sample rate @ median [kHz]", "rate_max_khz", "{:.0f}"),
        ("init() [cycles]", "init_cyc", "{:.0f}"),
        ("SAMPLE DELAY", None, None),
        ("pure delay: first output response [samples]", "first_response", "{:.0f}"),
        ("passthrough lag [samples]", "passthru_lag", "{:.0f}"),
        ("FREQUENCY DOMAIN", None, None),
        ("DC gain [dB]", "dc_gain_db", "{:+.4f}"),
        ("-3 dB cutoff [Hz]", "f3db", "{:.3f}"),
        ("group delay @ DC [samples]", "gd_dc", "{:.3f}"),
        ("group delay @ DC [ms]", lambda s: s.get("gd_dc", math.nan) * ms, "{:.3f}"),
        ("group delay @ -3 dB [samples]", "gd_fc", "{:.3f}"),
        ("peak passband group delay [samples]", "gd_peak", "{:.3f}"),
        ("  ...at [Hz]", "gd_peak_f", "{:.2f}"),
        ("gain @ fs/4 [dB]", "att_fs4", "{:.2f}"),
        ("gain @ 0.45 fs [dB]", "att_045", "{:.2f}"),
        ("STEP RESPONSE", None, None),
        ("10-90% rise [samples]", "rise", "{:.2f}"),
        ("delay to 50% [samples]", "t50", "{:.2f}"),
        ("overshoot [%]", "overshoot", "{:.2f}"),
        ("2% settling [samples]", "settle", "{:.0f}"),
        ("steady-state error [LSB]", "ss_err_lsb", "{:+.2f}"),
        ("headroom: max step without clipping [FS]", "headroom", "{:.4f}"),
        ("0.99 FS step: clipped output samples", "fs_clipped", "{:.0f}"),
        ("NOISE", None, None),
        ("noise power ratio sum(h^2)", "npr_theory", "{:.5f}"),
        ("noise power ratio measured var(y)/var(x)", "npr_meas", "{:.5f}"),
        ("noise power reduction, sum(h^2) [dB]", "npr_db", "{:.2f}"),
        ("output error vs ideal, rms [LSB]", "err_rms_lsb", "{:.3f}"),
        ("output error vs ideal, max [LSB]", "err_max_lsb", "{:.1f}"),
        ("output error vs ideal, mean/bias [LSB]", "err_mean_lsb", "{:+.3f}"),
        ("output SNR vs ideal [dB]", "snr_db", "{:.1f}"),
        ("limit cycle: max |y| in impulse tail [LSB]", "tail_lsb", "{:.0f}"),
        ("CONFORMITY", None, None),
        ("max |y - Q15 model|, impulse [LSB]", "model_err_impulse", "{:.0f}"),
        ("max |y - Q15 model|, noise [LSB]", "model_err_noise", "{:.0f}"),
    ]
    width = 46
    print(f"{'':{width}}" + "".join(f"{c:>12}" for c in cols))
    for label, key, f in rows:
        if key is None:
            print(f"\n{label}")
            continue
        vals = []
        for c in cols:
            v = key(spec[c]) if callable(key) else spec[c].get(key)
            vals.append(fmt(v, f))
        if all(v == "-" for v in vals):
            continue
        print(f"  {label:{width - 2}}" + "".join(f"{v:>12}" for v in vals))

    for path in ("raw", "drv"):
        s = spec[path]
        print(f"\n[{path}]")
        if s.get("failures"):
            for test, why in s["failures"].items():
                print(f"  FAILED {test}: {why}")
        if s.get("passthru_bad"):
            print("  passthrough mismatches (index, input counts, got, expected):")
            for i, xin, got, exp in s["passthru_bad"]:
                print(f"    [{i:2d}] in={xin:7d} got={got if got is not None else 'none':>7} expected={exp:7d}")
        if "fs_sr" in s:
            flags = [n for bit, n in SR_FLAGS.items() if s["fs_sr"] >> bit & 1 and bit >= 8]
            print(f"  status flags after 0.99 FS step: {', '.join(flags) or 'none'}")
        if "cyc_med" in spec["raw"] and "cyc_med" in s and path == "drv":
            print(f"  driver overhead vs raw registers: {s['cyc_med'] - spec['raw']['cyc_med']:.0f} cycles median")
        for name, al in s.get("align", {}).items():
            print(f"  init variant {name:>12}: {len(al['pre'])} output(s) before first input {al['pre']}, "
                  f"lag {al['lag']} sample(s), {al['bad']} mismatch(es)")
    for n in notes:
        print(f"\nnote: {n}")


def plot(info, curves, out, show):
    import matplotlib.pyplot as plt

    fs = info["fs"]
    freqs = curves["freqs"]
    style = {"ideal": ("k", "-"), "quantized": ("0.5", "--"), "raw": ("tab:blue", "-"), "drv": ("tab:red", ":")}
    fig, ax = plt.subplots(2, 3, figsize=(16, 9))
    fig.suptitle(f"FMAC characterization: {info['desc']}")

    for k in ("ideal", "raw", "drv"):
        if "impulse" in curves.get(k, {}):
            h = curves[k]["impulse"][:96]
            ax[0, 0].plot(h, color=style[k][0], linestyle=style[k][1], marker="." if k != "ideal" else None, label=k)
    ax[0, 0].set(title="impulse response (normalized)", xlabel="sample")

    for k in ("ideal", "quantized", "raw", "drv"):
        if "H" in curves.get(k, {}):
            ax[0, 1].plot(freqs, db(np.abs(curves[k]["H"])), color=style[k][0], linestyle=style[k][1], label=k)
            ax[0, 2].plot(freqs, curves[k]["gd"], color=style[k][0], linestyle=style[k][1], label=k)
    ax[0, 1].set(title="magnitude", xlabel="Hz", ylabel="dB", ylim=(-100, 5))
    gd_ideal = curves["ideal"]["gd"][freqs <= fs / 4]
    ax[0, 2].set(title="group delay", xlabel="Hz", ylabel="samples", xlim=(0, fs / 4),
                 ylim=(min(0.0, np.nanmin(gd_ideal)) - 1, 1.5 * np.nanmax(gd_ideal) + 1))

    for k in ("ideal", "raw", "drv"):
        if "step" in curves.get(k, {}):
            ax[1, 0].plot(curves[k]["step"][:128], color=style[k][0], linestyle=style[k][1], label=k)
    ax[1, 0].set(title="step response (normalized)", xlabel="sample")

    for k in ("raw", "drv"):
        if "noise_err" in curves.get(k, {}):
            e = curves[k]["noise_err"]
            ax[1, 1].hist(e, bins=np.arange(np.floor(e.min()) - 0.5, np.ceil(e.max()) + 1.5), alpha=0.5,
                          color=style[k][0], label=k)
    ax[1, 1].set(title="noise test: output - ideal", xlabel="LSB")

    for k in ("raw", "drv"):
        if "cycles" in curves.get(k, {}):
            c = curves[k]["cycles"]
            ax[1, 2].hist(c, bins=60, range=(c.min(), np.percentile(c, 99.5)), alpha=0.5, color=style[k][0], label=k)
    ax[1, 2].set(title="process() latency", xlabel="CPU cycles")

    for a in ax.flat:
        a.grid(alpha=0.3)
        if a.get_legend_handles_labels()[0]:
            a.legend()
    fig.tight_layout()
    fig.savefig(out, dpi=120)
    print(f"\nwrote {out}")
    if show:
        plt.show()


def to_jsonable(v):
    if isinstance(v, dict):
        return {str(k): to_jsonable(x) for k, x in v.items()}
    if isinstance(v, (list, tuple)):
        return [to_jsonable(x) for x in v]
    if isinstance(v, (np.floating, float)):
        return None if math.isnan(v) else float(v)
    if isinstance(v, np.integer):
        return int(v)
    return v


def cmd_analyze(args):
    meta, runs = parse_log(Path(args.log).read_text(errors="replace"))
    info, spec, curves, notes = analyze(meta, runs)
    print_report(info, spec, notes)
    if args.json:
        Path(args.json).write_text(json.dumps(to_jsonable({"info": info, "spec": spec, "notes": notes}), indent=2))
        print(f"wrote {args.json}")
    if args.plot or args.show:
        plot(info, curves, args.plot or "fmac_bench.png", args.show)


# ----------------------------------------------------------------------------------------------------------------------
# simulate
# ----------------------------------------------------------------------------------------------------------------------


def read_header(path: Path):
    text = path.read_text()

    def arr(name, conv):
        m = re.search(rf"{name}\s*=\s*\{{([^}}]*)\}}", text)
        return [conv(v.strip(" {}").rstrip("f")) for v in m.group(1).split(",")]

    return {
        "desc": re.search(r'DESCRIPTION\s*=\s*"([^"]*)"', text).group(1),
        "fs_millihz": int(re.search(r"SAMPLE_RATE_MILLIHZ\s*=\s*(\d+)", text).group(1)),
        "R": int(re.search(r"GAIN_EXPONENT\s*=\s*(\d+)", text).group(1)),
        "B_Q15": arr("B_Q15", int),
        "A_Q15": arr("A_Q15", int),
        "B_FLOAT": arr("B_FLOAT", float),
        "A_FLOAT": arr("A_FLOAT", float),
    }


def cmd_simulate(args):
    """Emit the same log format as the firmware, using the Q15 model for both paths (i.e. a bug-free driver)."""
    h = read_header(Path(args.header))
    rng = np.random.default_rng(1)
    bits = lambda v: f"{struct.unpack('<I', struct.pack('<f', v))[0]:08x}"
    lines = [
        "#FMAC_BENCH v1",
        f"meta sysclk_hz=250000000 fs_millihz={h['fs_millihz']} R={h['R']} P={len(h['B_Q15'])} Q={len(h['A_Q15'])}",
        f"desc {h['desc']} (SIMULATED)",
        "coef_b_q15 " + " ".join(map(str, h["B_Q15"])),
        "coef_a_q15 " + " ".join(map(str, h["A_Q15"])),
        "coef_b_f32 " + " ".join(bits(v) for v in h["B_FLOAT"]),
        "coef_a_f32 " + " ".join(bits(v) for v in h["A_FLOAT"]),
    ]
    noise = (rng.integers(-32768, 32768, 4096) // 2).astype(float)
    tests = {
        "passthru": ([16384, 0], [0], 1, np.array([0, 0.5, 0, 0, -0.5, 0.25, 0.999969, 1, -1, 1.5, -1.5] + [0] * 5) * Q15),
        "impulse": (h["B_Q15"], h["A_Q15"], h["R"], np.r_[29491, np.zeros(511)]),
        "step": (h["B_Q15"], h["A_Q15"], h["R"], np.full(512, 16384.0)),
        "step_fs": (h["B_Q15"], h["A_Q15"], h["R"], np.full(512, 32440.0)),
        "noise": (h["B_Q15"], h["A_Q15"], h["R"], noise),
    }
    for path, base in (("drv", 900), ("raw", 60)):
        for test, (bq, aq, r, x) in tests.items():
            xq = np.round(x)
            y = fmac_model(bq, aq, r, np.clip(xq, -32768, 32767))
            lines.append(f"begin {path} {test} n={len(x)} len={len(x)} err=-1 sr=0x1 init_cyc={base * 4} pre=0")
            cyc = base + rng.poisson(3, len(x)) + (rng.random(len(x)) < 0.01) * 400
            lines += [f"{int(a)} {int(b)} {int(c)}" for a, b, c in zip(xq, y, cyc)]
            lines.append(f"end {path} {test}")
    lines.append("#FMAC_BENCH done")
    Path(args.out).write_text("\n".join(lines) + "\n")
    print(f"wrote {args.out}")


# ----------------------------------------------------------------------------------------------------------------------


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)

    d = sub.add_parser("design", help="generate the bench coefficient header")
    d.add_argument("--fs", type=float, default=1000.0, help="sample rate [Hz] (labels only; FMAC is rate-agnostic)")
    d.add_argument("--fc", type=float, default=50.0, help="Butterworth cutoff [Hz]")
    d.add_argument("--order", type=int, default=2, help="Butterworth order")
    d.add_argument("--b", type=float, nargs="+", help="custom numerator coefficients (overrides Butterworth)")
    d.add_argument("--a", type=float, nargs="+", help="custom denominator coefficients, a[0] first")
    d.add_argument("--out", default=str(HEADER_PATH))
    d.set_defaults(fn=cmd_design)

    a = sub.add_parser("analyze", help="turn an RTT channel 2 log into a spec sheet")
    a.add_argument("log")
    a.add_argument("--plot", metavar="PNG", help="save plots to this file")
    a.add_argument("--show", action="store_true", help="open the plot window")
    a.add_argument("--json", metavar="FILE", help="write the specs as JSON")
    a.set_defaults(fn=cmd_analyze)

    s = sub.add_parser("simulate", help="write a synthetic log from the Q15 model")
    s.add_argument("--header", default=str(HEADER_PATH))
    s.add_argument("--out", default="fmac_bench_sim.log")
    s.set_defaults(fn=cmd_simulate)

    args = p.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
