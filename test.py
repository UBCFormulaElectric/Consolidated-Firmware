from logfs import LogFs, LogFsDummyContext

BLOCK_SIZE = 32
BLOCK_COUNT = 1024
context = LogFsDummyContext(block_size=BLOCK_SIZE, block_count=BLOCK_COUNT)
fs = LogFs(block_size=BLOCK_SIZE, block_count=BLOCK_COUNT, context=context)
fs.format()

# file = fs.open("/test.txt")
# fs.write(file, b"hello world! hello world! hello world!")

# print(fs.read(file))

# # Create dummy data.
# data = "hello world!".encode()

# # Write data.
# file1 = fs.open("/test1.txt")
# file2 = fs.open("/test2.txt")
# fs.write(file1, data)
# fs.write(file, data)

# # Read data back.
# read_data = fs.read(file, len(data))
# assert read_data == data

# # Re-mount filesystem.
# del fs

# fs = LogFs(block_size=BLOCK_SIZE, block_count=BLOCK_COUNT, context=context)
# fs.mount()

# # Read data back.
# read_data = fs.read(file, len(data))
# assert read_data == data

# # Try writing data again.
# fs.write(file, data)
# read_data = fs.read(file)

# print(read_data)
# # assert read_data == data + data

# files = [
#     "/test1.txt",
#     # "/test2.txt",
#     # "/dir1/test3.txt",
#     # "/dir1/test4.txt",
#     # "/dir2/dir3/test5.txt",
#     # "/dir2/dir3/test6.txt",
#     # "/dir4/dir5/dir6/test7.txt",
#     # "/test8.txt",
# ]
# fs.open("/test1.txt")

# print(fs.list_dir())

# assert fs.list_dir() == files
# assert fs.list_dir(file="/dir1") == ["/dir1/test3.txt", "/dir1/test4.txt"]
# assert fs.list_dir(file="/dir2") == ["/dir2/dir3/test5.txt", "/dir2/dir3/test6.txt"]
# assert fs.list_dir(file="/dir4/dir5/dir6") == ["/dir4/dir5/dir6/test7.txt"]
# assert fs.list_dir(file="/test8.txt") == ["/test8.txt"]
# assert fs.list_dir(file="mismatch") == []


file_names = [
    b"/test1.txt",
    b"/test2.txt",
    b"/test3.txt",
    # b"/test_with_very_very_long_name.txt",
]
files = [(name, fs.open(name)) for name in file_names]

# Write multiple files.
# for file_name, file in files:
#     print("what2?")
#     fs.close(file)

# Write multiple files.
for file_name, file in files:
    fs.write(file, file_name)

# for file_name, file in files:
#     fs.sync(file=file)

# # Read multiple files.
for file_name, file in files:
    read_data = fs.read(file)
    print("read: ", read_data)

# # Repeat for redundancy.
# for file_name, file in files:
#     fs.write(file, file_name)

# for file_name, file in files:
#     read_data = fs.read(file)
#     assert read_data == file_name * 2
