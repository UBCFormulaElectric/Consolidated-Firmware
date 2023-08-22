import sys
import subprocess
import threading
import time


class OpenOcdCommand:
    def __init__(self, *args):
        self.commands = args

    def as_arg(self) -> str:
        return f' -c "{"; ".join(self.commands)}"'


class Interface:
    JLINK_SWD = OpenOcdCommand(
        "source [find interface/jlink.cfg]", "transport select swd"
    )


class Target:
    STM32F3X = OpenOcdCommand("source [find target/stm32f3x.cfg]")
    STM32F4X = OpenOcdCommand("source [find target/stm32f4x.cfg]")


class OpenOcdServer:
    _WINDOWS_CMD = '"C:\\Program Files\\OpenOCD\\bin\\openocd.exe"'
    _UNIX_CMD = "/usr/bin/openocd"

    def __init__(
        self, name: str, interface: Interface, target: Target, port: int
    ) -> None:
        self.name = name
        self.interface = interface
        self.target = target
        self.port = port

        self.stop_event = threading.Event()
        self.server_thread = threading.Thread(target=self._server_thread)
        self.server_thread.daemon = True
        self.server_thread.start()

    def alive(self) -> bool:
        return self.server_thread.is_alive()

    def stop(self) -> None:
        if not self.alive():
            # Nothing to do if server already stopped.
            return

        if not self.stop_event.is_set():
            print(f"Closing OpenOCD GDB server for {self.name}...")
            self.stop_event.set()
            self.server_thread.join()
            return

    def _server_thread(self):
        commands = [
            self.interface,
            self.target,
            # Bind server to localhost, allowing remote connections.
            OpenOcdCommand("bindto 0.0.0.0"),
            # Configure port.
            OpenOcdCommand(f"gdb_port {self.port}"),
            # Reset MCU on connect.
            OpenOcdCommand("init", "reset init"),
        ]
        invocation = self._platform_invocation()
        invocation += " ".join(command.as_arg() for command in commands)

        print(f"Starting OpenOCD GDB server for {self.name}.")
        with subprocess.Popen(
            invocation, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        ) as process:
            # Wait until stop event is set, or OpenOCD command finishes.
            while not self.stop_event.is_set() and process.poll() is None:
                pass

            # Terminate subprocess, and get data piped to stdout.
            process.terminate()
            return_code = process.returncode
            _, stderr_data = process.communicate()
            stderr_str = stderr_data.decode("utf-8")

        # Print error if return code was unsuccessful.
        # This is unexpected, so useful for debugging.
        if return_code is not None and return_code != 0:
            print("Server raised an error:")
            print(stderr_str)
        else:
            print("Server closed without errors.")

    def _platform_invocation(self) -> str:
        if sys.platform.startswith("win"):
            return self._WINDOWS_CMD
        else:
            # Assume Unix-like if not on Windows.
            return self._UNIX_CMD


if __name__ == "__main__":
    gdb_server = OpenOcdServer(
        name="test",
        interface=Interface.JLINK_SWD,
        target=Target.STM32F4X,
        port=3334,
    )
    time.sleep(1)
    gdb_server.stop()
