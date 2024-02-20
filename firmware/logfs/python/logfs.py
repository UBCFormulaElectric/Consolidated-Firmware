from typing import Tuple
from logfs_lib import LogFsErr, PyLogFsCfg, PyLogFs, LogFsFile


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
        self.disk = []

        for block in range(block_count):
            self.disk.append(bytes(block_size))

    def read(self, block: int) -> Tuple[LogFsErr, bytes]:
        # print(f"read: {block}, {disk[block]}")
        return LogFsErr.OK, self.disk[block]

    def prog(self, block: int, data: bytes) -> LogFsErr:
        # print(f"prog: {block}, {buf}")
        self.disk[block] = data
        return LogFsErr.OK

    def erase(block: int):
        # print(f"erase: {block}")
        return LogFsErr.OK


BLOCK_COUNT = 100
BLOCK_SIZE = 32


disk = []
for block in range(BLOCK_COUNT):
    disk.append(bytes(BLOCK_SIZE))

def read(block):
    # print(f"read: {block}, {disk[block]}")
    return LogFsErr.OK, disk[block]


def prog(block, buf):
    # print(f"prog: {block}, {buf}")
    disk[block] = buf
    return LogFsErr.OK


def erase(block):
    # print(f"erase: {block}")
    return LogFsErr.OK


class LogFs:
    def __init__(
        self, block_size: int, block_count: int, context: LogFsContext
    ) -> None:
        read_lamda = lambda block: context.read(block)
        prog_lambda = lambda block, data: context.prog(block, data)
        erase_lamda = lambda block: context.erase(block)

        self.cfg = PyLogFsCfg(
            block_size, block_count, read_lamda, prog_lambda, erase
        )
        self.fs = PyLogFs(self.cfg)

    def format(self) -> LogFsErr:
        _check_err(self.fs.format())

    def mount(self) -> LogFsErr:
        _check_err(self.fs.mount())

    def open(self, path: str) -> LogFsErr:
        file = LogFsFile()
        _check_err(self.fs.open(file, path))
        return file

    def read(self, file: LogFsFile, size: int) -> int:
        num_read, data = self.fs.read(file, size)
        return data[:num_read]

    def write(self, file: LogFsFile, data: bytes):
        _check_err(self.fs.write(file, data, len(data)))
