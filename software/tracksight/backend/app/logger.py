from datetime import datetime
import os
import logging

from settings import LOG_LEVEL

def _get_log_level_from_name(name: str) -> int:
	if name == "DEBUG":
		return logging.DEBUG
	elif LOG_LEVEL == "INFO":
		return logging.INFO
	elif LOG_LEVEL == "WARNING":
		return logging.WARNING
	elif LOG_LEVEL == "ERROR":
		return logging.ERROR
	elif LOG_LEVEL == "CRITICAL":
		return logging.CRITICAL
	else:
		raise ValueError(f"Invalid LOG_LEVEL: {LOG_LEVEL}")

# Setup logging.
def _setupLogger() -> tuple[logging.Logger, str]:
	time_now = datetime.now().strftime("%Y-%m-%d_%H_%M_%S")
	app_dir = os.path.dirname(os.path.abspath(__file__))
	log_path = os.path.join(app_dir, "..", "logs", f"Live-Data--{time_now}.log")
	if not os.path.exists(os.path.dirname(log_path)):
		os.makedirs(os.path.dirname(log_path))

	logger = logging.getLogger(__name__)

	logger.handlers.clear()

	# Create a file handler
	file_handler = logging.FileHandler(log_path)
	# Create a formatter and set it for the file handler
	formatter = logging.Formatter('(%(asctime)s - %(name)s) [%(levelname)s] %(message)s')
	file_handler.setFormatter(formatter)
	logger.addHandler(file_handler)

	# console handler
	consoleHandler = logging.StreamHandler()
	consoleHandler.setFormatter(formatter)
	logger.addHandler(consoleHandler)

	logger.setLevel(_get_log_level_from_name(LOG_LEVEL))  # Set the logging level to DEBUG

	return logger, log_path

logger, log_path = _setupLogger()