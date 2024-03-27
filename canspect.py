from logfs import LogFs, LogFsUnixDisk

disk = LogFsUnixDisk(block_size=512, block_count=1000, disk_path="/dev/disk4")
fs = LogFs(block_size=512, block_count=1000, disk=disk, mount=True, rd_only=True)

file = fs.open("/can.txt")
data = fs.read(file)

print(len(data))

print(data)
