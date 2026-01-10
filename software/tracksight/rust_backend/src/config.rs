use std::env::var;
use std::sync::LazyLock;
use dotenv::{ dotenv, from_filename};

pub struct Config {
    pub serial_port: String,
    pub serial_baud_rate: u32,
    pub influxdb_url: String,
    pub influxdb_org: String,
    pub influxdb_token: String,
    pub influxdb_bucket: String,
    pub influxdb_measurement: String,
}

pub static CONFIG: LazyLock<Config> = LazyLock::new(|| load_env_file());

const DEFAULT_BACKEND_ENV_FILE: &str = "backend.env";
fn load_env_file() -> Config {
    dotenv().ok();

    from_filename(DEFAULT_BACKEND_ENV_FILE)
        .expect(&format!("{} file not found, could not load env file!", DEFAULT_BACKEND_ENV_FILE));

    let serial_port: String = var("SERIAL_PORT")
        .expect("SERIAL_PORT is missing!");

    let influxdb_url: String = var("INFLUXDB_URL")
        .expect("INFLUXDB_URL is missing!");

    let influxdb_org: String = var("INFLUXDB_ORG")
        .expect("INFLUXDB_ORG is missing!");

    let influxdb_token: String = var("INFLUXDB_TOKEN")
    .expect("INFLUXDB_TOKEN is missing!");
    
    let influxdb_bucket: String = var("INFLUXDB_BUCKET")
        .expect("INFLUXDB_BUCKET is missing!");

    let influxdb_measurement: String = format!(
        "{}_live", 
        var("CAR_NAME").expect("CAR_NAME is missing!")
    );

    return Config {
        serial_port: serial_port,
        serial_baud_rate: 57600, // hardcoded baudrate, probably wont ever change this
        influxdb_url: influxdb_url,
        influxdb_org: influxdb_org,
        influxdb_token: influxdb_token,
        influxdb_bucket: influxdb_bucket,
        influxdb_measurement: influxdb_measurement,
    }
}