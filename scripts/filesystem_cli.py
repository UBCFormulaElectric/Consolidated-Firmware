import argparse

import click
from littlefs import LittleFS, UserContextWinDisk

parser = argparse.ArgumentParser()
parser.add_argument("--disk", type=str, help="Disk path", default=r"\\.\D: ") # r"\\.\D: " for accessing low level disk, "c:\Users" for accessing file image
parser.add_argument("--block_size", type=int, help="Block size", default=512)
parser.add_argument("--block_count", type=int, help="Block count", default=1000000)
parser.add_argument("--mount", type=bool, help="Mount", default=False)
parser.add_argument("--format", type=bool, help="Format the disk or not", default=True)


args = parser.parse_args()
current_dir = "/"

fs = LittleFS(
    block_size=args.block_size,
    block_count=args.block_count,
    mount=args.mount,
    context=UserContextWinDisk(args.disk),
)

if(args.format):
    fs.format()
    print("File system formatted")


fs.mount()
@click.command()
@click.argument('path', default=current_dir)
def ls(path):
    files = fs.listdir(path)
    print(files)

@click.command()
@click.argument('file')
@click.argument('host_dir')
def dump(file, host_dir):
    with fs.open(file, "rb") as fh:
        data = fh.read()
        with open(host_dir, "wb+") as f:
            f.write(data)

@click.command()
@click.argument('dir')
def cd(dir):
    global current_dir
    current_dir = dir


@click.command()
def pwd():
    print(current_dir)

@click.group()
def cli():
    """Custom File System CLI"""
    pass


cli.add_command(ls)
cli.add_command(dump)
cli.add_command(cd)
cli.add_command(pwd)


if __name__ == '__main__':
    fs.mount()
    click.echo("Mounted")
    while True:
        try:
            cmd = input('> ')
            cli.main(cmd.split(), standalone_mode=False)
        except Exception as e:
            click.echo(e)
            break
    fs.unmount()
