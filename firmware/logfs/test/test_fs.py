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

    def erase(block):
        disk[block] = [bytes(BLOCK_SIZE)]
        return LogFsErr.OK

    cfg = LogFsCfg(BLOCK_SIZE, BLOCK_COUNT, read, prog, erase)
    return cfg


@pytest.fixture
def fs_unformatted(fs_cfg: LogFsCfg) -> LogFs:
    fs = LogFs(fs_cfg)
    return fs


@pytest.fixture
def fs(fs_unformatted: LogFs) -> LogFs:
    fs_unformatted.format()
    return fs_unformatted


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


def test_open_existing(fs):
    # Create dummy data.
    data1 = "hello world!".encode()
    data2 = "test!".encode()

    # Write data.
    handle = LogFsFile()
    fs.open(handle, "test.txt")
    fs.write(handle, data1, len(data1))

    # Read data back.
    read_len, read_data = fs.read(handle, len(data1))
    assert read_len == len(data1)
    assert read_data == data1

    # Try opening the file again
    new_handle = LogFsFile()
    fs.open(new_handle, "test.txt")

    # Read data back.
    read_len, read_data = fs.read(new_handle, len(data1))
    assert read_len == len(data1)
    assert read_data == data1

    # Try modifying the file.
    fs.write(new_handle, data2, len(data2))
    read_len, read_data = fs.read(new_handle, len(data1) + len(data2))
    assert read_len == len(data1) + len(data2)
    assert read_data == data1 + data2

    # Try opening/reading one more time.
    newest_handle = LogFsFile()
    fs.open(newest_handle, "test.txt")
    read_len, read_data = fs.read(newest_handle, len(data1) + len(data2))
    assert read_len == len(data1) + len(data2)
    assert read_data == data1 + data2


def test_mount_empty_fails(fs_unformatted):
    assert fs_unformatted.mount() == LogFsErr.CORRUPT


def test_mount(fs, fs_cfg):
    # Create dummy data.
    data = "hello world!".encode()

    # Write data.
    handle = LogFsFile()
    fs.open(handle, "test.txt")
    fs.write(handle, data, len(data))

    # Read data back.
    read_len, read_data = fs.read(handle, len(data))
    assert read_len == len(data)
    assert read_data == data

    # Re-mount filesystem.
    fs = None
    fs_mounted = LogFs(fs_cfg)
    fs_mounted.mount()

    # Read data back.
    read_len, read_data = fs_mounted.read(handle, len(data))
    assert read_len == len(data)
    assert read_data == data
