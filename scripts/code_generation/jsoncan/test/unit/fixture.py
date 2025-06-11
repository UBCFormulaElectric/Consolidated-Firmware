import os
import unittest

from ...src.can_database import CanDatabase
from ...src.json_parsing.json_can_parsing import JsonCanParser

# valid_json_folder = os.path.abspath("./test/valid_json")
valid_json_folder = os.path.join(
    os.path.dirname(__file__), os.pardir, "json_configs", "valid_json1"
)

# disable test sorting, so that the order of tests is consistent and per given
unittest.TestLoader.sortTestMethodsUsing = None


class CDBTests(unittest.TestCase):
    ecu1_msgs = {
        "ECU1_BasicSignalTypes",
        "ECU1_DecimalNumbers",
        "ECU1_DbcMatching",
        "ECU1_MinMaxClamping",
        "ECU1_Debug_Only",
        "ECU1_LongMessage",
    }
    ec1_alerts = {
        "ECU1_Warnings",
        "ECU1_WarningsCounts",
        "ECU1_Faults",
        "ECU1_FaultsCounts",
        "ECU1_Info",
        "ECU1_InfoCounts",
    }
    ecu2_msgs = {"ECU2_BasicSignalTypes"}
    ecu3_msgs = {"ECU3_TEST"}
    ecu3_alerts = {
        "ECU3_Warnings",
        "ECU3_WarningsCounts",
        "ECU3_Faults",
        "ECU3_FaultsCounts",
        "ECU3_Info",
        "ECU3_InfoCounts",
    }
    ecu4_msgs = {"ECU4_TEST"}
    ecu4_alerts = {
        "ECU4_Warnings",
        "ECU4_WarningsCounts",
        "ECU4_Faults",
        "ECU4_FaultsCounts",
        "ECU4_Info",
        "ECU4_InfoCounts",
    }
    ecu5_msgs = {"ECU5_TEST"}
    ecu5_alerts = {
        "ECU5_Warnings",
        "ECU5_WarningsCounts",
        "ECU5_Faults",
        "ECU5_FaultsCounts",
        "ECU5_Info",
        "ECU5_InfoCounts",
    }

    @classmethod
    def setUpClass(cls):
        cls.parser_valid: JsonCanParser = JsonCanParser(
            valid_json_folder
        )  # slight white box testing situation
        cls.cdb_valid: CanDatabase = cls.parser_valid.make_database()
