#!/usr/bin/env python3
"""
Launcher script for the Segment View UI
Run this script to start the BMS segment inspector application
"""

import sys
import os

# Add parent directory to path to allow imports
script_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(script_dir)
sys.path.insert(0, parent_dir)

# Import and run
from segmentview import segmentview

if __name__ == "__main__":
    segmentview.main()

