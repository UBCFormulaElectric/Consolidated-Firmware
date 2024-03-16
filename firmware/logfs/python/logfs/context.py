from typing import Tuple
from logfs_src import LogFsErr


class LogFsContext:
    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        return LogFsErr.IO

    def write(self, block: int, data: bytes) -> LogFsErr:
        return LogFsErr.IO


class LogFsDummyContext(LogFsContext):
    def __init__(self, block_size: int, block_count: int) -> None:
        self.block_size = block_size

        self.reads = 0
        self.writes = 0

        self.disk = []
        for _ in range(block_count):
            self.disk.append(bytes(block_size))

    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        # print(f"read @ {block}: {self.disk[block]}")
        self.reads += 1
        return LogFsErr.OK, self.disk[block]

    def write(self, block: int, data: bytes) -> LogFsErr:
        # print(f"write @ {block}: {data}")
        self.disk[block] = data
        self.writes += 1
        return LogFsErr.OK


class LogFsUnixContext(LogFsContext):
    def __init__(self, block_size: int, block_count: int, disk_path: str) -> None:
        self.block_size = block_size
        self.block_count = block_count
        self.disk = open(disk_path, "r+b")

    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        self.disk.seek(self.block_size * block)
        return LogFsErr.OK, self.disk.read(self.block_size)

    def write(self, block: int, data: bytes) -> LogFsErr:
        self.disk.seek(self.block_size * block)
        self.disk.write(data)
        return LogFsErr.OK

    def __del__(self):
        if hasattr(self, "disk") and self.disk is not None:
            self.disk.close()
