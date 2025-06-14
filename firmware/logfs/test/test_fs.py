import pytest
from logfs import LogFs
from .conftest import random_data


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

    # Create file.
    file = fs.open(path="/test.txt", flags="rwx")
    assert file.read() == b""
    assert file.size() == 0

    # Write data.
    file.write(data=data)

    # Read data back.
    assert file.read() == data
    assert file.size() == size_bytes


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
    files = list(zip(file_names, file_data, file_handles))

    # Write multiple files.
    for _, data, handle in files:
        assert handle.read() == b""
        assert handle.size() == 0
        handle.write(data=data)

    # Read multiple files.
    for _, data, handle in files:
        assert handle.read() == data
        assert handle.size() == len(data)

    # Repeat for redundancy.
    for _, data, handle in files:
        handle.write(data=data)

    for _, data, handle in files:
        assert handle.read() == data * 2
        assert handle.size() == len(data) * 2

    # And once more...
    for _, data, handle in files:
        handle.write(data=data)

    for _, data, handle in files:
        assert handle.read() == data * 3
        assert handle.size() == len(data) * 3


@pytest.mark.parametrize("data_size", [1, 10, 100, 1000, 10_000])
def test_open_existing(fs: LogFs, data_size: int) -> None:
    fs.format()
    fs.mount()

    # Create dummy data.
    file_name = "/test.txt"
    data1 = random_data(size_bytes=data_size)
    data2 = random_data(size_bytes=data_size)

    # Create file.
    handle = fs.open(path=file_name, flags="rwx")
    assert handle.read() == b""
    assert handle.size() == 0

    # Write data and read data back.
    handle.write(data=data1)
    assert handle.read() == data1
    assert handle.size() == data_size
    handle.close()

    # Try opening the file again, and read data back.
    del handle
    new_handle = fs.open(path=file_name, flags="rw")
    assert new_handle.read() == data1
    assert new_handle.size() == data_size

    # Try modifying the file.
    new_handle.write(data=data2)
    assert new_handle.read() == data1 + data2
    assert new_handle.size() == 2 * data_size
    new_handle.close()

    # Try opening/reading one more time.
    del new_handle
    newest_handle = fs.open(path=file_name, flags="r")
    assert newest_handle.read() == data1 + data2
    assert newest_handle.size() == 2 * data_size


@pytest.mark.parametrize("data_size", [1, 10, 100, 1000, 10_000])
def test_mount(fs: LogFs, data_size: int) -> None:
    # Create dummy data.
    data = random_data(size_bytes=data_size)

    # Write data.
    file = fs.open(path="/test.txt", flags="rwx")
    file.write(data=data)

    # Read data back.
    assert file.read() == data
    assert file.size() == data_size
    file.close()

    # Re-mount filesystem.
    del file
    fs.mount()

    # Read data back.
    file = fs.open(path="/test.txt", flags="rw")
    assert file.read() == data
    assert file.size() == data_size

    # Try writing data again.
    file.write(data=data)
    assert file.read() == data + data
    assert file.size() == data_size * 2


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

    assert list(fs.list_dir().keys()) == files
    assert list(fs.list_dir(matches="/dir1").keys()) == [
        "/dir1/test3.txt",
        "/dir1/test4.txt",
    ]
    assert list(fs.list_dir(matches="/dir2").keys()) == [
        "/dir2/dir3/test5.txt",
        "/dir2/dir3/test6.txt",
    ]
    assert list(fs.list_dir(matches="/dir4/dir5/dir6").keys()) == [
        "/dir4/dir5/dir6/test7.txt"
    ]
    assert list(fs.list_dir(matches="/test8.txt").keys()) == ["/test8.txt"]
    assert list(fs.list_dir(matches="mismatch").keys()) == []


@pytest.mark.parametrize("metadata_size", [1, 10, 100])
def test_metadata(fs: LogFs, metadata_size: int) -> None:
    file = fs.open(path="/test.txt", flags="rwx")

    # Metadata should start empty.
    assert file.read_metadata() == b""
    assert file.metadata_size() == 0

    # R/W metadata.
    data = random_data(size_bytes=metadata_size)
    file.write_metadata(data=data)
    assert file.read_metadata() == data

    # Metadata is independent of data on file.
    file_data = b"Hello world!"
    file.write(data=file_data)
    assert file.read() == file_data
    assert file.read_metadata() == data
    assert file.metadata_size() == metadata_size

    # Data should persist after reopening the file.
    file.close()
    del file

    file = fs.open(path="/test.txt", flags="r")
    assert file.read() == file_data
    assert file.read_metadata() == data
    assert file.metadata_size() == metadata_size


def test_list_dir_sizes(fs: LogFs, capsys: pytest.CaptureFixture) -> None:
    files = [
        ("/test1.txt", b"Hello, world!", ""),
        ("/test2.txt", b"", "Let's get this money"),
        ("/test3.txt", b"UBC For", "mula Electric"),
        ("/test4.txt", b"", b""),
        ("/test5.txt", bytes([0xAB] * 10_000), b""),
    ]
    for file_name, file_data, file_metadata in files:
        file = fs.open(path=file_name, flags="wrx")
        file.write(file_data)
        file.write_metadata(file_metadata)
        file.close()

    assert fs.list_dir() == {
        "/test1.txt": {"size": 13, "metadata_size": 0},
        "/test2.txt": {"size": 0, "metadata_size": 20},
        "/test3.txt": {"size": 7, "metadata_size": 13},
        "/test4.txt": {"size": 0, "metadata_size": 0},
        "/test5.txt": {"size": 10_000, "metadata_size": 0},
    }

    fs.list_dir_table()
    captured = capsys.readouterr()
    assert (
        captured.out.strip()
        == """\
+------------+----------+----------+
|    File    | Metadata |   Data   |
+------------+----------+----------+
| /test1.txt | 0 Bytes  | 13 Bytes |
| /test2.txt | 20 Bytes | 0 Bytes  |
| /test3.txt | 13 Bytes | 7 Bytes  |
| /test4.txt | 0 Bytes  | 0 Bytes  |
| /test5.txt | 0 Bytes  | 9.8 KiB  |
+------------+----------+----------+"""
    )
