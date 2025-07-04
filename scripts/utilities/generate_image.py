"""
generate_image.py
-----------------
Helper script used to create 2 hex files from an application and bootloader binary:
1. App with metadata: App hex with metadata information. Metadata is used by the bootloader
to verify the app's code before booting.
2. Image: The firmware image contains both the bootloader and the app with metadata. This
is the full program running on a microcontroller on the car.

"""

import argparse
import intelhex
import struct
import binascii


APP_METADATA_START = 0x8020000


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--app-hex", type=str, required=True)
    parser.add_argument("--boot-hex", type=str, required=True)
    parser.add_argument("--app-metadata-hex-out", type=str, required=True)
    parser.add_argument("--image-hex-out", type=str, required=True)
    args = parser.parse_args()

    app_hex = intelhex.IntelHex(args.app_hex)
    boot_hex = intelhex.IntelHex(args.boot_hex)

    app_size_bytes = app_hex.maxaddr() - app_hex.minaddr()
    app_code = bytes([app_hex[i] for i in range(app_hex.minaddr(), app_hex.maxaddr())])
    checksum = binascii.crc32(app_code)

    # Add checksum and app size (in bytes) to the app's metadata region.
    # Keep update to date with the "Metadata" struct in firmware/boot/shared/bootloader.c.
    metadata_bytes = struct.pack("<LL", checksum, app_size_bytes)
    app_hex[APP_METADATA_START : APP_METADATA_START + len(metadata_bytes)] = list(metadata_bytes)

    # Write app with metadata to filesystem.
    with open(args.app_metadata_hex_out, "w") as file:
        app_hex.tofile(file, format="hex")

    # Merge app with metadata and bootloader, creating the final image.
    app_hex.merge(boot_hex, overlap="replace")
    with open(args.image_hex_out, "w") as file:
        app_hex.tofile(file, format="hex")
