from logfs import LogFs, LogFsUnixDisk
import struct

disk = LogFsUnixDisk(block_size=512, block_count=1000, disk_path="/dev/disk4")
fs = LogFs(block_size=512, block_count=1000, disk=disk, mount=True, rd_only=True)

file = fs.open("/bootcount.txt")
data = fs.read_metadata(file)

bc = struct.unpack("<L", data)[0]
print(f"Boot count: {bc}")
