from typing import Union, Optional
from .context import LogFsContext
from logfs_src import LogFsErr, PyLogFs, LogFsFile, PyLogFsRead


class LogFsError(Exception):
    """
    Helper class for raising logfs-related exceptions.

    """

    def __init__(self, err: LogFsErr):
        super().__init__()
        self.err = err

    def __repr__(self) -> str:
        return f"<{self.__class__.__name__}({self.err})>"

    def __str__(self) -> str:
        return f"LogFsErr: {self.err}"


class LogFs:
    """
    Thin wrapper around a logfs filesystem, to make the interface more
    Pythonic.

    """

    READ_ITER_CHUNK_SIZE = 32

    def __init__(
        self, block_size: int, block_count: int, context: LogFsContext
    ) -> None:
        self.fs = PyLogFs(block_size, block_count, context)

    def format(self) -> None:
        """
        Format the filesystem (erases anything currently on the disk).

        """
        self._check_err(self.fs.format())

    def mount(self) -> None:
        """
        Mount the filesystem.

        """
        self._check_err(self.fs.mount())

    def boot_count(self) -> int:
        """
        Get the number of times the filesystem has been mounted.

        """
        err, boot_count = self.fs.boot_count()
        self._check_err(err)
        return boot_count

    def open(self, path: str) -> LogFsFile:
        """
        Open a file.

        """
        file = LogFsFile()
        self._check_err(self.fs.open(file, path))
        return file

    def close(self, file: LogFsFile) -> None:
        """
        Close a file.

        """
        self._check_err(self.fs.close(file))

    def sync(self, file: LogFsFile) -> None:
        """
        Sync a file with the disk.

        """
        self._check_err(self.fs.sync(file))

    def read(self, file: LogFsFile, size: Optional[int] = None) -> int:
        """
        Read data from a file.

        """
        if size is None:
            # Iteratively read the entire file.
            file_data = b""
            file_num_read = 0

            # Read 0 bytes from the start of the file to reset the file read iterator.
            self.fs.read(file, 0, PyLogFsRead.START)

            while True:
                err, num_read, data = self.fs.read(
                    file, self.READ_ITER_CHUNK_SIZE, PyLogFsRead.ITER
                )
                self._check_err(err)

                if num_read == 0:
                    # Read failed, assume we've reached the end of the file.
                    break

                file_data = data[:num_read] + file_data
                file_num_read += num_read

            return file_data[:file_num_read]
        else:
            # Read the first size bytes.
            err, num_read, data = self.fs.read(file, size, PyLogFsRead.START)
            self._check_err(err)
            return data[:num_read]

    def write(self, file: LogFsFile, data: Union[bytes, str]) -> None:
        """
        Write data to a file.

        """
        if isinstance(data, str):
            data = data.encode("utf-8")

        err, num_written = self.fs.write(file, data, len(data))
        self._check_err(err)
        assert num_written == len(data)

    def list_dir(self, file: str = "/"):
        """
        List contents of the filesystem.

        """
        err, path, path_str = self.fs.first_path()
        self._check_err(err)
        paths = [path_str]

        # Iterate to find all files.
        while True:
            err, path, path_str = self.fs.next_path(path)
            if err == LogFsErr.INVALID_PATH:
                # Error code of invalid path indictes no more files.
                break

            self._check_err(err)
            paths.append(path_str)

        # Remove secret root file.
        if "/.root" in paths:
            paths.remove("/.root")

        # Filter by provided prefix.
        filtered_paths = [path for path in paths if path.startswith(file)]
        return filtered_paths

    def _check_err(self, err: LogFsErr) -> None:
        if err != LogFsErr.OK:
            raise LogFsError(err)
