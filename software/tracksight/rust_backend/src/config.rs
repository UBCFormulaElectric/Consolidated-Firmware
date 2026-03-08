use std::net::IpAddr;
use std::{env::var, str::FromStr};
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
    pub backend_port: u16,
    pub mdns_local_ip: String,
}

pub static CONFIG: LazyLock<Config> = LazyLock::new(|| load_env_file());

const DEFAULT_BACKEND_ENV_FILE: &str = "backend.env";
fn load_env_file() -> Config {
    dotenv().ok();

    from_filename(DEFAULT_BACKEND_ENV_FILE)
        .expect(&format!("{} file not found, could not load env file!", DEFAULT_BACKEND_ENV_FILE));

    let serial_port: String = get_var::<String>("SERIAL_PORT").unwrap();

    let influxdb_url: String = get_var::<String>("INFLUXDB_URL").unwrap();

    let influxdb_org: String = get_var::<String>("INFLUXDB_ORG").unwrap();

    let influxdb_token: String = get_var::<String>("INFLUXDB_TOKEN").unwrap();
    
    let influxdb_bucket: String = get_var::<String>("INFLUXDB_BUCKET").unwrap();

    let car_name: String = get_var::<String>("CAR_NAME").unwrap();

    let influxdb_measurement: String = format!("{car_name}_live");

    // i love hardcoding
    let jsoncan_config_path: String = format!("../../../can_bus/{car_name}");

    let backend_port: u16 = get_var::<u16>("BACKEND_PORT").unwrap();

    // ohhh i love triple nested match
    // basically, if valid ipv4 address, use it, else search for default
    let mdns_local_ip: String = match match get_var::<String>("MDNS_LOCAL_IP") {
        Ok(ip) => {
            match ip.parse::<IpAddr>().and_then(|i| Ok(i.is_ipv4())) {
                Ok(_) => Some(ip),
                _ => None
            }
        }
        _ => None
    } {
        Some(ip) => ip,
        None => {
            let mut default_ip: String = String::new();
            for iface in if_addrs::get_if_addrs().unwrap() {
                if iface.addr.ip().is_ipv4() && (iface.name.starts_with("en") || iface.name.starts_with("eth")) {
                    default_ip = iface.addr.ip().to_string();
                    break;
                }
            }
            println!("Invalid Ipv4 address provided or empty, using first default: {}", default_ip);
            default_ip
        }
    };

    return Config {
        serial_port: serial_port,
        serial_baud_rate: 57600, // hardcoded baudrate, probably wont ever change this
        influxdb_url: influxdb_url,
        influxdb_org: influxdb_org,
        influxdb_token: influxdb_token,
        influxdb_bucket: influxdb_bucket,
        influxdb_measurement: influxdb_measurement,
        jsoncan_config_path: jsoncan_config_path,
        backend_port: backend_port,
        mdns_local_ip: mdns_local_ip
    }
}

fn get_var<T: std::str::FromStr>(env_key: &str) -> Result<T, <T as FromStr>::Err> {
    return var(env_key)
        .expect(&format!("{} is missing!", env_key))
        .parse::<T>()
}