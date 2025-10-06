from dataclasses import dataclass
from typing import Dict, Optional, Union

from strenum import StrEnum
from .utils import (bits_for_uint, is_int, pascal_to_screaming_snake_case, pascal_to_snake_case)

class CanSignalDatatype(StrEnum):
    """
    Enum for the possible primitive datatypes of a CAN signal.
    """

    BOOL = "bool"
    INT = "int32_t"
    UINT = "uint32_t"
    FLOAT = "float"

@dataclass(frozen=True)
class CanEnum:
    """
    Dataclass for fully describing a CAN value table.
    Note: Minimum value is assumed to always be 0. TODO: Change this?
    """

    name: str
    items: Dict[int, str]  # Dict of enum value to enum item name

    def max_val(self) -> int:
        """
        Maximum value present in this value table's entries.
        """
        return max(self.items.keys())

    @staticmethod
    def min_val() -> int:
        """
        Minimum value present in this value table's entries.
        """
        return 0

    def num_bits(self) -> int:
        """
        Number of bits needed to store this value table.
        """
        return bits_for_uint(self.max_val())

    def snake_name(self) -> str:
        return pascal_to_snake_case(self.name)

    def screaming_snake_name(self) -> str:
        return pascal_to_screaming_snake_case(self.name)

    def __str__(self) -> str:
        return self.name

    def __hash__(self) -> int:
        return hash(self.name)

@dataclass(frozen=True)
class CanSignal:
    """
    Dataclass for fully describing a CAN signal.
    """

    name: str  # Name of signal
    start_bit: int  # Start bit of signal in payload
    bits: int  # Number of bits to represent signal in payload, in bits
    scale: float  # Scale for encoding/decoding
    offset: float  # Offset for encoding/decoding
    min_val: float  # Min allowed value
    max_val: float  # Max allowed value
    # Default starting value, None if doesn't specify one
    start_val: Union[int, float]
    enum: Optional[CanEnum]  # Value table, None if doesn't specify one
    unit: str  # Signal's unit
    signed: bool  # Whether or not signal is represented as signed or unsigned
    description: str = "N/A"  # Description of signal
    big_endian: bool = False  # TODO: Add tests for big endianness

    def represent_as_integer(self) -> bool:
        """
        If this signal holds integer or unsigned integer values only.
        """
        return is_int(self.scale) and is_int(self.offset)

    def has_unit(self) -> bool:
        """
        If this signal specifies a unit.
        """
        return self.unit != ""

    def has_non_default_start_val(self) -> bool:
        """
        If this signal specifies a starting value.
        """
        return self.start_val is not None

    def representation(self) -> CanSignalDatatype:
        """
        How this signal will be represented in memory (specific to C).
        For example, in C, enums and booleans are both stored internally as unsigned ints.
        """
        if self.enum:
            return CanSignalDatatype.UINT
        elif self.represent_as_integer():
            if self.min_val >= 0:
                return CanSignalDatatype.UINT
            else:
                return CanSignalDatatype.INT
        else:
            return CanSignalDatatype.FLOAT

    def datatype(self) -> str | CanSignalDatatype:
        """
        The name the datatype this signal should be stored as (specific to C).
        """
        if self.enum:
            return self.enum.name
        elif (
            self.min_val == 0
            and self.max_val == 1
            and self.scale == 1
            and self.offset == 0
        ):
            return CanSignalDatatype.BOOL
        elif self.represent_as_integer():
            if self.min_val >= 0:
                return CanSignalDatatype.UINT
            else:
                return CanSignalDatatype.INT
        else:
            return CanSignalDatatype.FLOAT

    def snake_name(self) -> str:
        return pascal_to_snake_case(self.name)

    def screaming_snake_name(self) -> str:
        return pascal_to_screaming_snake_case(self.name)

    def start_val_macro(self) -> str:
        return f"CANSIG_{self.snake_name().upper()}_START_VAL"

    def max_val_macro(self) -> str:
        return f"CANSIG_{self.snake_name().upper()}_MAX_VAL"

    def min_val_macro(self) -> str:
        return f"CANSIG_{self.snake_name().upper()}_MIN_VAL"

    def scale_macro(self) -> str:
        return f"CANSIG_{self.snake_name().upper()}_SCALE"

    def offset_macro(self) -> str:
        return f"CANSIG_{self.snake_name().upper()}_OFFSET"

    def __str__(self) -> str:
        return self.name

    def __hash__(self) -> int:
        return hash(self.name)