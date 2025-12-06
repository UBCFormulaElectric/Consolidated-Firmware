"""
boards.py
-----------------
Config info about boards that can be updated.

"""

from typing import List
import os
import dataclasses

KB = 1024  # bytes


@dataclasses.dataclass(frozen=True)
class FlashSector:
    id: int
    base_address: int
    size: int
    write_protect: bool = False

    @property
    def max_address(self) -> int:
        return self.base_address + self.size - 1


@dataclasses.dataclass(frozen=True)
class Microcontroller:
    name: str
    flash_sectors: List[FlashSector]


@dataclasses.dataclass(frozen=True)
class Board:
    name: str
    boot_id_range_start: int
    app_id_range_start: int
    mcu: Microcontroller
    path: str

    def __hash__(self) -> int:
        return hash(
            (
                self.name,
                self.boot_id_range_start,
                self.app_id_range_start,
                self.mcu.name,
            )
        )


STM32F412_MCU = Microcontroller(
    name="STM32F412xx",
    # Referenced from ST RM0402.
    # (https://www.st.com/resource/en/reference_manual/dm00180369-stm32f412-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
    flash_sectors=[
        FlashSector(
            id=sector_id,
            base_address=base,
            size=size_kb * KB,
            write_protect=write_protect,
        )
        for sector_id, base, size_kb, write_protect in [
            # Sectors 0-4 taken up by bootloader code, so mark them as write-protect.
            (0, 0x08000000, 16, True),  # Sector 0
            (1, 0x08004000, 16, True),  # Sector 1
            (2, 0x08008000, 16, True),  # Sector 2
            (3, 0x0800C000, 16, True),  # Sector 3
            (4, 0x08010000, 64, True),  # Sector 4
            (5, 0x08020000, 128, False),  # Sector 5
            (6, 0x08040000, 128, False),  # Sector 6
            (7, 0x08060000, 128, False),  # Sector 7
            (8, 0x08080000, 128, False),  # Sector 8
            (9, 0x080A0000, 128, False),  # Sector 9
            (10, 0x080C0000, 128, False),  # Sector 10
            (11, 0x080E0000, 128, False),  # Sector 11
        ]
    ],
)

STM32H733_MCU = Microcontroller(
    name="STM32H733xx",
    # Referenced from ST RM0468.
    # (https://www.st.com/resource/en/reference_manual/rm0468-stm32h723733-stm32h725735-and-stm32h730-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
    flash_sectors=[
        FlashSector(
            id=sector_id,
            base_address=base,
            size=size_kb * KB,
            write_protect=write_protect,
        )
        for sector_id, base, size_kb, write_protect in [
            # Sectors 0 is taken up by bootloader code, so mark it as write-protect.
            (0, 0x08000000, 128, True),  # Sector 0
            (1, 0x08020000, 128, False),  # Sector 1
            (2, 0x08040000, 128, False),  # Sector 2
            (3, 0x08060000, 128, False),  # Sector 3
            (4, 0x08080000, 128, False),  # Sector 4
            (5, 0x080A0000, 128, False),  # Sector 5
            (6, 0x080C0000, 128, False),  # Sector 6
            (7, 0x080E0000, 128, False),  # Sector 7
        ]
    ],
)

quintuna_FSM = Board(
    name="FSM",
    boot_id_range_start=0x10000000,
    app_id_range_start=600,
    mcu=STM32F412_MCU,
    path=os.path.join("firmware", "quintuna", "FSM", "quintuna_FSM_app_metadata.hex"),
)
quintuna_RSM = Board(
    name="RSM",
    boot_id_range_start=0x14000000,
    app_id_range_start=700,
    mcu=STM32F412_MCU,
    path=os.path.join("firmware", "quintuna", "RSM", "quintuna_RSM_app_metadata.hex"),
)
quintuna_CRIT = Board(
    name="CRIT",
    boot_id_range_start=0x18000000,
    app_id_range_start=900,
    mcu=STM32F412_MCU,
    path=os.path.join("firmware", "quintuna", "CRIT", "quintuna_CRIT_app_metadata.hex"),
)
quintuna_DAM = Board(
    name="DAM",
    boot_id_range_start=0x0C000000,
    app_id_range_start=800,
    mcu=STM32H733_MCU,
    path=os.path.join("firmware", "quintuna", "DAM", "quintuna_DAM_app_metadata.hex"),
)
quintuna_VC = Board(
    name="VC",
    boot_id_range_start=0x08000000,
    app_id_range_start=500,
    mcu=STM32H733_MCU,
    path=os.path.join("firmware", "quintuna", "VC", "quintuna_VC_app_metadata.hex"),
)
quintuna_BMS = Board(
    name="BMS",
    boot_id_range_start=0x04000000,
    app_id_range_start=400,
    mcu=STM32H733_MCU,
    path=os.path.join("firmware", "quintuna", "BMS", "quintuna_BMS_app_metadata.hex"),
)

h7dev = Board(
    name="h7dev",
    boot_id_range_start=0x20000000,
    app_id_range_start=1200,
    mcu=STM32H733_MCU,
    path=os.path.join("firmware", "dev", "h7dev", "h7dev_app_metadata.hex"),
)

CONFIGS = {
    "quintuna_FSM": [quintuna_FSM],
    "quintuna_RSM": [quintuna_RSM],
    "quintuna_CRIT": [quintuna_CRIT],
    "quintuna_DAM": [quintuna_DAM],
    "quintuna_BMS": [quintuna_BMS],
    "quintuna_VC" : [quintuna_VC],
    "quintuna_FD": [quintuna_DAM, quintuna_VC, quintuna_BMS],
    "quintuna_Sx" : [quintuna_CRIT, quintuna_FSM, quintuna_RSM],
    "quintuna" : [quintuna_RSM, quintuna_BMS, quintuna_CRIT, quintuna_DAM, quintuna_CRIT, quintuna_FSM, quintuna_VC],
    "h7dev": [h7dev],
}
