"""
BMS Segment View - Interactive viewer for BMS segment information
Displays real-time cell voltage, temperature, and error data from CAN interface

Modular components:
- segments_model: Data structures
- can_decoder: CAN message handling
- ui_components: UI widgets
- segmentview: Main application
"""

from .segments_model import SegmentData
from .can_decoder import CanMessageDecoder, CanDataThread
from .ui_components import CompactSegmentView
from .segmentview import SegmentViewUI

__version__ = "2.0.0"
__all__ = [
    "SegmentData",
    "CanMessageDecoder",
    "CanDataThread",
    "CompactSegmentView",
    "SegmentViewUI",
]
