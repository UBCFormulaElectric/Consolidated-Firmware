from logfs_lib import LogFsCfg, LogFs, LogFsErr, LogFsFile

BLOCK_COUNT = 100
BLOCK_SIZE = 32

disk = []
for block in range(BLOCK_COUNT):
    disk.append(bytes(BLOCK_SIZE))


def read(block):
    # print(f"read: {block}, {disk[block]}")
    return LogFsErr.OK, disk[block]


def prog(block, buf):
    print(f"prog: {block}, {buf}")
    disk[block] = buf
    return LogFsErr.OK


def erase(block):
    # print(f"erase: {block}")
    return LogFsErr.OK


cfg = LogFsCfg(BLOCK_SIZE, BLOCK_COUNT, read, prog, erase)
fs = LogFs(cfg)
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
assert data * 2== read_data

print(fs.mount())