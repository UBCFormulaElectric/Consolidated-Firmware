import argparse
from IPython.terminal.embed import InteractiveShellEmbed
from .fs import LogFs
from .context import LogFsUnixContext


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--device", "-d", help="Path to device", required=True)
    parser.add_argument("--block_size", "-b", help="Block size in bytes", default=512)
    parser.add_argument(
        "--block_count", "-N", help="Number of blocks", default=1024 * 1024
    )
    parser.add_argument(
        "--format",
        "-f",
        action="store_true",
        help="Format device if mounting fails",
    )
    args = parser.parse_args()

    # Initialize the IPython shell.
    ipython_shell = InteractiveShellEmbed(
        banner1='A Python shell for interacting with logfs images.\nUse the "fs" object to read/modify the filesystem.',
        exit_msg="Leaving the shell.",
    )

    # Open the disk.
    context = LogFsUnixContext(
        block_size=args.block_size, block_count=args.block_count, disk_path=args.device
    )
    fs = LogFs(
        block_size=args.block_size, block_count=args.block_count, context=context
    )

    if args.format:
        fs.format()
    else:
        fs.mount()

    # Launch the IPython shell.
    global_ns = globals()  # Get the global namespace
    global_ns["fs"] = fs
    ipython_shell(global_ns=global_ns, local_ns={})
