import pytest
from logfs_lib import LogFsCfg, LogFs, LogFsErr, LogFsFile


BLOCK_COUNT = 10000
BLOCK_SIZE = 512


@pytest.fixture
def fs_cfg() -> LogFsCfg:
    disk = []
    for _ in range(BLOCK_COUNT):
        disk.append(bytes(BLOCK_SIZE))

    def read(block):
        return LogFsErr.OK, disk[block]

    def prog(block, buf):
        disk[block] = buf
        return LogFsErr.OK

    def erase(_):
        return LogFsErr.OK

    cfg = LogFsCfg(BLOCK_SIZE, BLOCK_COUNT, read, prog, erase)
    return cfg


@pytest.fixture
def fs(fs_cfg: LogFsCfg) -> LogFs:
    fs = LogFs(fs_cfg)
    fs.format()
    return fs


def test_rw_big_file(fs):
    # Create dummy data.
    data_len = 10_000
    data = " ".join(["hello world!" for _ in range(data_len)]).encode()

    # Write data.
    file = LogFsFile()
    fs.open(file, "test.txt")
    fs.write(file, data, len(data))

    # Read data back.
    read_len, read_data = fs.read(file, len(data))

    assert read_len == len(data)
    assert data == read_data


def test_rw_multiple_files(fs):
    file_names = [b"test1.txt", b"test2.txt", b"test3.txt", b"test_with_long_name.txt"]
    files = [(name, LogFsFile()) for name in file_names]

    # Write multiple files.
    for file_name, file in files:
        fs.open(file, file_name)
        fs.write(file, file_name, len(file_name))

    # Read multiple files.
    for file_name, file in files:
        read_len, read_data = fs.read(file, len(file_name))
        assert read_len == len(file_name)
        assert read_data == file_name

    # Repeat for redundancy.
    for file_name, file in files:
        fs.write(file, file_name, len(file_name))

    for file_name, file in files:
        read_len, read_data = fs.read(file, len(file_name) * 2)
        assert read_len == len(file_name) * 2
        assert read_data == file_name * 2
