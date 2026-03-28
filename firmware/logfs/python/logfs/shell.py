import argparse
import os

from IPython.terminal.embed import InteractiveShellEmbed
from logfs.disk import LogFsDiskFactory
from logfs.fs import LogFs


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--disk", "-d", help="Path to disk", required=True)
    parser.add_argument("--block_size", "-b", help="Block size in bytes", default=512)
    parser.add_argument(
        "--block_count", "-N", help="Number of blocks", default=1024 * 1024
    )
    parser.add_argument(
        "--format",
        "-f",
        action="store_true",
        default=False,
        help="Format disk (will wipe an existing filesystem)",
    )
    parser.add_argument(
        "--write",
        "-w",
        action="store_true",
        help="Allow writes to the filesystem",
        default=False,
    )
    args = parser.parse_args()
    print(args)
    # Initialize the IPython shell.
    ipython_shell = InteractiveShellEmbed(
        banner1='A Python shell for interacting with logfs images.\nUse the "fs" object to read/modify the filesystem.',
        exit_msg="Leaving the shell.",
    )

    disk = LogFsDiskFactory.create_disk(
        block_size=args.block_size, block_count=args.block_count, disk_path=args.disk
    )

    fs = LogFs(
        block_size=args.block_size,
        block_count=args.block_count,
        disk=disk,
        mount=True,
        format=args.format,
        rd_only=not args.write,
    )

    # Launch the IPython shell.
    global_ns = globals()  # Get the global namespace
    global_ns["fs"] = fs
    ipython_shell(global_ns=global_ns, local_ns={})


if __name__ == "__main__":
    main()
