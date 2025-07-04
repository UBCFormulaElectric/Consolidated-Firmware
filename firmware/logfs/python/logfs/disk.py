import logging
import os
from typing import Tuple

from logfs.logfs_src import LogFsErr

logger = logging.getLogger(__name__)


class LogFsDisk:
    def __init__(self, block_size: int, block_count: int) -> None:
        self.block_size = block_size
        self.block_count = block_count

        self.writes = 0
        self.reads = 0

        self.write_cycles = []
        for _ in range(block_count):
            self.write_cycles.append(0)

    def write(self, block: int, data: bytes) -> LogFsErr:
        """
        Write function to be invoked by logfs. Users must define a `write_impl`
        function with the same prototype.

        """
        self.writes += 1
        self.write_cycles[block] += 1

        if self.write_impl is None:
            return LogFsErr.IO

        err = self.write_impl(block=block, data=data)

        if err == LogFsErr.OK:
            logger.debug(f"Write @ {block}: {data}")
        else:
            logger.debug(f"Write @ {block}: {err}")

        return err

    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        """
        Read function to be invoked by logfs. Users must define a `write_impl`
        function with the same prototype.

        """
        self.reads += 1
        err, data = self.read_impl(block=block)

        if self.read_impl is None:
            return LogFsErr.IO

        if err == LogFsErr.OK:
            logger.debug(f"Read @ {block}: {data}")
        else:
            logger.debug(f"Read @ {block}: {err}")

        return err, data


class LogFsRamDisk(LogFsDisk):
    """
    Disk used for interacting with a logfs image stored entirely in RAM.
    No practical use, just used for testing/benchmarking.

    """

    def __init__(self, block_size: int, block_count: int) -> None:
        super().__init__(block_size=block_size, block_count=block_count)

        self.disk = []
        for _ in range(block_count):
            self.disk.append(bytes(block_size))

    def write_impl(self, block: int, data: bytes) -> LogFsErr:
        self.disk[block] = data
        return LogFsErr.OK

    def read_impl(self, block: int) -> Tuple[LogFsErr, bytes]:
        return LogFsErr.OK, self.disk[block]


class LogFsUnixDisk(LogFsDisk):
    """
    Disk used for interacting with a logfs image on a disk on a Unix machine.

    """

    def __init__(self, block_size: int, block_count: int, disk_path: str) -> None:
        super().__init__(block_size=block_size, block_count=block_count)

        self.disk = open(disk_path, "r+b")

    def write_impl(self, block: int, data: bytes) -> LogFsErr:
        self.disk.seek(self.block_size * block)
        self.disk.write(data)
        return LogFsErr.OK

    def read_impl(self, block: int) -> Tuple[LogFsErr, bytes]:
        self.disk.seek(self.block_size * block)
        return LogFsErr.OK, self.disk.read(self.block_size)

    def __del__(self):
        if hasattr(self, "disk") and self.disk is not None:
            self.disk.close()


try:
    import win32file
except ImportError:
    win32file = None


class LogFsWinDisk(LogFsDisk):
    """
    Disk used for interacting with a logfs image on a disk on a Windows machine.
    """

    # example disk_patgh E: (indicate the E drive)
    def __init__(self, block_size: int, block_count: int, disk_path: str) -> None:
        super().__init__(block_size=block_size, block_count=block_count)
        if win32file is None:
            raise ImportError("Cannot import win32file")
        disk_path = "\\\\.\\" + disk_path
        self.disk = win32file.CreateFile(
            disk_path,
            win32file.GENERIC_READ | win32file.GENERIC_WRITE,
            win32file.FILE_SHARE_READ | win32file.FILE_SHARE_WRITE,
            None,
            win32file.OPEN_EXISTING,
            0,
            None,
        )

    def write_impl(self, block: int, data: bytes) -> LogFsErr:
        win32file.SetFilePointer(
            self.disk, self.block_size * block, win32file.FILE_BEGIN
        )
        win32file.WriteFile(self.disk, data)
        return LogFsErr.OK

    def read_impl(self, block: int) -> Tuple[LogFsErr, bytes]:
        win32file.SetFilePointer(
            self.disk, self.block_size * block, win32file.FILE_BEGIN
        )
        err, data = win32file.ReadFile(self.disk, self.block_size)
        return LogFsErr.OK, data

    def __del__(self):
        if hasattr(self, "disk") and self.disk is not None:
            win32file.CloseHandle(self.disk)


class LogFsDiskFactory:
    @staticmethod
    def create_disk(block_size: int, block_count: int, disk_path: str):
        if os.name == "nt":
            return LogFsWinDisk(block_size, block_count, disk_path)
        elif os.name == "posix":
            return LogFsUnixDisk(block_size, block_count, disk_path)
        else:
            raise NotImplementedError(f"Unsupported OS: { os.name }")

    @staticmethod
    def create_ram_disk(block_size: int, block_count: int):
        return LogFsRamDisk(block_size, block_count)
