#!/usr/bin/env bash
# Builds and launches logfs_cmd. Run directly on native Linux, or via logfs_cmd.bat on Windows (WSL2).

set -euo pipefail

script_dir="$(dirname "$(readlink -f "$0")")"
backend_dir="$(readlink -f "$script_dir/../../rust_backend")"

[ -f "$HOME/.cargo/env" ] && . "$HOME/.cargo/env"
if ! command -v cargo >/dev/null; then
    echo "cargo not found. Install Rust with: curl --proto '=https' -sSf https://sh.rustup.rs | sh" >&2
    exit 1
fi
if ! /sbin/ldconfig -p 2>/dev/null | grep -q libclang; then
    echo -e "\033[0;33mWarning: libclang not found (bindgen needs it). Try: sudo apt install -y build-essential libclang-dev\033[0m"
fi

cargo build --manifest-path "$backend_dir/Cargo.toml" --bin logfs_cmd
bin="${CARGO_TARGET_DIR:-$backend_dir/target}/debug/logfs_cmd"

# The attached drive can take a few seconds to show up after usbipd attach.
echo "Waiting for a removable drive..."
for _ in $(seq 15); do
    grep -qx 1 /sys/block/*/removable 2>/dev/null && break
    sleep 1
done
lsblk -d -o NAME,SIZE,RM,TRAN,MODEL
if ! grep -qx 1 /sys/block/*/removable 2>/dev/null; then
    echo -e "\033[0;33mWarning: no drive with RM=1 found; use the explicit /dev path shown by lsblk with --disk or selectdisk.\033[0m"
fi

echo "Starting logfs_cmd (try: lsdisk, selectdisk /dev/sdX, mount, ls, export * all). Type 'help' for commands."
sudo "$bin" "$@"
