"""
Data models for BMS segment information
"""

from dataclasses import dataclass, field
from typing import Dict

@dataclass
class SegmentError:
    comm_ok: bool
    va_ov: bool
    va_uv: bool
    vd_ov: bool
    vd_uv: bool
    ced: bool
    cmed: bool
    sed: bool
    smed: bool
    vde: bool
    vdel: bool
    thsd: bool
    tmodchk: bool
    oscchk: bool

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
    errors: SegmentError = field(default_factory=dict)          # {error_name: is_error}
    vref2: float = 0.0
    itmp: float = 0.0
