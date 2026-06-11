pub mod api_handler;
pub mod serial_handler;
pub mod telem_message;
pub mod can_data;
pub mod client_api;
pub mod can_data_handler;

use std::{collections::HashSet, sync::{Arc, OnceLock}, time::Duration};

use serde::{Serialize};
use tokio::{select, sync::{RwLock, broadcast, mpsc}, time::sleep};

use crate::{utils::{red}, vprintln};

/**
 * BACKEND TELEMETRY
 */
// i think i like this much better than current implementation of health check
// replace health check with this pattern

#[derive(Debug, Clone, Serialize)]
struct BackendTelemetryState {
    total_crc_checks: u64,
    failed_crc_checks: u64,
}

pub enum BackendTelemetry {
    CrcCheck(bool)
}

static BACKEND_TELEMETRY_TX: OnceLock<mpsc::Sender<BackendTelemetry>> = OnceLock::new();
static BACKEND_TELEMETRY_STATE: OnceLock<Arc<RwLock<BackendTelemetryState>>>  = OnceLock::new();

pub fn get_backend_telem_tx() -> &'static mpsc::Sender<BackendTelemetry> {
    BACKEND_TELEMETRY_TX.get().expect("Backend telemetry channel not initialized")
}

pub fn get_backend_telem_state() -> &'static Arc<RwLock<BackendTelemetryState>> {
    BACKEND_TELEMETRY_STATE
        .get()
        .expect("Backend telemetry state not initialized")
}

pub async fn run_backend_telemetry_logger(mut shutdown_rx: ShutdownReceiver) {
    let (telem_tx, mut telem_rx) = mpsc::channel::<BackendTelemetry>(4096);
    BACKEND_TELEMETRY_TX.set(telem_tx).unwrap();

    let state = Arc::new(RwLock::new(BackendTelemetryState {
        total_crc_checks: 0,
        failed_crc_checks: 0,
    }));

    BACKEND_TELEMETRY_STATE.set(state.clone()).unwrap();
    
    loop {
        select! {
            _ = shutdown_rx.recv() => {
                vprintln!("Backend telemetry logger task shutting down.");
                break;
            }
            Some(telem) = telem_rx.recv() => {
                let mut state = state.write().await;
                match telem {
                    BackendTelemetry::CrcCheck(success) => {
                        state.total_crc_checks += 1;
                        if !success {
                            state.failed_crc_checks += 1;
                        }
                    }
                }
            }
        }
    }
}

/**
 * HEALTH CHECK
 */

// im not even going to lie
// this is kind of an overkill for a health check on tasks
// it can probably also be implemented a lot more cleanly
// but it was cool so idk

pub const TASK_RESTART_DELAY_MS: u64 = 1000;

pub type ShutdownReceiver = broadcast::Receiver<()>;

/**
 * Tasks to health check
 */
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum Task {
    SerialHandler,
    ApiHandler,
    CanDataHandler,
    InfluxHandler,
    LiveDataHandler,
    BackendTelemetryLogger
}

impl Task {
    // apparently rust doesn't allow (safe) count of enums
    // this number should just match number of tasks to run to health check every thing
    const ENUM_COUNT: usize = 5;
}

#[derive(Debug)]
pub enum HealthCheckError {
    Timeout,
    TaskFailure(Task),
    ChannelsClosed
}

pub struct HealthCheck {
    pub hc_tx: mpsc::Sender<(Task, bool)>,
    hc_rx: mpsc::Receiver<(Task, bool)>,
    checked_tasks: HashSet<Task>,
}

impl HealthCheck {
    // health checks for tasks, check for count after starting all tasks
    const HEALTH_CHECK_TIMEOUT_MS: u64 = 5000;

    pub fn new() -> Self {
        let (sender, receiver) = mpsc::channel::<(Task, bool)>(Task::ENUM_COUNT);
        let tasks_set = HashSet::new();

        return HealthCheck { hc_tx: sender, hc_rx: receiver, checked_tasks: tasks_set }
    }

    pub async fn wait_for_health_checks(&mut self) -> Result<(), HealthCheckError> {
        select! {
            health_check_res = self.track_health_checks() => {
                return health_check_res;
            },
            _ = sleep(Duration::from_millis(HealthCheck::HEALTH_CHECK_TIMEOUT_MS)) => {
                return Err(HealthCheckError::Timeout);
            }
        }
    }

    /**
     * Wait for a single specific task to report healthy. Used after a task
     * restart to confirm the new instance came up successfully.
     */
    pub async fn wait_for_health_check(&mut self, expected: Task) -> Result<(), HealthCheckError> {
        select! {
            res = self.track_single_health_check(expected) => {
                return res;
            },
            _ = sleep(Duration::from_millis(HealthCheck::HEALTH_CHECK_TIMEOUT_MS)) => {
                return Err(HealthCheckError::Timeout);
            }
        }
    }

    /**
     * Helper function to track incoming health checks
     */
    async fn track_health_checks(&mut self) -> Result<(), HealthCheckError> {
        while self.checked_tasks.len() <= Task::ENUM_COUNT {
            match self.hc_rx.recv().await {
                Some((task, status)) => {
                    if !status {
                        return Err(HealthCheckError::TaskFailure(task));
                    }

                    self.checked_tasks.insert(task);

                    if self.checked_tasks.len() >= Task::ENUM_COUNT {
                        break;
                    }
                }
                None => {
                    return Err(HealthCheckError::ChannelsClosed);
                }
            }
        }
        return Ok(());
    }

    /**
     * Helper that waits until `expected` reports healthy on the channel.
     * Any other tasks that emit checks in the meantime are still recorded
     * so we don't lose track of them.
     */
    async fn track_single_health_check(&mut self, expected: Task) -> Result<(), HealthCheckError> {
        loop {
            match self.hc_rx.recv().await {
                Some((task, status)) => {
                    if !status {
                        return Err(HealthCheckError::TaskFailure(task));
                    }
                    let is_match = task == expected;
                    self.checked_tasks.insert(task);
                    if is_match {
                        return Ok(());
                    }
                }
                None => {
                    return Err(HealthCheckError::ChannelsClosed);
                }
            }
        }
    }
}

pub type HealthCheckSender = mpsc::Sender<(Task, bool)>;

pub trait HealthCheckSenderExt {
    async fn send_health_check(&self, task: Task, status: bool);
}

impl HealthCheckSenderExt for HealthCheckSender {
    async fn send_health_check(&self, task: Task, status: bool) {
        if self.send((task.clone(), status)).await.is_err() {
            vprintln!("{:?} failed to send health check", task);
        }
    }
}

pub trait ResultExt<T> {
    async fn unwrap_or_fail_health_check(self, health_check_sender: &HealthCheckSender, task: Task) -> T;
}

impl<T, E: std::fmt::Debug> ResultExt<T> for Result<T, E> {
    async fn unwrap_or_fail_health_check(self, health_check_sender: &HealthCheckSender, task: Task) -> T {
        match self {
            Ok(r) => {
                return r;
            }
            Err(e) => {
                health_check_sender.send_health_check(task, false).await;
                panic!("{}", red(format!("{e:?}")));
            }
        }
    }
}