"""
Data models for BMS segment information
"""

from dataclasses import dataclass, field
from typing import Dict

@dataclass
class SegmentError:
    comm_ok: bool = False
    va_ov: bool = False
    va_uv: bool = False
    vd_ov: bool = False
    vd_uv: bool = False
    ced: bool = False
    cmed: bool = False
    sed: bool = False
    smed: bool = False
    vde: bool = False
    vdel: bool = False
    thsd: bool = False
    tmodchk: bool = False
    oscchk: bool = False

    def count(self) -> int:
        return sum(1 for v in self.__dict__.values() if v)
    def text(self) -> str:
        return ', '.join(name for name, is_error in self.__dict__.items() if is_error)

@dataclass
class SegmentData:
    """Data structure for a single segment"""
    segment_id: int
    total_voltage: float = 0.0
    cell_voltages: Dict[int, float] = field(default_factory=dict)  # {cell_id: voltage}
    cell_open_wire: Dict[int, bool] = field(default_factory=dict)  # {cell_id: is_open_wire}
    cell_overvoltage: Dict[int, bool] = field(default_factory=dict)  # {cell_id: is_overvoltage}
    cell_undervoltage: Dict[int, bool] = field(default_factory=dict)  # {cell_id: is_undervoltage}
    cell_temps: Dict[int, float] = field(default_factory=dict)     # {cell_id: temperature}
    thermistor_open_wire: Dict[int, bool] = field(default_factory=dict)  # {thermistor_id: is_open_wire}
    errors: SegmentError = field(default_factory=SegmentError)          # {error_name: is_error}
    vref2: float = 0.0
    itmp: float = 0.0
