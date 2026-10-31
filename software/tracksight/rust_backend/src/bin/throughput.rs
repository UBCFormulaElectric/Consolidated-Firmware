// sender: cargo run --bin throughput -- --sender --port /dev/tty.usbserial-A --baud 57600
// receiver: cargo run --bin throughput -- --receiver --port /dev/tty.usbserial-B --baud 57600

use std::{
    io::{self, Read, Write},
    sync::{
        Arc,
        atomic::{AtomicBool, AtomicU64, Ordering},
    },
    thread,
    time::{Duration, Instant},
};

/*
    Raw radio throughput test. Run the same binary on both ends:
        cargo run --bin throughput -- --sender   --port /dev/ttyUSB0 --baud 57600
        cargo run --bin throughput -- --receiver --port /dev/ttyUSB1 --baud 57600
    Sender writes an incrementing byte pattern, receiver counts bytes and pattern breaks.
*/

const USAGE: &str = "usage: throughput (--sender | --receiver) --port <path> [--baud <rate>] [--chunk <bytes>] [--rate <bytes/s>]";

enum Mode {
    Sender,
    Receiver,
}

struct Args {
    mode: Mode,
    port: String,
    baud: u32,
    chunk: usize,
    rate: Option<u64>,
}

fn parse_args() -> Result<Args, String> {
    let mut mode = None;
    let mut port = None;
    let mut baud = 57600;
    let mut chunk = 64;
    let mut rate = None;

    let mut args = std::env::args().skip(1);
    while let Some(arg) = args.next() {
        let mut value = || args.next().ok_or(format!("missing value for {}", arg));
        match arg.as_str() {
            "--sender" => mode = Some(Mode::Sender),
            "--receiver" => mode = Some(Mode::Receiver),
            "--port" => port = Some(value()?),
            "--baud" => baud = value()?.parse().map_err(|e| format!("bad baud: {}", e))?,
            "--chunk" => chunk = value()?.parse().map_err(|e| format!("bad chunk: {}", e))?,
            "--rate" => rate = Some(value()?.parse().map_err(|e| format!("bad rate: {}", e))?),
            "-h" | "--help" => return Err(String::new()),
            _ => return Err(format!("unknown argument: {}", arg)),
        }
    }

    Ok(Args {
        mode: mode.ok_or("specify --sender or --receiver")?,
        port: port.ok_or("specify --port")?,
        baud,
        chunk,
        rate,
    })
}

fn main() {
    let args = match parse_args() {
        Ok(args) => args,
        Err(message) => {
            if !message.is_empty() {
                eprintln!("{}", message);
            }
            eprintln!("{}", USAGE);
            std::process::exit(1);
        }
    };

    let port = match serialport::new(&args.port, args.baud)
        .timeout(Duration::from_millis(100))
        .open()
    {
        Ok(port) => port,
        Err(e) => {
            eprintln!("failed to open {}: {}", args.port, e);
            std::process::exit(1);
        }
    };

    let running = Arc::new(AtomicBool::new(true));
    {
        let running = running.clone();
        ctrlc::set_handler(move || running.store(false, Ordering::SeqCst)).unwrap();
    }

    let bytes = Arc::new(AtomicU64::new(0));
    let errors = Arc::new(AtomicU64::new(0));

    let worker = {
        let (running, bytes, errors) = (running.clone(), bytes.clone(), errors.clone());
        match args.mode {
            Mode::Sender => {
                thread::spawn(move || run_sender(port, args.chunk, args.rate, running, bytes))
            }
            Mode::Receiver => {
                thread::spawn(move || run_receiver(port, args.chunk, running, bytes, errors))
            }
        }
    };

    report(&running, &bytes, &errors);
    worker.join().unwrap();
}

/**
 * Writes an incrementing byte pattern as fast as the port (or --rate) allows
 */
fn run_sender(
    mut port: Box<dyn serialport::SerialPort>,
    chunk: usize,
    rate: Option<u64>,
    running: Arc<AtomicBool>,
    bytes: Arc<AtomicU64>,
) {
    let mut buf = vec![0u8; chunk];
    let mut next = 0u8;
    let start = Instant::now();

    while running.load(Ordering::SeqCst) {
        // throttle so we don't overrun the radio's buffer
        if let Some(rate) = rate {
            let allowed = (start.elapsed().as_secs_f64() * rate as f64) as u64;
            if bytes.load(Ordering::SeqCst) >= allowed {
                thread::sleep(Duration::from_millis(1));
                continue;
            }
        }

        for b in buf.iter_mut() {
            *b = next;
            next = next.wrapping_add(1);
        }
        match port.write_all(&buf) {
            Ok(()) => {
                bytes.fetch_add(buf.len() as u64, Ordering::SeqCst);
            }
            Err(e) if e.kind() == io::ErrorKind::TimedOut => {}
            Err(e) => {
                eprintln!("write error: {}", e);
                running.store(false, Ordering::SeqCst);
            }
        }
    }
}

/**
 * Reads raw bytes, counting breaks in the sender's incrementing pattern
 */
fn run_receiver(
    mut port: Box<dyn serialport::SerialPort>,
    chunk: usize,
    running: Arc<AtomicBool>,
    bytes: Arc<AtomicU64>,
    errors: Arc<AtomicU64>,
) {
    let mut buf = vec![0u8; chunk.max(1024)];
    let mut expected: Option<u8> = None;

    while running.load(Ordering::SeqCst) {
        let n = match port.read(&mut buf) {
            Ok(n) => n,
            Err(e) if e.kind() == io::ErrorKind::TimedOut => continue,
            Err(e) => {
                eprintln!("read error: {}", e);
                running.store(false, Ordering::SeqCst);
                break;
            }
        };

        for &b in &buf[..n] {
            // resync on mismatch so one dropped byte counts as one error
            if expected.is_some_and(|e| e != b) {
                errors.fetch_add(1, Ordering::SeqCst);
            }
            expected = Some(b.wrapping_add(1));
        }
        bytes.fetch_add(n as u64, Ordering::SeqCst);
    }
}

/**
 * Prints per-second and average throughput until ctrl-c
 */
fn report(running: &AtomicBool, bytes: &AtomicU64, errors: &AtomicU64) {
    let start = Instant::now();
    let mut last_bytes = 0;
    let mut last_tick = start;

    while running.load(Ordering::SeqCst) {
        thread::sleep(Duration::from_millis(100));
        if last_tick.elapsed() < Duration::from_secs(1) {
            continue;
        }

        let now = Instant::now();
        let total = bytes.load(Ordering::SeqCst);
        let rate = (total - last_bytes) as f64 / (now - last_tick).as_secs_f64();
        let avg = total as f64 / (now - start).as_secs_f64();
        println!(
            "{:>8.1} B/s ({:>6.2} kbps) | avg {:>8.1} B/s | total {} B | pattern errors {}",
            rate,
            rate * 8.0 / 1000.0,
            avg,
            total,
            errors.load(Ordering::SeqCst),
        );
        last_bytes = total;
        last_tick = now;
    }

    let elapsed = start.elapsed().as_secs_f64();
    let total = bytes.load(Ordering::SeqCst);
    let _ = io::stdout().flush();
    println!(
        "\ndone: {} B in {:.1}s, avg {:.1} B/s, pattern errors {}",
        total,
        elapsed,
        total as f64 / elapsed,
        errors.load(Ordering::SeqCst),
    );
}
