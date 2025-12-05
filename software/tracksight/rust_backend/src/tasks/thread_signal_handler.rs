use std::sync::atomic::{AtomicBool, Ordering};

static RUNNING: AtomicBool = AtomicBool::new(true);

pub fn stop_threads() {
    RUNNING.store(false, Ordering::SeqCst);
}

pub fn should_run() -> bool {
    RUNNING.load(Ordering::SeqCst)
}