from typing import Union, Optional
from logfs.context import LogFsContext
from logfs_src import LogFsErr, PyLogFs, LogFsFile, PyLogFsRead, PyLogFsWrite


class LogFsError(Exception):
    def __init__(self, err: LogFsErr):
        super().__init__()
        self.err = err

    def __repr__(self) -> str:
        return f"<{self.__class__.__name__}({self.err})>"

    def __str__(self) -> str:
        return f"LogFsErr: {self.err}"


class LogFs:
    READ_ITER_CHUNK_SIZE = 32

    def __init__(
        self, block_size: int, block_count: int, context: LogFsContext
    ) -> None:
        self.fs = PyLogFs(block_size, block_count, context)

    def format(self) -> None:
        self._check_err(self.fs.format())

    def mount(self) -> None:
        self._check_err(self.fs.mount())

    def open(self, path: str) -> None:
        file = LogFsFile()
        self._check_err(self.fs.open(file, path))
        return file

    def read(self, file: LogFsFile, size: Optional[int] = None) -> int:
        if size is None:
            file_data = b""
            file_num_read = 0
            
            self.fs.read(file, 0, PyLogFsRead.START) # Reset file read iterator

            while True:
                num_read, data = self.fs.read(
                    file, self.READ_ITER_CHUNK_SIZE, PyLogFsRead.ITER
                )
                if num_read == 0:
                    break

                file_data += data
                file_num_read += num_read

            return file_data[:file_num_read]
        else:
            num_read, data = self.fs.read(file, size, PyLogFsRead.START)
            return data[:num_read]

    def write(self, file: LogFsFile, data: Union[bytes, str]) -> None:
        if isinstance(data, str):
            data = data.encode("utf-8")

        self._check_err(self.fs.write(file, data, len(data), PyLogFsWrite.APPEND))

    def _check_err(self, err: LogFsErr) -> None:
        if err != LogFsErr.OK:
            raise LogFsError(err)
