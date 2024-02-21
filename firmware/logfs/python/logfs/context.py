from typing import Tuple
from logfs_src import LogFsErr


class LogFsContext:
    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        return LogFsErr.UNIMPLEMENTED

    def prog(self, block: int, data: bytes) -> LogFsErr:
        return LogFsErr.UNIMPLEMENTED

    def erase(self, block: int) -> LogFsErr:
        return LogFsErr.UNIMPLEMENTED

    def mass_erase(self) -> LogFsErr:
        return LogFsErr.UNIMPLEMENTED


class LogFsDummyContext(LogFsContext):
    def __init__(self, block_size: int, block_count: int) -> None:
        self.block_size = block_size
        self.block_count = block_count

        self.disk = []
        for _ in range(block_count):
            self.disk.append(bytes(block_size))

    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        return LogFsErr.OK, self.disk[block]

    def prog(self, block: int, data: bytes) -> LogFsErr:
        self.disk[block] = data
        return LogFsErr.OK

    def erase(self, block: int) -> LogFsErr:
        self.disk[block] = bytes(self.block_size)
        return LogFsErr.OK

    def mass_erase(self) -> LogFsErr:
        for i in range(0, self.block_count):
            self.erase(i)

        return LogFsErr.OK


class LogFsUnixContext(LogFsContext):
    def __init__(self, block_size: int, block_count: int, disk_path: str) -> None:
        self.block_size = block_size
        self.block_count = block_count
        self.disk = open(disk_path, "r+b")

    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        self.disk.seek(self.block_size * block)
        return LogFsErr.OK, self.disk.read(self.block_size)

    def prog(self, block: int, data: bytes) -> LogFsErr:
        self.disk.seek(self.block_size * block)
        self.disk.write(data)
        return LogFsErr.OK

    def erase(self, block: int) -> LogFsErr:
        self.disk.seek(self.block_size * block)
        self.disk.write(bytes([0x00] * self.block_size))
        return LogFsErr.OK

    def mass_erase(self) -> LogFsErr:
        for i in range(0, self.block_count):
            self.erase(i)

        return LogFsErr.OK

    def __del__(self):
        if hasattr(self, "disk") and self.disk is not None:
            self.disk.close()
