from typing import Tuple
from logfs_lib import LogFsErr, PyLogFs, LogFsFile


def _check_err(err: LogFsErr):
    if err != LogFsErr.OK:
        raise Exception(err)


class LogFsContext:
    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        return LogFsErr.UNIMPLEMENTED

    def prog(self, block: int, data: bytes) -> LogFsErr:
        return LogFsErr.UNIMPLEMENTED

    def erase(self, block: int) -> LogFsErr:
        return LogFsErr.UNIMPLEMENTED


class LogFsDummyContext:
    def __init__(self, block_size: int, block_count: int) -> None:
        self.block_size = block_size
        self.block_count = block_count

        self.disk = []
        for _ in range(block_count):
            self.disk.append(bytes(block_size))

    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        # print(f"read: {block}, {disk[block]}")
        return LogFsErr.OK, self.disk[block]

    def prog(self, block: int, data: bytes) -> LogFsErr:
        # print(f"prog: {block}, {buf}")
        self.disk[block] = data
        return LogFsErr.OK

    def erase(self, block: int) -> LogFsErr:
        # print(f"erase: {block}")
        self.disk[block] = bytes(self.block_size)
        return LogFsErr.OK


class LogFs:
    def __init__(
        self, block_size: int, block_count: int, context: LogFsContext
    ) -> None:
        self.fs = PyLogFs(block_size, block_count, context)

    def format(self) -> None:
        _check_err(self.fs.format())

    def mount(self) -> None:
        _check_err(self.fs.mount())

    def open(self, path: str) -> None:
        file = LogFsFile()
        _check_err(self.fs.open(file, path))
        return file

    def read(self, file: LogFsFile, size: int) -> int:
        num_read, data = self.fs.read(file, size)
        return data[:num_read]

    def write(self, file: LogFsFile, data: bytes) -> None:
        _check_err(self.fs.write(file, data, len(data)))
