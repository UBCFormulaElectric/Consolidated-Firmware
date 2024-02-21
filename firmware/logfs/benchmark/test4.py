from logfs_lib import PyLogFs, LogFsErr, LogFsFile
from typing import Tuple

BLOCK_COUNT = 100
BLOCK_SIZE = 32


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

    def erase(self, block: int):
        # print(f"erase: {block}")
        return LogFsErr.OK


fs = PyLogFs(BLOCK_SIZE, BLOCK_COUNT, LogFsDummyContext(BLOCK_SIZE, BLOCK_COUNT))
fs.format()

data_len = 1
data = " ".join(["hello world!" for _ in range(data_len)]).encode()

# Write data.
file = LogFsFile()
fs.open(file, "test.txt")
fs.write(file, data, len(data))

# Read data back.
read_len, read_data = fs.read(file, len(data))
fs.write(file, data, len(data))
assert read_len == len(data)
assert data == read_data

# Try opening the file again
file = None
file = LogFsFile()
fs.open(file, "test.txt")

read_len, read_data = fs.read(file, len(data))
assert read_len == len(data)
assert data == read_data

fs.write(file, data, len(data))
read_len, read_data = fs.read(file, len(data) * 2)
assert read_len == len(data) * 2
assert data * 2 == read_data

print(fs.mount())
