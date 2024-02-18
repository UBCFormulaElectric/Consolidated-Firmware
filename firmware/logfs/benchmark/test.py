from logfs_lib import LogFsCfg, LogFs, LogFsErr, LogFsFile

BLOCK_COUNT = 100
BLOCK_SIZE = 32

disk = []
for block in range(BLOCK_COUNT):
    disk.append(bytes(BLOCK_SIZE))


def read(block):
    print(f"read: {block}, {disk[block]}")
    return LogFsErr.OK, disk[block]


def prog(block, buf):
    print(f"prog: {block}, {buf}")
    disk[block] = buf
    return LogFsErr.OK


def erase(block):
    print(f"erase: {block}")
    return LogFsErr.OK


cfg = LogFsCfg(BLOCK_SIZE, BLOCK_COUNT, read, prog, erase)
fs = LogFs(cfg)
fs.format()

file = LogFsFile()
fs.open(file, "test.txt")

# file2 = LogFsFile()
# fs.open(file2, "test2.txt")

# data1 = "hello world hello world hello world hello world ".encode()
# data2 = "testo ".encode()
# data3 = "grootings!".encode()

# fs.write(file1, data1, len(data1))
# fs.write(file2, data2, len(data2))
# fs.write(file1, data2, len(data2))
# fs.write(file1, data2, len(data2))

# num_read, data = fs.read(file1, len(data1) + len(data2) * 2)
# print(num_read)
# print(data)

# num_read, data = fs.read(file2, len(data2))
# print(num_read)
# print(data)

# file3 = LogFsFile()
# fs.open(file3, "test3.txt")
# fs.write(file3, data3, len(data3))

# num_read, data = fs.read(file1, len(data1) + len(data2) * 2)
# print(num_read)
# print(data)

# num_read, data = fs.read(file2, len(data2))
# print(num_read)
# print(data)

# num_read, data = fs.read(file3, len(data3))
# print(num_read)
# print(data)

# fs.write(file1, data1, len(data1))
