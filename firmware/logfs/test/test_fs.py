import pytest
from logfs import LogFs, LogFsDummyContext


BLOCK_COUNT = 100000
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
    file = fs.open("/test.txt")
    fs.write(file, data)

    # Read data back.
    read_data = fs.read(file)

    print("lets go!\n")
    print(read_data)
    print(data)
    assert data == read_data


def test_rw_multiple_files(fs):
    file_names = [
        b"/test1.txt",
        b"/test2.txt",
        b"/test3.txt",
        b"/test_with_very_very_long_name.txt",
    ]
    files = [(name, fs.open(name)) for name in file_names]

    print("whattt?")

    # Write multiple files.
    for file_name, file in files:
        fs.write(file, file_name)

    # Read multiple files.
    for file_name, file in files:
        print("what?")
        read_data = fs.read(file)
        assert read_data == file_name

    # Repeat for redundancy.
    for file_name, file in files:
        fs.write(file, file_name)

    for file_name, file in files:
        read_data = fs.read(file)
        assert read_data == file_name * 2


def test_open_existing(fs):
    # Create dummy data.
    file_name = "/test.txt"
    data1 = "hello world!".encode()
    data2 = "test!".encode()

    # Write data and read data back.
    handle = fs.open(file_name)
    fs.write(handle, data1)
    read_data = fs.read(handle)
    assert read_data == data1
    fs.close(handle)

    # Try opening the file again, and read data back.
    del handle
    new_handle = fs.open(file_name)
    read_data = fs.read(new_handle)
    assert read_data == data1

    # Try modifying the file.
    fs.write(new_handle, data2)
    read_data = fs.read(new_handle)
    assert read_data == data1 + data2
    fs.close(new_handle)

    # Try opening/reading one more time.
    del new_handle
    newest_handle = fs.open(file_name)
    read_data = fs.read(newest_handle)
    assert read_data == data1 + data2


def test_mount_empty_fails(fs_unformatted):
    with pytest.raises(Exception):
        fs_unformatted.mount()


def test_mount(fs, fs_unformatted):
    # Create dummy data.
    data = "hello world!".encode()

    # Write data.
    file = fs.open("/test.txt")
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

    # Try writing data again.
    fs_unformatted.write(file, data)
    read_data = fs_unformatted.read(file)
    assert read_data == data + data


def test_read_entire_file_iter(fs):
    # Create dummy data.
    data_len = 10_000
    data = " ".join(["hello world!" for _ in range(data_len)]).encode()

    # Write data.
    file = fs.open("/test.txt")
    fs.write(file, data)

    # Read data back.
    read_data = fs.read(file)
    assert data == read_data


def test_list_dir(fs):
    files = [
        "/test1.txt",
        "/test2.txt",
        "/dir1/test3.txt",
        "/dir1/test4.txt",
        "/dir2/dir3/test5.txt",
        "/dir2/dir3/test6.txt",
        "/dir4/dir5/dir6/test7.txt",
        "/test8.txt",
    ]
    for file in files:
        fs.open(file)

    assert fs.list_dir() == files
    assert fs.list_dir(file="/dir1") == ["/dir1/test3.txt", "/dir1/test4.txt"]
    assert fs.list_dir(file="/dir2") == ["/dir2/dir3/test5.txt", "/dir2/dir3/test6.txt"]
    assert fs.list_dir(file="/dir4/dir5/dir6") == ["/dir4/dir5/dir6/test7.txt"]
    assert fs.list_dir(file="/test8.txt") == ["/test8.txt"]
    assert fs.list_dir(file="mismatch") == []
