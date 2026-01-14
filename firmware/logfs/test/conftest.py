from typing import Optional
import pytest
from logfs import LogFs, LogFsDisk, LogFsRamDisk, LogFsUnixDisk
import random
import string

FS_SIZE_BYTES = 2 * 1024 * 1024  # 1 MB


class LogFsTestDisk:
    def __init__(self, disk: LogFsDisk) -> None:
        self.disk = disk

    def corrupt(self, block: Optional[int] = None) -> None:
        if block is None:
            self.corrupt(block=0)
            self.corrupt(block=1)
        else:
            self.disk.write(block, bytes([0xAB] * self.disk.block_size))


def pytest_addoption(parser: pytest.Parser) -> None:
    parser.addoption(
        "--disk",
        action="store",
        default=None,
        required=False,
        type=str,
        help="Path to block device to run test suite on (will format the disk!).",
    )


@pytest.fixture(params=[128, 512, 1024])
def block_size(request: pytest.FixtureRequest) -> int:
    if request.config.getoption("disk") is not None:
        # If we're testing on a physical disk, need to use 512 byte block size.
        return 512
    else:
        # We only ever use this on an SD card with 512 byte block size, but
        # testing with different sizes will hopefully catch some bugs in the
        # filesystem.
        return request.param


@pytest.fixture
def block_count(block_size: int) -> int:
    return FS_SIZE_BYTES // block_size


@pytest.fixture(params=[0, 1, 10, 100])
def write_cycles(request: pytest.FixtureRequest) -> int:
    return request.param


@pytest.fixture
def disk(
    request: pytest.FixtureRequest, block_size: int, block_count: int
) -> LogFsDisk:
    if disk := request.config.getoption("disk"):
        return LogFsUnixDisk(
            block_size=block_size, block_count=block_count, disk_path=disk
        )
    else:
        return LogFsRamDisk(
            block_size=block_size, block_count=FS_SIZE_BYTES // block_size
        )


@pytest.fixture
def test_disk(disk: LogFsDisk) -> LogFsTestDisk:
    return LogFsTestDisk(disk=disk)


@pytest.fixture
def fs(disk: LogFsDisk, block_size: int, block_count: int, write_cycles: int) -> LogFs:
    return LogFs(
        block_size=block_size,
        block_count=block_count,
        write_cycles=write_cycles,
        rd_only=False,
        disk=disk,
        format=True,
    )


def random_data(size_bytes: int) -> bytes:
    data = "".join(random.choices(string.ascii_uppercase + string.digits, k=size_bytes))
    data = data.encode()
    return data
