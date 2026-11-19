#!/usr/bin/env bash
#
# Helper script to prepare J-Link GDB Server and build binaries for Zed DAP debugging
#
set -e

DEVICE="STM32H733VG"
SPEED="4000"
PORT="2331"
BUILD_CMD=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --device)
      DEVICE="$2"
      shift 2
      ;;
    --speed)
      SPEED="$2"
      shift 2
      ;;
    --port)
      PORT="$2"
      shift 2
      ;;
    --build)
      BUILD_CMD="$2"
      shift 2
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

# 1. Run build command if specified
if [[ -n "$BUILD_CMD" ]]; then
  echo "==> Running build: $BUILD_CMD"
  eval "$BUILD_CMD"
fi

PID_FILE="/tmp/jlink_gdbserver.pid"
DEV_FILE="/tmp/jlink_gdbserver.device"

# Windows (Git Bash / MSYS2)
IS_WINDOWS=0
if [[ "$OSTYPE" == msys* || "$OSTYPE" == cygwin* ]]; then
  IS_WINDOWS=1
fi

# Helper to check if port is open
is_port_listening() {
  (echo >/dev/null 2>&1 <"/dev/tcp/127.0.0.1/$PORT")
}

# If running, check if device matches
if is_port_listening; then
  CURRENT_DEV=$(cat "$DEV_FILE" 2>/dev/null || echo "")
  if [[ "$CURRENT_DEV" == "$DEVICE" ]]; then
    echo "==> J-Link GDB Server already running on port $PORT for device $DEVICE"
    exit 0
  else
    echo "==> J-Link GDB Server running on port $PORT for different device ($CURRENT_DEV != $DEVICE). Restarting..."
    if [[ -f "$PID_FILE" ]]; then
      kill "$(cat "$PID_FILE")" 2>/dev/null || true
    fi
    pkill -f "JLinkGDBServer" 2>/dev/null || true
    if [[ "$IS_WINDOWS" == 1 ]]; then
      taskkill //F //IM JLinkGDBServerCL.exe >/dev/null 2>&1 || true
    fi
    sleep 0.5
  fi
fi

# Find JLinkGDBServerCLExe binary
if command -v JLinkGDBServerCLExe >/dev/null 2>&1; then
  JLINK_BIN="JLinkGDBServerCLExe"
elif [[ -x "$HOME/.local/bin/JLinkGDBServerCLExe" ]]; then
  JLINK_BIN="$HOME/.local/bin/JLinkGDBServerCLExe"
elif [[ -x "/usr/bin/JLinkGDBServerCLExe" ]]; then
  JLINK_BIN="/usr/bin/JLinkGDBServerCLExe"
elif [[ -x "/opt/SEGGER/JLink/JLinkGDBServerCLExe" ]]; then
  JLINK_BIN="/opt/SEGGER/JLink/JLinkGDBServerCLExe"
elif [[ -x "/Applications/SEGGER/JLink/JLinkGDBServerCLExe" ]]; then
  # macOS
  JLINK_BIN="/Applications/SEGGER/JLink/JLinkGDBServerCLExe"
elif compgen -G "/Applications/SEGGER/JLink_*/JLinkGDBServerCLExe" >/dev/null; then
  # macOS, versioned install folder (newest version wins)
  JLINK_BIN="$(ls -d /Applications/SEGGER/JLink_*/JLinkGDBServerCLExe | sort | tail -n 1)"
elif command -v JLinkGDBServerCL.exe >/dev/null 2>&1; then
  # Windows, J-Link folder on PATH
  JLINK_BIN="$(command -v JLinkGDBServerCL.exe)"
elif compgen -G "/c/Program Files/SEGGER/JLink*/JLinkGDBServerCL.exe" >/dev/null; then
  # Windows, default install folder (newest version wins)
  JLINK_BIN="$(ls -d /c/Program\ Files/SEGGER/JLink*/JLinkGDBServerCL.exe | sort | tail -n 1)"
else
  echo "Error: JLinkGDBServerCLExe not found!"
  exit 1
fi

echo "==> Starting J-Link GDB Server for $DEVICE on port $PORT..."
if [[ "$IS_WINDOWS" == 1 ]]; then
  # Start-Process gives the server its own hidden console. A nohup'd child would stay attached to the editor
  # task's console and stop when that task ends.
  powershell.exe -NoProfile -Command "Start-Process -WindowStyle Hidden -FilePath '$(cygpath -w "$JLINK_BIN")' \
    -ArgumentList '-device $DEVICE -if SWD -speed $SPEED -port $PORT -swoport 2332 -telnetport 2333 -nohalt -silent'"
  NEW_PID="see Task Manager"
  rm -f "$PID_FILE"
else
  nohup "$JLINK_BIN" \
    -device "$DEVICE" \
    -if SWD \
    -speed "$SPEED" \
    -port "$PORT" \
    -swoport 2332 \
    -telnetport 2333 \
    -nohalt \
    -silent > /tmp/jlink_gdbserver.log 2>&1 &

  NEW_PID=$!
  echo "$NEW_PID" > "$PID_FILE"
fi
echo "$DEVICE" > "$DEV_FILE"

# Wait briefly for port to open
for i in {1..20}; do
  if is_port_listening; then
    echo "==> J-Link GDB Server listening on port $PORT (PID $NEW_PID)"
    exit 0
  fi
  sleep 0.1
done

if [[ "$IS_WINDOWS" == 1 ]]; then
  echo "==> J-Link GDB Server launched, but port $PORT is not open yet. Is the J-Link connected?"
else
  echo "==> J-Link GDB Server launched (PID $NEW_PID). Output in /tmp/jlink_gdbserver.log"
fi
exit 0
