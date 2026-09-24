# logfs_cmd launcher

Scripts to build and start `logfs_cmd`, the interactive shell for reading and writing a LogFS-formatted SD card
(source: [`rust_backend/src/bin/logfs_cmd.rs`](../../rust_backend/src/bin/logfs_cmd.rs)).

`logfs_cmd` needs raw access to the card as a Linux block device (`/dev/sdX`), so it runs on Linux or on
Windows through WSL2. macOS is not supported.

| File | Purpose |
|---|---|
| `logfs_cmd.bat` | Windows entry point: double-click this |
| `logfs_cmd.ps1` | Attaches the USB card reader to WSL2 with usbipd-win, runs `logfs_cmd.sh` inside WSL, detaches on exit |
| `logfs_cmd.sh` | Builds `logfs_cmd` and runs it with `sudo` (used directly on Linux) |

## One-time setup (both platforms)

In your Linux environment (native Linux, or your WSL2 distro on Windows):

1. Install Rust:
   ```bash
   curl --proto '=https' -sSf https://sh.rustup.rs | sh
   ```
2. Install the C toolchain and libclang, which the logfs bindings need to build:
   ```bash
   sudo apt install -y build-essential libclang-dev
   ```

## Linux

1. Plug in the SD card (through a reader).
2. Run:
   ```bash
   software/tracksight/utils/logfs_cmd/logfs_cmd.sh
   ```
3. Enter your password when `sudo` asks. The tool needs root to open `/dev/sdX`.

## Windows (WSL2)

### Extra one-time setup

- You need **WSL2** with a distro such as Ubuntu installed. WSL1 won't work. Check the version with `wsl -l -v`.
- Update the WSL kernel so it has USB storage support:
  ```bash
  wsl --update
  ```
- [usbipd-win](https://github.com/dorssel/usbipd-win) 4.0 or newer is required. The script installs it through
  `winget` if it's missing, and upgrades it if it's older than 4.0 (older versions use different commands). You may
  need to re-run the script once after an install or upgrade.

### Running

1. Plug in the SD card reader. If Windows offers to format the card, **cancel**.
2. Double-click `logfs_cmd.bat` and accept the admin (UAC) prompt.
3. The script picks the card reader automatically if exactly one USB storage device is connected. Otherwise it lists
   your USB devices and asks for the reader's `BUSID`.
4. It attaches the reader to WSL, builds `logfs_cmd`, and starts it. Enter your WSL password when `sudo` asks.
5. Type `exit` when you're done. The reader is detached and goes back to Windows.

While the reader is attached to WSL, Windows can't see it. That's expected.

## Using logfs_cmd

A typical session:

```
<> / > lsdisk
  /dev/sdb
<> / > selectdisk /dev/sdb
<> / > mount
<> / > ls
<> / > export * all
<> / > exit
```

| Command | Description |
|---|---|
| `lsdisk` | List removable drives |
| `selectdisk <disk>` | Open a drive, e.g. `/dev/sdb` |
| `format` | Format the selected drive as LogFS. **Erases the card** |
| `mount` | Mount an existing LogFS on the selected drive |
| `ls` | List the current directory |
| `cd <dir>` | Change directory |
| `cat <file>` | Print a file's metadata (start timestamp) and data as hex |
| `write <file> <data>` | Write a single word to a file (no spaces) |
| `export <file\|*> [format]` | Decode a CAN log and save it to `exports/`. `*` exports every log in the current directory. `format` is `mf4` (default), `csv`, `xlsx` or `all` |
| `help` | List commands |
| `exit` | Quit |

## Exporting (MF4, CSV, XLSX)

`export` decodes the raw CAN frames in a log with the JSONCAN database in `can_bus/hexray`. It writes one file per
log and format to the `exports/` folder next to this README, named after the log (e.g. `exports/<log>.csv`). That
folder is gitignored. The files are owned by you, not root, even though `logfs_cmd` runs with `sudo`.

```
export 2025-06-01T12-00-00_042.txt        # MF4 only (default)
export 2025-06-01T12-00-00_042.txt csv
export * all                              # every log, all three formats
```

| Format | Layout | Open with |
|---|---|---|
| `mf4` | One channel group per signal, each with its own time channel | [asammdf GUI](https://www.csselectronics.com/pages/asammdf-gui-api-mdf4-mf4) or any MDF4 tool |
| `csv` | One row per sample, in log order | Anything; easiest for scripts (pandas etc.) |
| `xlsx` | Same table as the CSV, with real Excel timestamps | Excel, LibreOffice |

CSV and XLSX columns: `time_s` (seconds since the log started), `timestamp`, `signal`, `value`, `label`, `unit`.

- The start timestamp is the car's RTC time at boot. There's no timezone, so it's stored as local time.
- `label` is the enum value's name (e.g. `BMS_BALANCING_STATE`). It's empty for other signals. MF4 only has the
  raw numbers.
- Excel sheets hold at most about 1 million rows, so a long log continues on sheets named `Data 2`, `Data 3`, and
  so on. For big logs, MF4 or CSV is faster to write and open.
- Frames with IDs that aren't in the CAN database are skipped. The export prints how many were skipped. If there are
  many, the log was probably recorded with a different CAN config than your checkout.

## Troubleshooting

- **`lsdisk` shows nothing / "no drive with RM=1 found":** `logfs_cmd` only accepts drives the kernel reports as removable.
  Check the `RM` column in the `lsblk` output the script prints. Some card readers report `RM=0`, and
  `selectdisk` will refuse those.
- **Windows: the drive never shows up in WSL:** run `dmesg | tail` inside WSL after attaching. If there's no
  `usb-storage` / `sd` message, run `wsl --update`. If it still doesn't appear, your WSL kernel may lack USB storage
  support.
- **Windows: `usbipd attach` fails:** make sure the WSL distro is WSL2 (`wsl -l -v`) and that no other WSL instance or
  VM has the device attached (`usbipd list`).
- **Build fails in bindgen / "Unable to find libclang":** install `libclang-dev` (see setup above).
- **The first build is slow on Windows:** the repo lives on the Windows filesystem (`/mnt/c`). Later builds are
  incremental.
- **My local logfs changes aren't picked up:** `rust_backend/Cargo.toml` pulls `logfs` from `master` on GitHub.
  To test local changes, temporarily point it at the local crate:
  ```toml
  logfs = { path = "../../../firmware/logfs/rust" }
  ```
