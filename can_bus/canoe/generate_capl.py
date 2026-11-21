#!/usr/bin/env python3
"""
Generate a CANoe rest-bus simulation CAPL script (and a CANoe-ready DBC) from the
JSONCAN definitions of a car.

What it does:
  1. Builds/runs the Rust JSONCAN codegen (scripts/code_generation/jsoncan-rust) once
     per node, which regenerates the DBC exactly like the firmware build does and also
     emits each node's io_canTx / io_canReroute sources.
  2. Reads cycle times and start values from the DBC, and which bus(es) every message
     is put on (and which messages VC/BMS forward between buses) from the generated C++.
  3. Writes <car>_restbus.can plus <car>_canoe.dbc (the same DBC with VFrameFormat
     added so CANoe treats FDCAN messages as CAN FD frames).

Usage:
  python3 can_bus/canoe/generate_capl.py [--car hexray]
"""

import argparse
import json
import re
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
JSONCAN_RUST = REPO / "scripts" / "code_generation" / "jsoncan-rust"
CODEGEN_BIN = JSONCAN_RUST / "target" / "release" / "cppcodegen"

# Valid CAN FD payload sizes; the firmware rounds up to the next one (hw_can_hx.cpp).
FD_SIZES = [0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64]

# Nodes that are off by default: not normal application traffic.
DEFAULT_DISABLED = {"Boot", "Debug", "DAMLogger"}


