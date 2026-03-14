use std::{collections::HashSet, time::Duration};

use tokio::{select, sync::mpsc, time::sleep};

use crate::vprintln;

// im not even going to lie
// this is kind of an overkill for a health check on tasks
// it can probably also be implemented a lot more cleanly
// but it was cool so idk

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
}

impl Task {
    const ENUM_COUNT: usize = 5; // apparently rust doesn't let allow counting enums
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

    pub async fn wait_for_health_checks(self) -> Result<(), HealthCheckError> {
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
     * Helper function to track incoming health checks
     */
    async fn track_health_checks(mut self) -> Result<(), HealthCheckError> {
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
                panic!("{:?}", e);
            }
        }
    }
}