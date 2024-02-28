import pytest
from logfs import LogFs, LogFsDummyContext

BLOCK_COUNT = 100
BLOCK_SIZE = 32

fs = LogFs(BLOCK_SIZE, BLOCK_COUNT, LogFsDummyContext(BLOCK_SIZE, BLOCK_COUNT))
fs.format()

file1 = fs.open("/test1.txt")
fs.write(file1, "hello world! grootings!")
file2 = fs.open("/test2.txt")
fs.write(file2, "hello brother! hello brother!")
fs.write(file1, "grootings!")

print(fs.read(file1))
print(fs.read(file2))

print(fs.list_dir())
