import pytest
from logfs import LogFs
from .conftest import LogFsTestDisk


def test_mount_empty_fails(fs: LogFs, test_disk: LogFsTestDisk) -> None:
    test_disk.corrupt()

    with pytest.raises(Exception):
        fs.mount()
