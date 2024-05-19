from littlefs import LittleFS, UserContextWinDisk

# number = -1
# with open("2", "rb") as f:
#     data = f.read(32)
#     # take first 4 bytes
#     new_number = int.from_bytes(data[:4], byteorder='little')
#     print(new_number)
#     if(number + 1 != new_number):
#         print("worng number")
#     number = new_number

disk_path = r"\\.\D: "  # assume F drive is your storage device for littlefs

bc_16g = 30228480
bc_2g = bc_16g // 8
fs = LittleFS(
    block_size=512,
    block_count=1000000,
    mount=False,
    context=UserContextWinDisk(disk_path),
)


# fs.format()
fs.mount()

files = fs.listdir("/")
for file in files:
    with fs.open(file, "rb") as fh:
        with open(file, "wb+") as f:
            data = fh.read()
            print(data)
            f.write(data)
