//! Dummy CAN traffic for the dimos dashboard.
//!
//! Every message transmitted on the bus(es) the dashboard node sits on is packed by
//! jsoncan_rust from the car's CAN JSON, with each signal sweeping through its valid
//! range. Frames go over UDP to the app's UdpCanSource, or to a SocketCAN interface
//! such as vcan0 on Linux.

use std::collections::HashSet;
use std::f64::consts::TAU;
use std::hash::{DefaultHasher, Hash, Hasher};
use std::net::UdpSocket;
use std::path::PathBuf;
use std::process::ExitCode;
use std::thread;
use std::time::{Duration, Instant};

use clap::Parser;
use jsoncan_rust::can_database::{CanDatabase, CanMessage, CanSignal, CanSignalType, DecodedSignal};
use jsoncan_rust::parsing::JsonCanParser;

#[derive(Parser, Debug)]
#[command(about = "Dummy CAN traffic for the dimos dashboard, packed by jsoncan_rust")]
struct Args {
    /// Car whose CAN JSON (can_bus/<car>) to simulate.
    #[arg(long, default_value = "hexray")]
    car: String,
    /// Node whose bus traffic to simulate (everything the other nodes on its buses transmit).
    #[arg(long, default_value = "DIMOS")]
    node: String,
    /// UDP address of the app's UdpCanSource.
    #[arg(long, default_value = "127.0.0.1:5005")]
    udp: String,
    /// Send to this SocketCAN interface (e.g. vcan0) instead of UDP. Linux only.
    #[arg(long)]
    socketcan: Option<String>,
    /// Lower bound on each message's period in ms; real cycle times are often 10 ms.
    #[arg(long, default_value_t = 50)]
    min_period_ms: u32,
}

/// UDP batches use the layout in lib/data/services/can_frames.dart:
/// repeated [u32 little-endian id][u8 payload length][payload].
const MAX_UDP_BATCH_BYTES: usize = 8 * 1024;
const APERIODIC_PERIOD_MS: u32 = 1000;
const STATS_INTERVAL: Duration = Duration::from_secs(5);

struct SimMessage {
    msg: CanMessage,
    // Sorted enum values for each signal (empty for non-enum signals).
    enum_values: Vec<Vec<u32>>,
    period: Duration,
    next_due: Instant,
    enabled: bool,
}

enum Output {
    Udp(UdpSocket, String),
    #[cfg(target_os = "linux")]
    SocketCan(socketcan::CanFdSocket),
}

impl Output {
    fn send(&self, frames: &[(u32, Vec<u8>)]) -> std::io::Result<()> {
        match self {
            Output::Udp(socket, addr) => {
                let mut batch = Vec::with_capacity(MAX_UDP_BATCH_BYTES);
                for (id, data) in frames {
                    if batch.len() + 5 + data.len() > MAX_UDP_BATCH_BYTES {
                        socket.send_to(&batch, addr)?;
                        batch.clear();
                    }
                    batch.extend_from_slice(&id.to_le_bytes());
                    batch.push(data.len() as u8);
                    batch.extend_from_slice(data);
                }
                if !batch.is_empty() {
                    socket.send_to(&batch, addr)?;
                }
                Ok(())
            }
            #[cfg(target_os = "linux")]
            Output::SocketCan(socket) => {
                use socketcan::{CanFdFrame, CanFrame, EmbeddedFrame, ExtendedId, Id, Socket, StandardId};
                for (raw_id, data) in frames {
                    let id: Id = match StandardId::new(*raw_id as u16) {
                        Some(id) if *raw_id <= 0x7FF => id.into(),
                        _ => ExtendedId::new(*raw_id).expect("CAN id out of range").into(),
                    };
                    if data.len() <= 8 {
                        socket.write_frame(&CanFrame::new(id, data).expect("classic frame"))?;
                    } else {
                        socket.write_frame(&CanFdFrame::new(id, data).expect("FD frame"))?;
                    }
                }
                Ok(())
            }
        }
    }
}

fn seed(name: &str) -> u64 {
    let mut hasher = DefaultHasher::new();
    name.hash(&mut hasher);
    hasher.finish()
}

/// A slow, per-signal sweep through the signal's valid range.
fn dummy_value(signal: &CanSignal, enum_values: &[u32], t: f64) -> f64 {
    let seed = seed(&signal.name);
    let period_s = 8.0 + (seed % 25) as f64;
    let phase = ((seed >> 8) % 1000) as f64 / 1000.0;
    let sweeps = t / period_s + phase;
    let wave = (TAU * sweeps).sin();

    match signal.signal_type {
        CanSignalType::Enum if !enum_values.is_empty() => {
            enum_values[(sweeps * enum_values.len() as f64) as usize % enum_values.len()] as f64
        }
        // Alerts are mostly off so the warning bar isn't permanently full.
        CanSignalType::Alert => f64::from(wave > 0.9),
        CanSignalType::Boolean => f64::from(wave > 0.0),
        _ if signal.max > signal.min => signal.min + (signal.max - signal.min) * (wave + 1.0) / 2.0,
        _ => signal.start_val,
    }
}

