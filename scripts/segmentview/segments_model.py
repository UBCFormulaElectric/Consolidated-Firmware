"""
Data models for BMS segment information
"""

from dataclasses import dataclass, field
from typing import Dict


@dataclass
class SegmentData:
    """Data structure for a single segment"""
    segment_id: int
    cell_voltages: Dict[int, float] = field(default_factory=dict)  # {cell_id: voltage}
    cell_temps: Dict[int, float] = field(default_factory=dict)     # {cell_id: temperature}
    errors: Dict[str, bool] = field(default_factory=dict)          # {error_name: is_error}

