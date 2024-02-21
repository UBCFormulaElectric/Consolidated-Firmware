from logfs import LogFs, LogFsUnixContext
    
context = LogFsUnixContext(
    block_size=512, block_count=1024, disk_path="/dev/disk4"
)
fs = LogFs(
    block_size=512, block_count=1024, context=context
)
fs.mount()

file = fs.open("test.txt")
# fs.write(file, b"hello world!")

print("bruh!")
print(fs.read(file, 12))

