# canup

`canup` is a program to interface with our bootloader, allowing one-click programming of multiple boards over the CAN bus.
It handles parsing an embedded binary and sending it to the bootloader. 

It is separate from `Consolidated-Firmware` because many of our members develop on Windows Subsystem for Linux (WSL), 
but WSL does not support CAN. So if you use WSL, clone this on native Windows. However, it is also a submodule in `Consolidated-Firmware` for non-WSL members.

## Setup

To install Python packages system-wide, run:
```sh
pip3 install -r requirements.txt
```
To use a Pipenv virtual environment, run: 
```sh
pipenv install
```
And prefix running the Python script with `pipenv run`.

## Usage

To see how to use `canup`, run `python update.py --help`. You need to specify the path to your firmware build directory with the `--build` or `-b` flag. If you're developing on WSL, you can access your Linux distro filesystems from `\\\wsl.localhost\`. Also, pass the `--config` or `-c` flag to set which boards are updated. These are defined in `boards.py`. 

For example, to update all of Quadruna's boards, I run: 

```sh
python update.py -b //wsl.localhost/Ubuntu/home/gtaharaedmonds/Consolidated-Firmware/build_fw_deploy -c quadruna
```

**Note**: On Windows using the Git Bash terminal, \ is replaced with /
