from logfs_src import LogFsCfg, LogFs, LogFsErr, LogFsFile
from littlefs import LittleFS, UserContext
import time

N = 1_000_000
BLOCK_COUNT = 125_000_000
BLOCK_SIZE = 512

disk_reads = 0
disk_progs = 0
disk_erases = 0

disk = []
for block in range(BLOCK_COUNT):
    disk.append(bytes(BLOCK_SIZE))


def read(block):
    global disk_reads
    disk_reads += 1
    return LogFsErr.OK, disk[block]


def prog(block, buf):
    global disk_progs
    disk_progs += 1
    disk[block] = buf
    return LogFsErr.OK


def erase(block):
    global disk_erases
    disk_erases += 1
    return LogFsErr.OK


cfg = LogFsCfg(BLOCK_SIZE, BLOCK_COUNT, read, prog, erase)
fs = LogFs(cfg)
fs.format()

file = LogFsFile()
file2 = LogFsFile()
fs.open(file, "test.txt")
# fs.open(file2, "test2.txt")
data = " ".join(["hello world!" for _ in range(N)]).encode()

start_time = time.time()

fs.write(file, data, len(data))

end_time = time.time()
logfs_time = end_time - start_time
print(f"Execution time: {logfs_time} seconds")

read_size, read_data = fs.read(file, len(data))
assert(read_data == data)
print(f"Read size: {read_size / 1e6} Mb")

# fs.write(file2, data, len(data))
# fs.write(file2, data, len(data))
# num_read, data = fs.read(file, len(data))
# print(num_read)
# print(data)
print(disk_reads, disk_progs, disk_erases)


ctx = UserContext(buffsize=BLOCK_SIZE * BLOCK_COUNT)
fs = LittleFS(block_size=BLOCK_SIZE, block_count=BLOCK_COUNT, context=ctx)

start_time = time.time()

# Open a file and write some content
with fs.open("test.txt", "w") as fh:
    fh.write(str(data))


end_time = time.time()
lfs_time = end_time - start_time
print(f"Execution time: {lfs_time} seconds")

print(ctx.reads, ctx.progs, ctx.erases, ctx.syncs)

print(lfs_time / logfs_time)

