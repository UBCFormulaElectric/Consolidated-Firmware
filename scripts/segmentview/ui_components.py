"""
UI components for the BMS segment monitor
"""

from PyQt5.QtGui import QFont, QColor
from PyQt5.QtWidgets import (QWidget, QVBoxLayout, QHBoxLayout, QLabel,
                             QTableWidget, QTableWidgetItem, QHeaderView)

from .segments_model import SegmentData

SEGMENT_COUNT = 10
THERMISTOR_COUNT = 14
CELL_COUNT = 14

class CompactSegmentView(QWidget):
    """Single-page compact view for all segments - displays all tables at once"""

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setup_ui()
        self.segment_data = {}

    def setup_ui(self):
        """Initialize the compact UI with all tables on one page"""
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(3, 3, 3, 3)
        main_layout.setSpacing(2)

        # Summary section at top
        summary_label = QLabel("Summary")
        summary_font = QFont("Arial", 9)
        summary_font.setBold(True)
        summary_label.setFont(summary_font)
        main_layout.addWidget(summary_label, 0)

        self.summary_table = QTableWidget()
        self.summary_table.setColumnCount(5)
        self.summary_table.setHorizontalHeaderLabels(["Seg", "Status", "Avg V", "Max T", "Errors"])
        self.summary_table.horizontalHeader().setSectionResizeMode(0, QHeaderView.ResizeToContents)
        self.summary_table.horizontalHeader().setSectionResizeMode(1, QHeaderView.ResizeToContents)
        self.summary_table.horizontalHeader().setSectionResizeMode(2, QHeaderView.ResizeToContents)
        self.summary_table.horizontalHeader().setSectionResizeMode(3, QHeaderView.ResizeToContents)
        self.summary_table.horizontalHeader().setSectionResizeMode(4, QHeaderView.Stretch)
        self.summary_table.setStyleSheet("QTableWidget { font-size: 8pt; }")
        self.summary_table.setRowCount(10)
        main_layout.addWidget(self.summary_table, 2)

        # Horizontal layout for voltages and temperatures side-by-side
        side_by_side_layout = QHBoxLayout()
        side_by_side_layout.setContentsMargins(0, 0, 0, 0)
        side_by_side_layout.setSpacing(2)

        # Voltages section (left)
        voltages_layout = QVBoxLayout()
        voltages_layout.setContentsMargins(0, 0, 0, 0)
        voltages_layout.setSpacing(2)

        voltages_label = QLabel("Cell Voltages (V)")
        voltages_label.setFont(summary_font)
        voltages_layout.addWidget(voltages_label, 0)

        self.voltages_table = QTableWidget()
        header = ["Seg"] + [f"C{i}" for i in range(CELL_COUNT)]
        assert CELL_COUNT == THERMISTOR_COUNT
        self.voltages_table.setColumnCount(len(header))
        self.voltages_table.setHorizontalHeaderLabels(header)
        for i in range(len(header)):
            self.voltages_table.horizontalHeader().setSectionResizeMode(i, QHeaderView.ResizeToContents)
        self.voltages_table.setStyleSheet("QTableWidget { font-size: 7pt; }")
        self.voltages_table.setRowCount(SEGMENT_COUNT)
        voltages_layout.addWidget(self.voltages_table, 1)

        side_by_side_layout.addLayout(voltages_layout, 1)

        # Temperatures section (right)
        temps_layout = QVBoxLayout()
        temps_layout.setContentsMargins(0, 0, 0, 0)
        temps_layout.setSpacing(2)

        temps_label = QLabel("Thermistor Temperatures (°C)")
        temps_label.setFont(summary_font)
        temps_layout.addWidget(temps_label, 0)

        self.temps_table = QTableWidget()
        self.temps_table.setColumnCount(len(header))
        self.temps_table.setHorizontalHeaderLabels(header)
        for i in range(len(header)):
            self.temps_table.horizontalHeader().setSectionResizeMode(i, QHeaderView.ResizeToContents)
        self.temps_table.setStyleSheet("QTableWidget { font-size: 7pt; }")
        self.temps_table.setRowCount(SEGMENT_COUNT)
        temps_layout.addWidget(self.temps_table, 1)

        side_by_side_layout.addLayout(temps_layout, 1)

        # Add the side-by-side layout to main layout
        main_layout.addLayout(side_by_side_layout, 3)

        self.setLayout(main_layout)

        # Initialize tables
        self._init_tables()

    def _init_tables(self):
        """Initialize table rows for all segments"""
        for seg_id in range(10):
            # Summary table
            seg_label = QTableWidgetItem(f"S{seg_id}")
            seg_label.setFont(QFont("Arial", 8))
            self.summary_table.setItem(seg_id, 0, seg_label)

            # Status
            status = QTableWidgetItem("✓")
            status.setForeground(QColor("green"))
            self.summary_table.setItem(seg_id, 1, status)

            # Avg voltage
            avg_v = QTableWidgetItem("0.00")
            self.summary_table.setItem(seg_id, 2, avg_v)

            # Max temp
            max_t = QTableWidgetItem("0.0")
            self.summary_table.setItem(seg_id, 3, max_t)

            # Errors
            errors = QTableWidgetItem("")
            self.summary_table.setItem(seg_id, 4, errors)

            # Voltages table
            seg_label = QTableWidgetItem(f"S{seg_id}")
            self.voltages_table.setItem(seg_id, 0, seg_label)
            for cell_id in range(14):
                cell = QTableWidgetItem("0.00")
                cell.setFont(QFont("Arial", 7))
                self.voltages_table.setItem(seg_id, cell_id + 1, cell)

            # Temperatures table
            seg_label = QTableWidgetItem(f"S{seg_id}")
            self.temps_table.setItem(seg_id, 0, seg_label)
            for cell_id in range(14):
                cell = QTableWidgetItem("0.0")
                cell.setFont(QFont("Arial", 7))
                self.temps_table.setItem(seg_id, cell_id + 1, cell)

    def update_data(self, segment_id: int, data: SegmentData):
        """Update segment data in all tables"""
        self.segment_data[segment_id] = data

        # Update summary
        status_item = self.summary_table.item(segment_id, 1)
        avg_v_item = self.summary_table.item(segment_id, 2)
        max_t_item = self.summary_table.item(segment_id, 3)
        errors_item = self.summary_table.item(segment_id, 4)

        if data.cell_voltages:
            avg_v = sum(data.cell_voltages.values()) / len(data.cell_voltages)
            avg_v_item.setText(f"{avg_v:.2f}")

        if data.cell_temps:
            max_t = max(data.cell_temps.values())
            max_t_item.setText(f"{max_t:.1f}")
            # Color code max temp
            if max_t > 55:
                max_t_item.setForeground(QColor("red"))
            elif max_t > 45:
                max_t_item.setForeground(QColor("orange"))
            else:
                max_t_item.setForeground(QColor("green"))

        # Check for errors
        if data.errors.count() > 0:
            errors_item.setText(data.errors.text())
            errors_item.setForeground(QColor("red"))
            status_item.setText("⚠")
            status_item.setForeground(QColor("red"))
        else:
            errors_item.setText("")
            status_item.setText("✓")
            status_item.setForeground(QColor("green"))

        # Update voltage colors and values
        for cell_id, voltage in data.cell_voltages.items():
            item = self.voltages_table.item(segment_id, cell_id + 1)
            if item:
                item.setText(f"{voltage:.2f}")
                if voltage < 2.5 or voltage > 4.2:
                    item.setForeground(QColor("red"))
                else:
                    item.setForeground(QColor("black"))

        # Update temperature colors and values
        for cell_id, temp in data.cell_temps.items():
            item = self.temps_table.item(segment_id, cell_id + 1)
            if item:
                item.setText(f"{temp:.1f}")
                if temp > 55:
                    item.setForeground(QColor("red"))
                elif temp > 45:
                    item.setForeground(QColor("orange"))
                else:
                    item.setForeground(QColor("black"))

