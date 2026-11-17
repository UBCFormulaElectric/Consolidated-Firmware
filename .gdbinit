# GDB Initialization for Consolidated-Firmware
set architecture auto

# helper commands for J-Link / Cortex-M debugging
define rst
  monitor reset
end
document rst
Reset the target processor via J-Link monitor command.
end

define halt
  monitor halt
end
document halt
Halt the target processor via J-Link monitor command.
end

define flash
  load
  monitor reset
end
document flash
Flash the current program to the target and reset.
end

# Editor debugging through GDB's DAP server (Zed): run after attach, and update breakpoints while the target runs.
# Does nothing in other GDB sessions. Keep this last: GDB without Python stops reading this file at this line.
source scripts/debugging/gdb_dap_remote.py
