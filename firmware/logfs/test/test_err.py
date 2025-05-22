import pytest
from logfs import LogFs, LogFsDisk, LogFsRamDisk, LogFsUnixDisk, LogFsError
from .conftest import LogFsTestDisk, random_data

# TODO: Add "recovery" tests: Tests that check that if, for ex, writes to a
# block fail then the filesystem doesn't get corrupted.

SMALL_BLOCK_COUNT = 11
SMALL_WRITE_CYCLES = 10


@pytest.fixture
def fs_small(disk: LogFsDisk) -> LogFs:
    return LogFs(
        block_size=512,
        block_count=SMALL_BLOCK_COUNT,
        write_cycles=SMALL_WRITE_CYCLES,
        rd_only=False,
        disk=disk,
        format=True,
    )


def test_mount_empty_fails(fs: LogFs, test_disk: LogFsTestDisk) -> None:
    test_disk.corrupt()

    with pytest.raises(Exception):
        fs.mount()

    fs.format()
    fs.mount()


def test_nomem_on_data_write(fs_small: LogFs):
    # A file takes up 5 blocks minimum (2 for the file pair, 2 for the metadata
    # pair, and 1 block of data). The amount of data that can be stored per
    # block is a bit less than the block size (some is reserved for a checksum,
    # etc.). There is also a hidden .root file at the origin of the filesystem.
    # This means that creating a single empty file creates a filesystem that
    # occupies 10 blocks. If we fill up the file's data block and request an
    # 11th, the filesystem will be full, and we should get a NOMEM error.
    #
    # Note that the filesystem generates a NOMEM error when all blocks are
    # occupied, not when all blocks are exceeded (for ex, a NOMEM error is
    # generated when you try to write to 11/11 blocks, not 12/11). This is
    # because the range check happens when incrementing the head - if we, for
    # ex, write to 11/11 blocks, the head will be moved to 12, which is out of
    # bounds. Technically this reduces our capacity by 1 block but since blocks
    # are typically on the scale of >1 million, this is fine, and makes
    # implementation easier.

    file = fs_small.open(path="/test.txt", flags="rwx")
    data = random_data(size_bytes=256)

    file.write(data=data)

    with pytest.raises(LogFsError):
        # File's data block fills up so another block is requested, which
        # should raise a NOMEM since the filesystem will be full.
        file.write(data=data)


def test_nomem_on_create_file(fs_small: LogFs):
    fs_small.open(path="/test.txt", flags="rwx")

    with pytest.raises(LogFsError):
        # Not enough space for a new file.
        fs_small.open(path="/test2.txt", flags="rwx")


def test_nomem_on_replace(fs_small: LogFs):
    # Metadata is stored in a pair, with writes ping-ponged between the two
    # pair blocks, so pairs can handle 2 * write_cycles writes before
    # replacement. There is a single write done when creating a file, so we
    # can do 2 * write_cycles - 1 more writes to metadata, and the next will
    # trigger a replacement. For this small filesystem, there isn't enough
    # memory for the replacement so we expect a NOMEM error.

    file = fs_small.open(path="/test.txt", flags="rwx")
    data = random_data(size_bytes=256)

    for _ in range(2 * SMALL_WRITE_CYCLES - 1):
        file.write_metadata(data)

    with pytest.raises(LogFsError):
        file.write_metadata(data)
