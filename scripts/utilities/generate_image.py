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
import math
import struct


APP_METADATA_START = 0x8020000


class Crc32:
    """
    Helper class used to calculate a 32-bit checksum. This mirrors STM32's Cyclic Redundancy
    Check (CRC) hardware peripheral so the microcontroller can check its app code for errors.

    Stolen from here:
    https://www.reddit.com/r/embedded/comments/13xiz1u/stm32_crc32_incompatible_with_all_other_existing/

    """

    STM32_CRC_POLY = 0x04C11DB7
    crc_table = {}

    def __init__(self) -> None:
        for i in range(256):
            c = i << 24
            for _ in range(8):
                c = (c << 1) ^ self.STM32_CRC_POLY if (c & 0x80000000) else c << 1

            self.crc_table[i] = c & 0xFFFFFFFF

    def calculate(self, buf: bytes) -> int:
        crc = 0xFFFFFFFF

        i = 0
        while i < len(buf):
            b = [buf[i + 3], buf[i + 2], buf[i + 1], buf[i + 0]]
            i += 4

            for byte in b:
                crc = ((crc << 8) & 0xFFFFFFFF) ^ self.crc_table[(crc >> 24) ^ byte]

        return crc


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--app-hex", type=str, required=True)
    parser.add_argument("--boot-hex", type=str, required=True)
    parser.add_argument("--app-metadata-hex-out", type=str, required=True)
    parser.add_argument("--image-hex-out", type=str, required=True)
    args = parser.parse_args()

    crc32 = Crc32()
    app_hex = intelhex.IntelHex(args.app_hex)
    boot_hex = intelhex.IntelHex(args.boot_hex)

    # Add checksum to app file metadata, so the bootloader can verify the chip has a valid
    # binary before booting. Our bootloader programs 8 bytes at a time (2 4-byte words), so
    # round the size up to the nearest 4 bytes.
    app_size_bytes = int(math.ceil((app_hex.maxaddr() - app_hex.minaddr()) / 8) * 8)
    app_code = [
        app_hex[i] for i in range(app_hex.minaddr(), app_hex.minaddr() + app_size_bytes)
    ]
    checksum = crc32.calculate(app_code)

    # Add checksum and app size (in bytes) to the app's metadata region.
    # Keep update to date with the "Metadata" struct in firmware/boot/shared/bootloader.c.
    metadata_bytes = struct.pack("<LL", checksum, app_size_bytes)
    app_hex[APP_METADATA_START : APP_METADATA_START + len(metadata_bytes)] = list(
        metadata_bytes
    )

    # Write app with metadata to filesystem.
    with open(args.app_metadata_hex_out, "w") as file:
        app_hex.tofile(file, format="hex")

    # Merge app with metadata and bootloader, creating the final image.
    app_hex.merge(boot_hex, overlap="replace")
    with open(args.image_hex_out, "w") as file:
        app_hex.tofile(file, format="hex")
