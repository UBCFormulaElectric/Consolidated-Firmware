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
    pub jsoncan_config_path: String,
    pub backend_port: u32,
}

pub static CONFIG: LazyLock<Config> = LazyLock::new(|| load_env_file());

const DEFAULT_BACKEND_ENV_FILE: &str = "backend.env";
fn load_env_file() -> Config {
    dotenv().ok();

    from_filename(DEFAULT_BACKEND_ENV_FILE)
        .expect(&format!("{} file not found, could not load env file!", DEFAULT_BACKEND_ENV_FILE));

    let serial_port: String = get_var::<String>("SERIAL_PORT");

    let influxdb_url: String = get_var::<String>("INFLUXDB_URL");

    let influxdb_org: String = get_var::<String>("INFLUXDB_ORG");

    let influxdb_token: String = get_var::<String>("INFLUXDB_TOKEN");
    
    let influxdb_bucket: String = get_var::<String>("INFLUXDB_BUCKET");

    let car_name = get_var::<String>("CAR_NAME");

    let influxdb_measurement: String = format!("{car_name}_live");

    let backend_port: u32 = get_var::<u32>("BACKEND_PORT");

    return Config {
        serial_port: serial_port,
        serial_baud_rate: 57600, // hardcoded baudrate, probably wont ever change this
        influxdb_url: influxdb_url,
        influxdb_org: influxdb_org,
        influxdb_token: influxdb_token,
        influxdb_bucket: influxdb_bucket,
        influxdb_measurement: influxdb_measurement,
        jsoncan_config_path: format!("../../../can_bus/{car_name}"), // i love hardcoding
        backend_port: backend_port
    }
}

fn get_var<T: std::str::FromStr>(env_key: &str) -> T {
    return var(env_key)
        .expect(&format!("{} is missing!", env_key))
        .parse::<T>()
        .ok().unwrap();
}