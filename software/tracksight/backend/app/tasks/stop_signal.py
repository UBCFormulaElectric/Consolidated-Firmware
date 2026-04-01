_should_run = True

def stop_run_signal() -> None:
	global _should_run
	_should_run = False

def should_run() -> bool:
	global _should_run
	return _should_run