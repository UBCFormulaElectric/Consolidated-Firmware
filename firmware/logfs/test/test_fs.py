import pytest
import random
import string
from logfs import LogFs


def random_data(size_bytes: int) -> bytes:
    data = "".join(random.choices(string.ascii_uppercase + string.digits, k=size_bytes))
    data = data.encode()
    return data


@pytest.mark.parametrize(
    "size_bytes",
    [
        1,
        10,
        100,
        1000,
        10_000,
        100_000,
        1_000_000,
    ],
)
def test_rw_big_file(fs: LogFs, size_bytes) -> None:
    # Create dummy data.
    data = random_data(size_bytes=size_bytes)

    # Write data.
    file = fs.open(path="/test.txt", flags="rwx")
    file.write(data=data)

    # Read data back.
    read_data = file.read()
    assert data == read_data


@pytest.mark.parametrize(
    "num_files",
    [
        1,
        5,
        10,
        100,
    ],
)
@pytest.mark.parametrize("data_size", [1, 10, 1000])
def test_rw_multiple_files(
    fs: LogFs,
    num_files: int,
    data_size: int,
) -> None:
    file_names = [f"/test{i}.txt".encode() for i in range(num_files)]
    file_data = [random_data(size_bytes=data_size) for i in range(num_files)]
    file_handles = [fs.open(path=name, flags="rwx") for name in file_names]
    files = zip(file_names, file_data, file_handles)

    # Write multiple files.
    for _, data, handle in files:
        handle.write(data=data)

    # Read multiple files.
    for _, data, handle in files:
        read_data = handle.read()
        assert read_data == data

    # Repeat for redundancy.
    for _, data, handle in files:
        handle.write(data=data)

    for _, data, handle in files:
        read_data = handle.read()
        assert read_data == handle * 2

    # And once more...
    for _, data, handle in files:
        handle.write(data=data)

    for _, data, handle in files:
        read_data = handle.read()
        assert read_data == handle * 3


@pytest.mark.parametrize("data_size", [1, 10, 100, 1000, 10_000])
def test_open_existing(fs: LogFs, data_size: int) -> None:
    fs.format()
    fs.mount()

    # Create dummy data.
    file_name = "/test.txt"
    data1 = random_data(size_bytes=data_size)
    data2 = random_data(size_bytes=data_size)

    # Write data and read data back.
    handle = fs.open(path=file_name, flags="rwx")
    handle.write(data=data1)
    read_data = handle.read()
    assert read_data == data1
    handle.close()

    # Try opening the file again, and read data back.
    del handle
    new_handle = fs.open(path=file_name, flags="rw")
    read_data = new_handle.read()
    assert read_data == data1

    # Try modifying the file.
    new_handle.write(data=data2)
    read_data = new_handle.read()
    assert read_data == data1 + data2
    new_handle.close()

    # Try opening/reading one more time.
    del new_handle
    newest_handle = fs.open(path=file_name, flags="r")
    read_data = newest_handle.read()
    assert read_data == data1 + data2


@pytest.mark.parametrize("data_size", [1, 10, 100, 1000, 10_000])
def test_mount(fs: LogFs, data_size: int) -> None:
    # Create dummy data.
    data = random_data(size_bytes=data_size)

    # Write data.
    file = fs.open(path="/test.txt", flags="rwx")
    file.write(data=data)

    # Read data back.
    read_data = file.read()
    assert read_data == data
    file.close()

    # Re-mount filesystem.
    del file
    fs.mount()

    # Read data back.
    file = fs.open(path="/test.txt", flags="rw")
    read_data = file.read()
    assert read_data == data

    # Try writing data again.
    file.write(data=data)
    read_data = file.read()
    assert read_data == data + data


@pytest.mark.parametrize("data_size", [1, 10, 100, 1000, 10_000])
def test_read_entire_file_iter(fs: LogFs, data_size: int) -> None:
    # Create dummy data.
    data = random_data(size_bytes=data_size)

    # Write data.
    file = fs.open(path="/test.txt", flags="rwx")
    file.write(data=data)

    # Read data back.
    read_data = file.read()
    assert data == read_data


def test_list_dir(fs: LogFs) -> None:
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
        fs.open(path=file, flags="wx")

    assert fs.list_dir() == files
    assert fs.list_dir(matches="/dir1") == ["/dir1/test3.txt", "/dir1/test4.txt"]
    assert fs.list_dir(matches="/dir2") == [
        "/dir2/dir3/test5.txt",
        "/dir2/dir3/test6.txt",
    ]
    assert fs.list_dir(matches="/dir4/dir5/dir6") == ["/dir4/dir5/dir6/test7.txt"]
    assert fs.list_dir(matches="/test8.txt") == ["/test8.txt"]
    assert fs.list_dir(matches="mismatch") == []


@pytest.mark.parametrize("metadata_size", [1, 10, 100])
def test_metadata(fs: LogFs, metadata_size: int) -> None:
    file = fs.open(path="/test.txt", flags="rwx")

    # Metadata should start empty.
    assert file.read_metadata() == b""

    # R/W metadata.
    data = random_data(size_bytes=metadata_size)
    file.write_metadata(data=data)
    assert file.read_metadata() == data

    # Metadata is independent of data on file.
    file_data = b"Hello world!"
    file.write(data=file_data)
    assert file.read() == file_data
    assert file.read_metadata() == data

    # Data should persist after reopening the file.
    file.close()
    del file

    file = fs.open(path="/test.txt", flags="r")
    assert file.read() == file_data
    assert file.read_metadata() == data
    