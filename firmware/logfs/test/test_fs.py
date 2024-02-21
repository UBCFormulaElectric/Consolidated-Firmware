import pytest
from logfs.fs import LogFs
from logfs.context import LogFsDummyContext


BLOCK_COUNT = 10000
BLOCK_SIZE = 512


@pytest.fixture
def fs_unformatted() -> LogFs:
    fs = LogFs(BLOCK_SIZE, BLOCK_COUNT, LogFsDummyContext(BLOCK_SIZE, BLOCK_COUNT))
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
    file = fs.open("test.txt")
    fs.write(file, data)

    # Read data back.
    read_data = fs.read(file, len(data))
    assert data == read_data


def test_rw_multiple_files(fs):
    file_names = [b"test1.txt", b"test2.txt", b"test3.txt", b"test_with_long_name.txt"]
    files = [(name, fs.open(name)) for name in file_names]

    # Write multiple files.
    for file_name, file in files:
        fs.write(file, file_name)

    # Read multiple files.
    for file_name, file in files:
        read_data = fs.read(file, len(file_name))
        assert read_data == file_name

    # Repeat for redundancy.
    for file_name, file in files:
        fs.write(file, file_name)

    for file_name, file in files:
        read_data = fs.read(file, len(file_name) * 2)
        assert read_data == file_name * 2


def test_open_existing(fs):
    # Create dummy data.
    file_name = "test.txt"
    data1 = "hello world!".encode()
    data2 = "test!".encode()

    # Write data and read data back.
    handle = fs.open(file_name)
    fs.write(handle, data1)
    read_data = fs.read(handle, len(data1))
    assert read_data == data1

    # Try opening the file again, and read data back.
    del handle
    new_handle = fs.open(file_name)
    read_data = fs.read(new_handle, len(data1))
    assert read_data == data1

    # Try modifying the file.
    fs.write(new_handle, data2)
    read_data = fs.read(new_handle, len(data1) + len(data2))
    assert read_data == data1 + data2

    # Try opening/reading one more time.
    del new_handle
    newest_handle = fs.open(file_name)
    read_data = fs.read(newest_handle, len(data1) + len(data2))
    assert read_data == data1 + data2


def test_mount_empty_fails(fs_unformatted):
    with pytest.raises(Exception):
        fs_unformatted.mount()


def test_mount(fs, fs_unformatted):
    # Create dummy data.
    data = "hello world!".encode()

    # Write data.
    file = fs.open("test.txt")
    fs.write(file, data)

    # Read data back.
    read_data = fs.read(file, len(data))
    assert read_data == data

    # Re-mount filesystem.
    del fs
    fs_unformatted.mount()

    # Read data back.
    read_data = fs_unformatted.read(file, len(data))
    assert read_data == data


def test_read_entire_file_iter(fs):
    # Create dummy data.
    data_len = 10_000
    data = " ".join(["hello world!" for _ in range(data_len)]).encode()

    # Write data.
    file = fs.open("test.txt")
    fs.write(file, data)

    # Read data back.
    read_data = fs.read(file)
    assert data == read_data
