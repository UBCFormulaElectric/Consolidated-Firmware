from typing import Optional
import pytest
from logfs import LogFs, LogFsDisk, LogFsRamDisk, LogFsUnixDisk


SD_BLOCK_SIZE = 512
FS_SIZE_BYTES = 2 * 1024 * 1024  # 1 MB
FS_WRITE_CYCLES = 0


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
    parser.addoption(
        "--block_size",
        action="store",
        default=None,
        required=False,
        type=int,
        help="Path to block device to run test suite on (will format the disk!).",
    )


@pytest.fixture
def block_size(request: pytest.FixtureRequest) -> int:
    if request.config.getoption("disk") is not None:
        # If we're testing on a physical disk, need to use 512 byte block size.
        return SD_BLOCK_SIZE
    elif block_size := request.config.getoption("block_size"):
        return block_size
    else:
        # Default to 512 bytes
        return SD_BLOCK_SIZE


@pytest.fixture
def block_count(block_size: int) -> int:
    return FS_SIZE_BYTES // block_size


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
def fs(disk: LogFsDisk, block_size: int, block_count: int) -> LogFs:
    return LogFs(
        block_size=block_size,
        block_count=block_count,
        write_cycles=FS_WRITE_CYCLES,
        rd_only=False,
        disk=disk,
        format=True,
    )
