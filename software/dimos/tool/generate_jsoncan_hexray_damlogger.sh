#!/usr/bin/env bash
set -euo pipefail

DIMOS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ROOT_DIR="$(cd "$DIMOS_DIR/../.." && pwd)"

CAN_DATA_DIR="$ROOT_DIR/can_bus/hexray"
OUT_DIR="$DIMOS_DIR/native/jsoncan_gen/hexray/DAMLogger"

TMP_DIR="$(mktemp -d)"
cleanup() { rm -rf "$TMP_DIR"; }
trap cleanup EXIT

mkdir -p "$TMP_DIR"

PYTHONPATH="$ROOT_DIR/scripts/code_generation" \
  python3 -m jsoncan.generate_can_from_json \
  --can_data_dir "$CAN_DATA_DIR" \
  --dbc_output "$TMP_DIR/hexray.dbc" \
  --board DAMLogger \
  --output_dir "$TMP_DIR/out"

rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"
cp -R "$TMP_DIR/out/"* "$OUT_DIR/"
cp "$TMP_DIR/hexray.dbc" "$OUT_DIR/"

echo "Generated jsoncan C + DBC into: $OUT_DIR"

