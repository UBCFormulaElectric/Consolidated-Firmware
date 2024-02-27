from logfs.fs import LogFs
from logfs.context import LogFsDummyContext

BLOCK_COUNT = 100
BLOCK_SIZE = 32

fs = LogFs(BLOCK_SIZE, BLOCK_COUNT, LogFsDummyContext(BLOCK_SIZE, BLOCK_COUNT))
fs.format()

data1 = "hello world".encode()
file1 = fs.open("test1.txt")
fs.write(file1, data1)
print(fs.read(file1))
