"""
GDB helpers for debugging an embedded target from an editor through GDB's DAP interpreter (for example Zed's GDB
adapter, which runs "gdb -i=dap" and attaches with "target remote :2331" to the J-Link GDB server).

Loaded by /.gdbinit. It does nothing unless GDB runs as a DAP server and the inferior is on a remote connection, so
host test debugging and plain command-line GDB are not changed.

It adds two things that VS Code's cortex-debug does, and that GDB's DAP server does not do:

1. Run after attach.
   "target remote" always halts the core, and DAP "attach" has no option to continue after that. The editor then
   shows a stop at the reset vector (or at a random location if the core was running). This script hides that stop
   from the editor and continues, so the firmware runs to the first breakpoint.

2. Breakpoint changes while the target runs.
   In all-stop mode GDB writes a new breakpoint to the target at once when the target runs, and a remote target
   cannot accept memory writes while it runs. So GDB fails the DAP request with "Cannot execute this command while
   the target is running", and the breakpoint is not set. (The J-Link GDB server does not support non-stop mode.)
   This script wraps the DAP breakpoint requests: when the target runs, it interrupts the target, hides that stop
   from the editor, lets DAP set the breakpoints, and continues.

This uses internal names of GDB's DAP code (gdb/python/lib/gdb/dap, checked against GDB 18.1): server._commands,
events._on_stop, events._suppress_cont, events._expected_stop_reason, events.inferior_running.
"""

import sys
import threading

import gdb

# DAP requests that change breakpoints. Each is called on the DAP thread, and then waits for the GDB thread.
_BREAKPOINT_REQUESTS = ("setBreakpoints", "setFunctionBreakpoints", "setInstructionBreakpoints")
# How long a breakpoint request waits for the target to halt.
_INTERRUPT_TIMEOUT_S = 5.0

_installed = False
_waiting_for_objfile = False

# Set while a breakpoint request waits for our interrupt. Only read and written on the GDB thread.
_interrupt_pending = False
# Set by the stop handler when the target halted, so the waiting DAP thread can continue.
_halted = threading.Event()
# True if the stop was our interrupt (and so the target must continue after the breakpoints are set).
# False if a real stop (for example a breakpoint hit) came first: then the target stays stopped.
_resume_after_update = False


def _dap_module(name):
    return sys.modules.get("gdb.dap." + name)


def _is_remote():
    try:
        connection = gdb.selected_inferior().connection
    except (gdb.error, RuntimeError):
        return False
    return connection is not None and connection.type in ("remote", "extended-remote")


def _continue_hidden(events):
    """Continue the target without a DAP "continued" event. Call on the GDB thread."""
    events._suppress_cont = True
    try:
        gdb.execute("continue")
    except gdb.error as e:
        events._suppress_cont = False
        gdb.write(f"gdb_dap_remote: could not continue: {e}\n", gdb.STDERR)


def _on_stop(events, dap_on_stop, event):
    global _interrupt_pending, _resume_after_update

    if not _is_remote():
        dap_on_stop(event)
        return

    if events._expected_stop_reason == "attach":
        # The halt from "target remote". Do not show it, and run the firmware.
        events._expected_stop_reason = None
        events.inferior_running = False
        gdb.post_event(lambda: _continue_hidden(events))
        return

    if _interrupt_pending:
        _interrupt_pending = False
        if isinstance(event, gdb.BreakpointEvent):
            # A breakpoint hit before our interrupt arrived. Show it, and keep the target stopped.
            _resume_after_update = False
            dap_on_stop(event)
        else:
            # Our interrupt. Do not show it. The breakpoint request continues the target when it is done.
            _resume_after_update = True
            events.inferior_running = False
        _halted.set()
        return

    dap_on_stop(event)


def _interrupt_for_update():
    """Halt the target for a breakpoint update. Called on the GDB thread."""
    global _interrupt_pending, _resume_after_update

    events = _dap_module("events")
    if not events.inferior_running or not _is_remote():
        # Already stopped (or not a remote target): nothing to do.
        _resume_after_update = False
        _halted.set()
        return

    _interrupt_pending = True
    try:
        gdb.execute("interrupt -a")
    except gdb.error as e:
        _interrupt_pending = False
        _resume_after_update = False
        gdb.write(f"gdb_dap_remote: could not interrupt to set breakpoints: {e}\n", gdb.STDERR)
        _halted.set()


def _finish_update():
    """Continue the target after a breakpoint update, if our interrupt stopped it. Called on the GDB thread."""
    global _interrupt_pending, _resume_after_update

    _interrupt_pending = False
    if _resume_after_update:
        _resume_after_update = False
        _continue_hidden(_dap_module("events"))


def _wrap_breakpoint_request(dap_request):
    """Wrap a DAP breakpoint request. The wrapper runs on the DAP thread."""

    def wrapper(**args):
        events = _dap_module("events")
        if not events.inferior_running:
            return dap_request(**args)

        _halted.clear()
        gdb.post_event(_interrupt_for_update)
        if not _halted.wait(_INTERRUPT_TIMEOUT_S):
            gdb.post_event(_finish_update)
            raise gdb.GdbError("gdb_dap_remote: the target did not halt, so the breakpoints were not set")
        try:
            return dap_request(**args)
        finally:
            gdb.post_event(_finish_update)

    return wrapper


def _install(*_args):
    global _installed, _waiting_for_objfile
    if _installed:
        return

    events = _dap_module("events")
    server = _dap_module("server")
    if events is None or server is None:
        # Not a DAP session (yet).
        return

    # Replace the DAP stop handler with a wrapper, so the stops that this script causes are not sent to the editor.
    dap_on_stop = events._on_stop
    gdb.events.stop.disconnect(dap_on_stop)
    gdb.events.stop.connect(lambda event: _on_stop(events, dap_on_stop, event))

    for name in _BREAKPOINT_REQUESTS:
        if name in server._commands:
            server._commands[name] = _wrap_breakpoint_request(server._commands[name])

    # (disconnect() raises if the callback is not connected.)
    if _waiting_for_objfile:
        gdb.events.new_objfile.disconnect(_install)
        _waiting_for_objfile = False
    _installed = True


# In a DAP session the DAP modules are normally loaded before the init files. If not, install at the first new
# objfile: DAP "attach" loads the program ("file") before "target remote".
_install()
if not _installed:
    _waiting_for_objfile = True
    gdb.events.new_objfile.connect(_install)
