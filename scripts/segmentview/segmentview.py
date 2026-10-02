#!/usr/bin/env python3
"""
BMS Segment View - Interactive viewer for BMS segment information
Displays cell voltages, temperatures, and error states for all 10 battery segments

This is the main application file that imports modular components.
"""

import sys
from pathlib import Path
from threading import Thread
from typing import Dict

from PyQt5.QtGui import QFont
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QStatusBar

try:
    import can
except ImportError:
    print("Error: python-can not installed. Install with: pip install python-can")
    sys.exit(1)

# Import modular components
from .segments_model import SegmentData
from .can_decoder import CanDataThread
from .ui_components import CompactSegmentView




class SegmentViewUI(QMainWindow):
    """Main application window for segment viewing"""
    
    def __init__(self):
        super().__init__()
        self.setWindowTitle("BMS Segment Inspector")
        self.setGeometry(100, 100, 1400, 900)

        # Setup UI first
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(5, 5, 5, 5)
        main_layout.setSpacing(5)

        # Header
        header_layout = QHBoxLayout()
        header_layout.setContentsMargins(0, 0, 0, 0)
        title = QLabel("BMS Segment Monitor")
        title_font = QFont()
        title_font.setPointSize(12)
        title_font.setBold(True)
        title.setFont(title_font)
        header_layout.addWidget(title)
        header_layout.addStretch()

        self.connection_label = QLabel("● Connecting...")
        self.connection_label.setStyleSheet("color: orange; font-weight: bold; font-size: 9pt;")
        header_layout.addWidget(self.connection_label)

        main_layout.addLayout(header_layout)

        # Compact view widget - all tables on one page
        self.compact_view = CompactSegmentView()
        main_layout.addWidget(self.compact_view, 1)

        central_widget.setLayout(main_layout)

        # Initialize CAN data thread with DBC file
        self.can_thread = CanDataThread(Path(__file__).parent.parent.parent / "can_bus" / "dbcs" / "hexray.dbc")
        self.can_thread.data_received.connect(self.on_segment_data_received)
        self.can_thread.status_changed.connect(self.on_status_changed)
        self.can_thread.error_occurred.connect(self.on_error_occurred)

        # Start CAN reader in background thread
        self.reader_thread = Thread(target=self.can_thread.run, daemon=True)
        self.reader_thread.start()

        # Setup status
        self.setStatusBar(QStatusBar())
        self.statusBar().showMessage("Initializing CAN connection...")

    def on_segment_data_received(self, data: Dict):
        """Update display with received segment data"""
        seg_id = data['segment_id']
        seg_data = data['data']

        # Create placeholder errors dict if needed
        if not seg_data.errors:
            seg_data.errors = {
                "CommOk": True,
                "VA_OV": False,
                "VA_UV": False,
                "VD_OV": False,
                "VD_UV": False,
                "CED": False,
                "CMED": False,
                "SED": False,
                "SMED": False,
                "VDE": False,
                "VDEL": False,
                "THSD": False,
            }
        self.compact_view.update_data(seg_id, seg_data)

    def on_status_changed(self, status: str):
        """Update status bar"""
        self.statusBar().showMessage(status)
        if "Connected" in status:
            self.connection_label.setText("● Connected")
            self.connection_label.setStyleSheet("color: green; font-weight: bold; font-size: 9pt;")
        elif "Failed" in status or "Error" in status:
            self.connection_label.setText("● Error")
            self.connection_label.setStyleSheet("color: red; font-weight: bold; font-size: 9pt;")
        else:
            self.connection_label.setText("● Connecting...")
            self.connection_label.setStyleSheet("color: orange; font-weight: bold; font-size: 9pt;")

    def on_error_occurred(self, error_msg: str):
        """Handle CAN errors"""
        print(f"CAN Error: {error_msg}")
        self.statusBar().showMessage(f"Error: {error_msg}")

    def closeEvent(self, event):
        """Clean up when closing the application"""
        self.can_thread.stop()
        self.reader_thread.join(timeout=2)
        event.accept()



def main():
    """Main entry point"""
    app = QApplication(sys.argv)
    
    # Set application style
    app.setStyle('Oxygen')
    
    window = SegmentViewUI()
    window.show()
    
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