# ----------------------------------------------------------------------------- codegen
def run_codegen(car_dir: Path, out_root: Path, nodes: list[str]) -> Path:
    subprocess.run(
        ["cargo", "build", "--release", "--bin", "cppcodegen"],
        cwd=JSONCAN_RUST,
        check=True,
        stdout=subprocess.DEVNULL,
    )
    dbc = out_root / "car.dbc"
    for node in nodes:
        subprocess.run(
            [
                str(CODEGEN_BIN),
                "--board", node,
                "--can-data-dir", str(car_dir),
                "--output-dir", str(out_root / node),
                "--dbc-output", str(dbc),
            ],
            cwd=JSONCAN_RUST,
            check=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
    return dbc


# ----------------------------------------------------------------------------- DBC
def parse_dbc(text: str):
    msgs = {}  # name -> dict
    by_dbc_id = {}
    cur = None
    for line in text.splitlines():
        m = re.match(r"^BO_ (\d+) (\w+): (\d+) (\w+)", line)
        if m:
            dbc_id, name, length, tx = int(m[1]), m[2], int(m[3]), m[4]
            cur = {
                "name": name,
                "dbc_id": dbc_id,
                "id": dbc_id & 0x1FFFFFFF,
                "extended": bool(dbc_id & 0x80000000),
                "len": length,
                "tx": tx,
                "cycle": 0,
                "signals": [],
                "start": {},
            }
            msgs[name] = cur
            by_dbc_id[dbc_id] = cur
            continue
        m = re.match(r"^\s+SG_ (\w+)\s*:", line)
        if m and cur:
            cur["signals"].append(m[1])
            continue
        m = re.match(r'^BA_ "GenMsgCycleTime" BO_ (\d+) (\d+);', line)
        if m:
            by_dbc_id[int(m[1])]["cycle"] = int(m[2])
            continue
        m = re.match(r'^BA_ "GenSigStartValue" SG_ (\d+) (\w+) (\S+);', line)
        if m:
            by_dbc_id[int(m[1])]["start"][m[2]] = m[3]
    return msgs


def clamp_classic_msgs(text: str, msgs: dict, fd_msgs: set[str]) -> str:
    """
    A message that only lives on a classic bus can't be longer than 8 bytes. If JSONCAN
    sized one bigger (a signal runs past bit 63), clamp it to 8 bytes for CANoe and trim
    the overrunning little-endian signals, warning so the JSON can be fixed.
    """
    lines = text.splitlines(keepends=True)
    cur = None
    for i, line in enumerate(lines):
        m = re.match(r"^BO_ (\d+) (\w+): (\d+) (\w+)", line)
        if m:
            cur = msgs[m[2]] if (m[2] not in fd_msgs and int(m[3]) > 8) else None
            if cur:
                lines[i] = f"BO_ {m[1]} {m[2]}: 8 {m[4]}\n"
                print(f"warning: {m[2]} is {m[3]} bytes on a classic-only bus; clamped to 8 in the CANoe DBC", file=sys.stderr)
            continue
        if not line.startswith("\tSG_") and not line.startswith(" SG_"):
            cur = None if not line.strip() else cur
            continue
        if cur is None:
            continue
        s = re.match(r"^(\s+SG_ (\w+)\s*: )(\d+)\|(\d+)@1([+-]) \(([^,]+),([^)]+)\) \[([^|]+)\|([^\]]+)\](.*)$", line.rstrip("\n"))
        if s and int(s[3]) + int(s[4]) > 64:
            start, bits = int(s[3]), 64 - int(s[3])
            maxv = (1 << bits) - 1
            lines[i] = f"{s[1]}{start}|{bits}@1{s[5]} ({s[6]},{s[7]}) [{s[8]}|{maxv}]{s[10]}\n"
            print(f"warning: {s[2]} ends at bit {int(s[3]) + int(s[4])}; trimmed to {bits} bits (fix the JSON)", file=sys.stderr)
    for m in msgs.values():
        if m["name"] not in fd_msgs and m["len"] > 8:
            m["len"] = 8
    return "".join(lines)


def add_vframeformat(text: str, msgs: dict, fd_msgs: set[str]) -> str:
    """CANoe uses VFrameFormat to decide CAN vs CAN FD per message."""
    defs = (
        'BA_DEF_ BO_  "VFrameFormat" ENUM  "StandardCAN","ExtendedCAN","reserved",'
        '"reserved","reserved","reserved","reserved","reserved","reserved","reserved",'
        '"reserved","reserved","reserved","reserved","StandardCAN_FD","ExtendedCAN_FD";\n'
        'BA_DEF_DEF_  "VFrameFormat" "StandardCAN";\n'
    )
    vals = ""
    for m in msgs.values():
        fd = m["name"] in fd_msgs
        fmt = (15 if m["extended"] else 14) if fd else (1 if m["extended"] else 0)
        vals += f'BA_ "VFrameFormat" BO_ {m["dbc_id"]} {fmt};\n'
    text = text.replace('BA_DEF_DEF_  "BusType" "CAN";', defs + 'BA_DEF_DEF_  "BusType" "CAN";', 1)
    return text.replace('BA_ "BusType" "CAN FD";', 'BA_ "BusType" "CAN FD";\n' + vals, 1)


# ----------------------------------------------------------------------------- routing
def parse_tx_buses(cpp: str) -> dict[str, list[str]]:
    """message name -> ordered list of buses the owning node enqueues it on."""
    out = {}
    for m in re.finditer(r"void (\w+)_send(?:Periodic|Aperiodic)\(\)\s*\{(.*?)\n\}", cpp, re.S):
        out[m[1]] = re.findall(r"enqueue_func_(\w+)\(tx_msg\)", m[2])
    return out


def parse_reroutes(cpp: str) -> list[tuple[str, str, str]]:
    """[(src_bus, msg, dst_bus)] for a forwarding node."""
    out = []
    for fn in re.finditer(r"void reroute_(\w+)\(const JsonCanMsg& rx_msg\)\s*\{(.*?)\n\}", cpp, re.S):
        for case in re.finditer(r"case app::can_utils::(\w+)_Signals::MSG_ID:\s*\{(.*?)\}", fn[2], re.S):
            for dst in re.findall(r"transmit_func_(\w+)\(rx_msg\)", case[2]):
                out.append((fn[1], case[1], dst))
    return out


# ----------------------------------------------------------------------------- CAPL
def fd_size(n: int) -> int:
    return next(s for s in FD_SIZES if s >= n)


def capl_value(v: str) -> str:
    return v if re.fullmatch(r"-?\d+(\.\d+)?", v) else "0"


def emit_capl(car, bus_cfg, msgs, tx_buses, reroutes, generated_from) -> str:
    buses = [b["name"] for b in bus_cfg["buses"]]
    fd_bus = {b["name"]: bool(b.get("fd", False)) for b in bus_cfg["buses"]}
    speed = {b["name"]: b["bus_speed"] for b in bus_cfg["buses"]}
    ch_const = {b: f"CH_{b.upper()}" for b in buses}

    periodic = [m for m in msgs.values() if m["cycle"] > 0 and tx_buses.get(m["name"])]
    no_bus = [m for m in msgs.values() if m["cycle"] > 0 and not tx_buses.get(m["name"])]
    aperiodic = [m for m in msgs.values() if m["cycle"] == 0]
    nodes = sorted({m["tx"] for m in periodic} | {f for f, *_ in reroutes})
    by_period = defaultdict(list)
    for m in sorted(periodic, key=lambda m: (m["tx"], m["id"])):
        by_period[m["cycle"]].append(m)

    # forwarding: (msg) -> [(forwarder, src_bus, dst_bus)]
    fwd = defaultdict(list)
    for forwarder, src, name, dst in reroutes:
        if name in msgs:
            fwd[name].append((forwarder, src, dst))

    L = []
    w = L.append
    w("/*@!Encoding:1252*/")
    w("/*")
    w(f" * {car} rest-bus simulation for Vector CANoe.")
    w(" *")
    w(" * AUTO-GENERATED by can_bus/canoe/generate_capl.py. Do not edit by hand; rerun the script")
    w(f" * after changing can_bus/{car}/*.json. Generated from: {generated_from}")
    w(" *")
    w(" * Setup:")
    w(f" *   - Add {car}_canoe.dbc to the CANoe configuration and assign it to every channel below.")
    for b in buses:
        kind = "CAN FD, BRS off (data rate = arbitration rate)" if fd_bus[b] else "classic CAN"
        w(f" *   - {b}: {speed[b]} kbit/s, {kind} -> {ch_const[b]}")
    w(" *   - Add this file as a CAPL node in the Simulation Setup (any channel; each send sets .CAN).")
    w(" *   - Set gSim_<Node> = 0 for every node that is real hardware on the bench.")
    w(" *")
    w(" * Timing mirrors the firmware: every periodic message is sent at each multiple of its")
    w(" * GenMsgCycleTime from measurement start, on the bus(es) its owner sends it on. When a")
    w(" * forwarding node (VC, BMS) is simulated it also forwards the messages it reroutes in the")
    w(" * firmware, both from simulated nodes and from real hardware.")
    w(" * Signals start at GenSigStartValue; every <Node>_Heartbeat is driven to 1.")
    w(" */")
    w("")
    w("includes")
    w("{")
    w("}")
    w("")
    w("variables")
    w("{")
    w("  // ---- CANoe channel per bus: edit to match your hardware/channel mapping ----")
    for i, b in enumerate(buses, 1):
        w(f"  const int {ch_const[b]} = {i};")
    w("")
    w("  // ---- Which transmitters to simulate (1 = CANoe sends its messages) ----")
    for n in nodes:
        w(f"  int gSim_{n} = {0 if n in DEFAULT_DISABLED else 1};")
    w("")
    w("  // ---- One cyclic timer per distinct cycle time ----")
    for p in sorted(by_period):
        w(f"  msTimer tCycle_{p}ms;")
    w("")
    w("  // ---- Periodic messages ----")
    for m in sorted(periodic, key=lambda m: (m["tx"], m["id"])):
        w(f"  message {m['name']} m_{m['name']};")
    w("}")
    w("")

    # init
    w("void InitMessages()")
    w("{")
    for m in sorted(periodic, key=lambda m: (m["tx"], m["id"])):
        n = m["name"]
        for sig, val in m["start"].items():
            v = capl_value(val)
            if v not in ("0", "0.0"):
                w(f"  m_{n}.{sig} = {v};")
        hb = f"{m['tx']}_Heartbeat"
        if hb in m["signals"]:
            w(f"  m_{n}.{hb} = 1;")
    w("}")
    w("")

    # helpers
    w("void SendFd(message * m, int ch, int len)")
    w("{")
    w("  m.CAN = ch;")
    w("  m.FDF = 1;")
    w("  m.BRS = 0;")
    w("  m.DataLength = len;")
    w("  output(m);")
    w("}")
    w("")
    w("void SendClassic(message * m, int ch, int len)")
    w("{")
    w("  m.CAN = ch;")
    w("  m.FDF = 0;")
    w("  m.BRS = 0;")
    w("  m.DLC = len;")
    w("  output(m);")
    w("}")
    w("")

    def send_line(var, bus, length, indent):
        if fd_bus[bus]:
            return f"{indent}SendFd({var}, {ch_const[bus]}, {fd_size(length)});"
        return f"{indent}SendClassic({var}, {ch_const[bus]}, {min(length, 8)});"

    # per-message send functions
    for m in sorted(periodic, key=lambda m: (m["tx"], m["id"])):
        n, tx = m["name"], m["tx"]
        w(f"void Send_{n}()")
        w("{")
        for bus in tx_buses[n]:
            w(send_line(f"m_{n}", bus, m["len"], "  "))
        for forwarder, src, dst in fwd.get(n, []):
            if forwarder == tx or dst in tx_buses[n]:
                continue
            w(f"  if (gSim_{forwarder}) // {forwarder} forwards {src} -> {dst}")
            w(send_line(f"m_{n}", dst, m["len"], "    "))
        w("}")
        w("")

    # timers
    for p in sorted(by_period):
        w(f"on timer tCycle_{p}ms")
        w("{")
        for m in by_period[p]:
            w(f"  if (gSim_{m['tx']}) Send_{m['name']}();")
        w("}")
        w("")

    # forwarding of real-hardware frames by a simulated forwarder
    fwd_handlers = [
        (n, entries) for n, entries in fwd.items()
        if any(f != msgs[n]["tx"] for f, *_ in entries)
    ]
    for n, entries in sorted(fwd_handlers, key=lambda e: msgs[e[0]]["id"]):
        m = msgs[n]
        w(f"on message {n}")
        w("{")
        w("  message * f;")
        w("  if (this.dir != RX) return;")
        for forwarder, src, dst in entries:
            # Only when the source is real hardware; simulated sources are covered in Send_*().
            w(f"  if (gSim_{forwarder} && !gSim_{m['tx']} && this.CAN == {ch_const[src]})")
            w("  {")
            w("    f = this;")
            w(send_line("f", dst, m["len"], "    "))
            w("  }")
        w("}")
        w("")

    w("on start")
    w("{")
    w("  InitMessages();")
    for p in sorted(by_period):
        w(f"  setTimerCyclic(tCycle_{p}ms, {p});")
    w(f'  write("{car} rest-bus sim: {len(periodic)} periodic messages, {len(by_period)} cycle groups");')
    w("}")
    w("")
    w("on stopMeasurement")
    w("{")
    for p in sorted(by_period):
        w(f"  cancelTimer(tCycle_{p}ms);")
    w("}")
    w("")
    w("/*")
    w(f" * Not sent by this script: {len(aperiodic)} event-driven messages (no GenMsgCycleTime),")
    w(" * e.g. bootloader traffic, Bootup and Debug commands. Send them from the Interactive")
    w(" * Generator or add handlers here:")
    for m in sorted(aperiodic, key=lambda m: (m["tx"], m["id"])):
        w(f" *   {m['tx']:<10} 0x{m['id']:X} {m['name']}")
    if no_bus:
        w(" * Periodic in the DBC but no transmitting bus found in codegen (skipped):")
        for m in no_bus:
            w(f" *   {m['tx']:<10} 0x{m['id']:X} {m['name']}")
    w(" */")
    return "\n".join(L) + "\n"


# ----------------------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--car", default="hexray")
    ap.add_argument("--out-dir", default=str(Path(__file__).resolve().parent))
    args = ap.parse_args()

    car_dir = REPO / "can_bus" / args.car
    bus_cfg = json.loads((car_dir / "bus.json").read_text())
    nodes = sorted(p.name for p in car_dir.iterdir() if p.is_dir())

    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        dbc_path = run_codegen(car_dir, tmp, nodes)
        dbc_text = dbc_path.read_text()
        msgs = parse_dbc(dbc_text)

        tx_buses: dict[str, list[str]] = {}
        reroutes = []
        for node in nodes:
            tx_cpp = tmp / node / "io" / "io_canTx.cpp"
            if tx_cpp.exists():
                tx_buses.update(parse_tx_buses(tx_cpp.read_text()))
            rr_cpp = tmp / node / "io" / "io_canReroute.cpp"
            if rr_cpp.exists():
                reroutes += [(node, *r) for r in parse_reroutes(rr_cpp.read_text())]

    fd_names = {b["name"] for b in bus_cfg["buses"] if b.get("fd")}
    # Messages whose owner sends them on an FD bus. Frames forwarded from a classic bus
    # keep their classic payload (<= 8 bytes), so they don't make a message "FD-sized".
    fd_msgs = {n for n, bs in tx_buses.items() if any(b in fd_names for b in bs)}
    dbc_text = clamp_classic_msgs(dbc_text, msgs, fd_msgs)
    fd_msgs |= {name for _, _, name, dst in reroutes if dst in fd_names}

    rev = subprocess.run(["git", "rev-parse", "--short", "HEAD"], cwd=REPO, capture_output=True, text=True).stdout.strip()
    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / f"{args.car}_canoe.dbc").write_text(add_vframeformat(dbc_text, msgs, fd_msgs))
    capl = emit_capl(args.car, bus_cfg, msgs, tx_buses, reroutes, f"can_bus/{args.car} @ {rev}")
    (out_dir / f"{args.car}_restbus.can").write_text(capl)

    n_periodic = sum(1 for m in msgs.values() if m["cycle"] > 0)
    print(f"{len(msgs)} messages, {n_periodic} periodic -> {out_dir / (args.car + '_restbus.can')}")


if __name__ == "__main__":
    sys.exit(main())
