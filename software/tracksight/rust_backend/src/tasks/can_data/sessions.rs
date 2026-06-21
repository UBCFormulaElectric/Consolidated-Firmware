use std::{sync::Arc, time::Duration};

use chrono::{DateTime, FixedOffset};
use influxdb2::FromDataPoint;
use tokio::{select, time::sleep};

use crate::{config::CONFIG, error_println, vprintln, tasks::client_api::INFLUX_QUERY_TIMEOUT_MS};

#[derive(Debug, FromDataPoint, Default)]
pub struct SessionStarts {
    pub time: DateTime<FixedOffset>,
}

pub async fn get_sessions(
    start_utc: String,
    end_utc: String,
    source: String, 
    influx_client: Arc<influxdb2::Client>
) -> Result<Vec<(String, Option<String>)>, (String)> {
    let car_on_query = format!(r#"
        from(bucket: "{}")
        |> range(start: time(v: "{start_utc}"), stop: time(v: "{end_utc}"))
        |> filter(fn: (r) => r["_measurement"] == "{}")
        |> filter(fn: (r) => r["signal_name"] == "DAM_Alive")
        |> filter(fn: (r) => r["source"] == "{source}")
        |> sort(columns: ["_time"])
        "#, &CONFIG.influxdb_bucket, &CONFIG.influxdb_measurement);

    let req: Result<Vec<_>, influxdb2::RequestError> = select! {
        val = influx_client.query::<SessionStarts>(
            Some(influxdb2::models::Query::new(car_on_query))
        ) => val,
        _ = sleep(Duration::from_millis(INFLUX_QUERY_TIMEOUT_MS)) => {
            return Err("InfluxDB query timed out".to_string());
        }
    };

    fn to_unix(w: &SessionStarts) -> String {
        return w.time.timestamp_millis().to_string();
    }

    let time_bigram: Vec<(String, Option<String>)> = match req {
        Ok(starts) => {
            vprintln!("[sessions] influx returned {} DAM_Alive point(s)", starts.len());
            let mut tb: Vec<(String, Option<String>)> = starts.windows(2)
            .filter(|w| w[0].time < w[1].time)
            .map(|w|
                (
                    to_unix(&w[0]),
                    Some(to_unix(&w[1]))
                )
            )
            .collect();
            if let Some(last) = starts.last() {
                tb.push((to_unix(last), None));
            }
            tb
        },
        Err(e) => {
            error_println!("[sessions] INTERNAL_SERVER_ERROR: influx query failed: {}", e);
            return Err(format!("Error occurred while fetching session starts: {}", e));
        },
    }; 

    Ok(time_bigram)
}