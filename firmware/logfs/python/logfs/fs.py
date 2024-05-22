from typing import Union, Optional, Type, Any
from .disk import LogFsDisk
from logfs_src import LogFsErr, PyLogFs, PyLogFsFile, PyLogFsReadFlags, PyLogFsOpenFlags


READ_ITER_CHUNK_SIZE = 32


def _raise_err(err: LogFsErr) -> None:
    """
    Raise an exception if there was an error.

    """
    if err != LogFsErr.OK:
        raise LogFsError(err)


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


class LogFsFile:
    """
    Thin wrapper around a logfs file, to make the interface more
    Pythonic.

    """

    def __init__(self, file: PyLogFsFile, fs: PyLogFs, block_size: int) -> None:
        self.file = file
        self.fs = fs
        self.block_size = block_size

    def path(self) -> str:
        return self.file.path()

    def write(self, data: Union[bytes, str]) -> None:
        """
        Write data to a file.

        """
        if isinstance(data, str):
            data = data.encode("utf-8")

        err = self.fs.write(self.file, data)
        _raise_err(err)

    def read(self, size: Optional[int] = None) -> int:
        """
        Read data from a file.

        """
        if size is None:
            # Iteratively read the entire file.
            file_data = b""
            file_num_read = 0

            # Read 0 bytes from the end of the file to reset the file read iterator.
            self.fs.read(self.file, 0, PyLogFsReadFlags.END)

            while True:
                err, num_read, data = self.fs.read(
                    self.file, READ_ITER_CHUNK_SIZE, PyLogFsReadFlags.ITER
                )
                _raise_err(err)

                if num_read == 0:
                    # Read failed, assume we've reached the end of the file.
                    break

                file_data = data[:num_read] + file_data
                file_num_read += num_read

            return file_data[:file_num_read]
        else:
            # Read the last size bytes.
            err, num_read, data = self.fs.read(self.file, size, PyLogFsReadFlags.END)
            _raise_err(err)
            return data[:num_read]

    def write_metadata(self, data: Union[bytes, str]) -> None:
        """
        Write metadata to a file.

        """
        if isinstance(data, str):
            data = data.encode("utf-8")

        err = self.fs.write_metadata(self.file, data)
        _raise_err(err)

    def read_metadata(self, size: Optional[int] = None) -> int:
        """
        Read metadata from a file.

        """
        if size is None:
            size = self.block_size

        err, num_read, data = self.fs.read_metadata(self.file, size)
        _raise_err(err)
        return data[:num_read]

    def close(self) -> None:
        """
        Close a file.

        """
        _raise_err(self.fs.close(self.file))

    def sync(self) -> None:
        """
        Sync a file with the disk.

        """
        _raise_err(self.fs.sync(self.file))

    def __enter__(self) -> "LogFsFile":
        """
        Context manager protocol (for use with the `with` statement).

        """
        return self

    def __exit__(
        self,
        exc_type: Optional[Type[BaseException]],
        exc_value: Optional[BaseException],
        exc_traceback: Optional[Any],
    ) -> Optional[bool]:
        """
        Context manager protocol (for use with the `with` statement).

        """
        if self.file.is_open():
            self.close()

        return None

    def __del__(self) -> None:
        """
        Close file on deletion.

        """
        if self.file.is_open():
            self.close()


class LogFs:
    """
    Thin wrapper around a logfs filesystem, to make the interface more
    Pythonic.

    """

    def __init__(
        self,
        block_size: int,
        block_count: int,
        disk: LogFsDisk,
        write_cycles: int = 0,
        rd_only: bool = True,
        mount=True,
        format=False,
    ) -> None:
        self.block_size = block_size
        self.block_count = block_count
        self.write_cycles = write_cycles
        self.disk = disk
        self.fs = PyLogFs(block_size, block_count, write_cycles, rd_only, disk)

        if format:
            self.format()
            self.mount()
        elif mount:
            self.mount()

    def format(self) -> None:
        """
        Format the filesystem (erases anything currently on the disk).

        """
        _raise_err(self.fs.format())

    def mount(self) -> None:
        """
        Mount the filesystem.

        """
        _raise_err(self.fs.mount())

    def open(self, path: str, flags="r") -> LogFsFile:
        """
        Open a file.

        """
        reading = False
        writing = False
        creating = False

        for ch in flags:
            if ch == "r":
                reading = True
            elif ch == "w":
                writing = True
            elif ch == "x":
                creating = True
            else:
                raise ValueError(f"Invalid flags: '{ch}'")

        flags = 0
        if reading and writing:
            flags |= int(PyLogFsOpenFlags.RD_WR)
        elif reading:
            flags |= int(PyLogFsOpenFlags.RD_ONLY)
        elif writing:
            flags |= int(PyLogFsOpenFlags.WR_ONLY)
        else:
            raise ValueError(f"Invalid flags string: '{flags}'")

        if creating:
            flags |= int(PyLogFsOpenFlags.CREATE)

        file = PyLogFsFile()
        _raise_err(self.fs.open(file, path, flags))
        return LogFsFile(file=file, fs=self.fs, block_size=self.block_size)

    def list_dir(self, matches: str = "/"):
        """
        List contents of the filesystem.

        """
        err, path, path_str = self.fs.first_path()
        _raise_err(err)
        paths = [path_str]

        # Iterate to find all files.
        while True:
            err, path, path_str = self.fs.next_path(path)
            if err == LogFsErr.NO_MORE_FILES:
                # Error code of invalid path indictes no more files.
                break

            _raise_err(err)
            paths.append(path_str)

        # Remove super secret root file.
        if "/.root" in paths:
            paths.remove("/.root")

        # Filter by provided prefix.
        filtered_paths = [path for path in paths if path.startswith(matches)]
        return filtered_paths
    
    def cat(self, path: str) -> None:
        """
        Linux command to print contents of a file.

        """
        file = self.open(path=path, flags="r")
        metadata = file.read_metadata()
        data = file.read()

        print("Metadata:")
        print(metadata)
        print("Data:")
        print(data)