fn open_output(args: &Args) -> std::io::Result<(Output, String)> {
    if let Some(interface) = &args.socketcan {
        #[cfg(target_os = "linux")]
        {
            use socketcan::Socket;
            let socket = socketcan::CanFdSocket::open(interface)?;
            return Ok((Output::SocketCan(socket), format!("SocketCAN {interface}")));
        }
        #[cfg(not(target_os = "linux"))]
        {
            return Err(std::io::Error::new(
                std::io::ErrorKind::Unsupported,
                format!("--socketcan {interface}: SocketCAN is Linux only, use UDP instead"),
            ));
        }
    }
    let socket = UdpSocket::bind("127.0.0.1:0")?;
    Ok((Output::Udp(socket, args.udp.clone()), format!("UDP {}", args.udp)))
}

fn main() -> ExitCode {
    let args = Args::parse();

    let can_data_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("../../../../can_bus").join(&args.car);
    let db = match CanDatabase::from(JsonCanParser::new(can_data_dir.to_string_lossy().into_owned())) {
        Ok(db) => db,
        Err(e) => {
            eprintln!("Failed to load CAN JSON from {}: {e:?}", can_data_dir.display());
            return ExitCode::FAILURE;
        }
    };

    let bus_nodes: HashSet<&str> = db
        .buses
        .iter()
        .filter(|bus| bus.node_names.contains(&args.node))
        .flat_map(|bus| bus.node_names.iter().map(String::as_str))
        .collect();
    if bus_nodes.is_empty() {
        eprintln!("Node {} isn't on any bus in can_bus/{}", args.node, args.car);
        return ExitCode::FAILURE;
    }

    let start = Instant::now();
    let mut sims: Vec<SimMessage> = db
        .get_all_msgs()
        .expect("Failed to read messages from the CAN database")
        .into_iter()
        .filter(|msg| {
            msg.tx_node_name != args.node && bus_nodes.contains(msg.tx_node_name.as_str()) && !msg.signals.is_empty()
        })
        .map(|msg| {
            let period_ms = msg.cycle_time.unwrap_or(APERIODIC_PERIOD_MS).max(args.min_period_ms);
            let enum_values = msg
                .signals
                .iter()
                .map(|signal| {
                    let mut values: Vec<u32> = signal
                        .enum_name
                        .as_deref()
                        .and_then(|name| db.get_enum(name))
                        .map(|e| e.values.into_values().collect())
                        .unwrap_or_default();
                    values.sort_unstable();
                    values
                })
                .collect();
            SimMessage {
                // Stagger first sends so every message isn't due on the same tick.
                next_due: start + Duration::from_millis(seed(&msg.name) % u64::from(period_ms)),
                period: Duration::from_millis(period_ms.into()),
                enum_values,
                enabled: true,
                msg,
            }
        })
        .collect();

    let (output, destination) = match open_output(&args) {
        Ok(output) => output,
        Err(e) => {
            eprintln!("{e}");
            return ExitCode::FAILURE;
        }
    };
    let fd_count = sims.iter().filter(|s| s.msg.requires_fd()).count();
    println!(
        "Simulating {} messages ({} CAN FD) on {}'s bus from can_bus/{} -> {}",
        sims.len(),
        fd_count,
        args.node,
        args.car,
        destination
    );

    let mut frames_since_stats = 0usize;
    let mut last_stats = start;
    loop {
        let now = Instant::now();
        let t = now.duration_since(start).as_secs_f64();

        let mut frames = Vec::new();
        for sim in sims.iter_mut().filter(|s| s.enabled && s.next_due <= now) {
            sim.next_due += sim.period;
            if sim.next_due < now {
                sim.next_due = now + sim.period;
            }

            let signals: Vec<DecodedSignal> = sim
                .msg
                .signals
                .iter()
                .zip(&sim.enum_values)
                .map(|(signal, enum_values)| DecodedSignal {
                    name: signal.name.clone(),
                    value: dummy_value(signal, enum_values, t),
                    timestamp: None,
                    label: None,
                    unit: None,
                    signal_type: signal.signal_type.clone(),
                })
                .collect();
            match db.pack(&sim.msg.name, &signals) {
                Ok(frame) => frames.push(frame),
                Err(e) => {
                    eprintln!("Disabling {}: jsoncan couldn't pack it ({e:?})", sim.msg.name);
                    sim.enabled = false;
                }
            }
        }

        if let Err(e) = output.send(&frames) {
            eprintln!("Send failed: {e}");
        }
        frames_since_stats += frames.len();

        if now.duration_since(last_stats) >= STATS_INTERVAL {
            let rate = frames_since_stats as f64 / now.duration_since(last_stats).as_secs_f64();
            println!("{rate:.0} frames/s");
            frames_since_stats = 0;
            last_stats = now;
        }

        let next_due = sims.iter().filter(|s| s.enabled).map(|s| s.next_due).min().unwrap_or(now + STATS_INTERVAL);
        thread::sleep(next_due.saturating_duration_since(Instant::now()));
    }
}
