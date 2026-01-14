use clap::{Parser, arg, command};
use jsoncan_rust::can_database::CanDatabase;
use jsoncan_rust::parsing::JsonCanParser;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(long, default_value = "500000")]
    can_bit_rate: u32,
    #[arg(long)]
    can_data_dir: String,
}

const FRAME_BITS: u32 = 1 + 2 + 7; //SOF + ACK + EOF
const ID_BITS: u32 = 11;
const CONTROL_BITS: u32 = 1 + 6; // RTR + control
const DATA_BYTE_BITS: u32 = 8;
const CRC_BITS: u32 = 16;

// https://www.chiefdelphi.com/t/is-70-can-bus-utilization-bad/392683/2
const BAD_BUS_LOAD: u32 = 80;

fn msg_payload_bits(dlc: u16) -> u32 {
    FRAME_BITS + ID_BITS + CONTROL_BITS + DATA_BYTE_BITS * dlc as u32 + CRC_BITS
}

fn calculate_bus_load(can_database: &CanDatabase, can_bit_rate: u32) -> f32 {
    let mut bits_per_s: f32 = 0.0;
    for msg in &can_database.get_all_msgs().expect("no fail") {
        if let Some(cycle_time) = msg.cycle_time {
            let freq_hz = 1000.0 / cycle_time as f32;
            bits_per_s += msg_payload_bits(msg.dlc()) as f32 * freq_hz;
        }
    }
    bits_per_s / can_bit_rate as f32 * 100.0
}

fn report_bus_load(can_database: &CanDatabase, can_bit_rate: u32) {
    let estimated_bus_load = calculate_bus_load(can_database, can_bit_rate);
    if estimated_bus_load > BAD_BUS_LOAD as f32 {
        println!("⚠️: Estimated bus load is {}%", estimated_bus_load.round());
    } else {
        println!("👍 Estimated bus load: {}%", estimated_bus_load.round());
    }
}

fn main() {
    let args = Args::parse();
    let can_db = CanDatabase::from(JsonCanParser::new(args.can_data_dir)).unwrap();
    report_bus_load(&can_db, args.can_bit_rate);
}
