import datetime
import os
from typing import Optional

from jsoncan import JsonCanParser


class AppSingletons:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(AppSingletons, cls).__new__(cls)
            cls._instance._live_can_db = None
            cls._instance._board_start_time = None
        return cls._instance

    def init_can_db(self, json_can_config_root):
        self._live_can_db = JsonCanParser(json_can_config_root).make_database()

    def update_can_db(self, path):
        self._live_can_db = JsonCanParser(path).make_database()

    def get_can_db(self):
        return self._live_can_db

    def update_base_time(self, time: datetime.datetime):
        self._board_start_time = time

    def get_base_time(self) -> Optional[datetime.datetime]:
        return self._board_start_time
