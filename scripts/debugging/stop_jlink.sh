#!/usr/bin/env bash
#
# Helper script to stop running J-Link GDB Server
#
PID_FILE="/tmp/jlink_gdbserver.pid"
DEV_FILE="/tmp/jlink_gdbserver.device"

if [[ -f "$PID_FILE" ]]; then
  kill "$(cat "$PID_FILE")" 2>/dev/null || true
  rm -f "$PID_FILE" "$DEV_FILE"
fi

pkill -f "JLinkGDBServer" 2>/dev/null || true
if [[ "$OSTYPE" == msys* || "$OSTYPE" == cygwin* ]]; then
  # Windows (Git Bash / MSYS2)
  taskkill //F //IM JLinkGDBServerCL.exe >/dev/null 2>&1 || true
  rm -f "$DEV_FILE"
fi
echo "==> J-Link GDB Server stopped."
