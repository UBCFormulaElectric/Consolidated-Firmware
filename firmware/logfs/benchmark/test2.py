from logfs import LogFs, LogFsDummyContext, LogFsErr
from logfs_src import PyLogFs, PyLogFsCfg

BLOCK_COUNT = 100
BLOCK_SIZE = 32

fs = LogFs(BLOCK_SIZE, BLOCK_COUNT, LogFsDummyContext(BLOCK_SIZE, BLOCK_COUNT))
fs.format()

data1 = "hello world".encode()
file1 = fs.open("test1.txt")
print(fs.read(file1, len(data1)))
