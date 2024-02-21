from typing import Tuple, Union
from logfs.context import LogFsContext
from logfs_src import LogFsErr, PyLogFs, LogFsFile


class LogFsError(Exception):
    def __init__(self, err: LogFsErr):
        super().__init__()
        self.err = err

    def __repr__(self) -> str:
        return f"<{self.__class__.__name__}({self.err})>"

    def __str__(self) -> str:
        return f"LogFsErr: {self.err}"


def _check_err(err: LogFsErr):
    if err != LogFsErr.OK:
        raise LogFsError(err)


class LogFs:
    def __init__(
        self, block_size: int, block_count: int, context: LogFsContext
    ) -> None:
        self.fs = PyLogFs(block_size, block_count, context)

    def format(self) -> None:
        _check_err(self.fs.format())

    def mount(self) -> None:
        _check_err(self.fs.mount())

    def open(self, path: str) -> None:
        file = LogFsFile()
        _check_err(self.fs.open(file, path))
        return file

    def read(self, file: LogFsFile, size: int) -> int:
        num_read, data = self.fs.read(file, size)
        return data[:num_read]

    def write(self, file: LogFsFile, data: Union[bytes, str]) -> None:
        if isinstance(data, str):
            data = data.encode("utf-8")

        _check_err(self.fs.write(file, data, len(data)))
