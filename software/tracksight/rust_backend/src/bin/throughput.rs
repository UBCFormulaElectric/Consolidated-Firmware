// sender: cargo run --bin throughput -- --sender --port /dev/tty.usbserial-A --baud 57600
// receiver: cargo run --bin throughput -- --receiver --port /dev/tty.usbserial-B --baud 57600

use std::{
    fs,
    io::{self, Read, Write},
    path::{Path, PathBuf},
    sync::{
        Arc, Mutex,
        atomic::{AtomicBool, Ordering},
    },
    thread,
    time::{Duration, Instant},
};

use crc::{CRC_32_ISO_HDLC, Crc};
use crossterm::{
    event::{
        self, Event, KeyCode, KeyEventKind, KeyModifiers, KeyboardEnhancementFlags,
        PopKeyboardEnhancementFlags, PushKeyboardEnhancementFlags,
    },
    execute, terminal,
};
use plotters::prelude::*;

/*
    Radio throughput test. Run the same binary on both ends.
    Packets use the same framing as serial_handler.rs:
        [MAGIC 1, MAGIC 2, LENGTH, CRC32 (4, LE), PAYLOAD ...]
    Payload is a u32 LE sequence number followed by random bytes. The receiver validates
    framing + CRC and uses the sequence number to count dropped packets.
    On ctrl-c, per-second samples are written to CSV and plotted as SVGs in --out.
    With --hold-space, the sender only transmits while space is held down.
*/

const USAGE: &str = "usage: throughput (--sender | --receiver) --port <path> [--baud <rate>] [--size <payload bytes>] [--rate <bytes/s>] [--out <dir>] [--hold-space]";

// terminals without key release events only give key repeats, so space counts as
// released once repeats stop for this long (must exceed the OS initial repeat delay)
const HOLD_TIMEOUT: Duration = Duration::from_millis(600);

// mirrors TelemetryIncomingMessage in tasks/telem_message.rs (bins can't import from the backend crate)
const CRC32_CALC: Crc<u32> = Crc::<u32>::new(&CRC_32_ISO_HDLC);
const MAGIC: [u8; 2] = [0xaa, 0x55];
const HEADER_SIZE: usize = 7;
const MAX_PAYLOAD_SIZE: usize = 100;
const SEQ_SIZE: usize = 4;

enum Mode {
    Sender,
    Receiver,
}

struct Args {
    mode: Mode,
    port: String,
    baud: u32,
    size: usize,
    rate: Option<u64>,
    out: PathBuf,
    hold_space: bool,
}

fn parse_args() -> Result<Args, String> {
    let mut mode = None;
    let mut port = None;
    let mut baud = 57600;
    let mut size = MAX_PAYLOAD_SIZE;
    let mut rate = None;
    let mut out = None;
    let mut hold_space = false;

    let mut args = std::env::args().skip(1);
    while let Some(arg) = args.next() {
        let mut value = || args.next().ok_or(format!("missing value for {}", arg));
        match arg.as_str() {
            "--sender" => mode = Some(Mode::Sender),
            "--receiver" => mode = Some(Mode::Receiver),
            "--port" => port = Some(value()?),
            "--baud" => baud = value()?.parse().map_err(|e| format!("bad baud: {}", e))?,
            "--size" => size = value()?.parse().map_err(|e| format!("bad size: {}", e))?,
            "--rate" => rate = Some(value()?.parse().map_err(|e| format!("bad rate: {}", e))?),
            "--out" => out = Some(PathBuf::from(value()?)),
            "--hold-space" => hold_space = true,
            "-h" | "--help" => return Err(String::new()),
            _ => return Err(format!("unknown argument: {}", arg)),
        }
    }

    if !(SEQ_SIZE..=MAX_PAYLOAD_SIZE).contains(&size) {
        return Err(format!("size must be between {} and {}", SEQ_SIZE, MAX_PAYLOAD_SIZE));
    }

    let mode = mode.ok_or("specify --sender or --receiver")?;
    if hold_space && matches!(mode, Mode::Receiver) {
        return Err("--hold-space only applies to --sender".to_string());
    }
    let out = out.unwrap_or_else(|| {
        let name = match mode {
            Mode::Sender => "sender",
            Mode::Receiver => "receiver",
        };
        PathBuf::from("throughput_results")
            .join(format!("{}_{}", name, chrono::Local::now().format("%Y%m%d_%H%M%S")))
    });

    Ok(Args {
        mode,
        port: port.ok_or("specify --port")?,
        baud,
        size,
        rate,
        out,
        hold_space,
    })
}

/**
 * Counters accumulated by the worker thread, drained once a second by the reporter
 */
#[derive(Clone, Copy, Default)]
struct Counters {
    wire_bytes: u64,    // every byte written/read on the port
    payload_bytes: u64, // payload bytes of packets sent/received with valid CRC
    packets: u64,
    crc_errors: u64,
    length_errors: u64,
    dropped: u64, // sequence number gaps
}

struct Sample {
    t: f64,
    c: Counters,
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

    // long timeout like serial_handler, timeouts are only used to poll the running flag
    let port = match serialport::new(&args.port, args.baud)
        .timeout(Duration::from_millis(1000))
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

    // without --hold-space the sender is always sending
    let sending = Arc::new(AtomicBool::new(!args.hold_space));
    let watcher = args.hold_space.then(|| {
        let (running, sending) = (running.clone(), sending.clone());
        thread::spawn(move || watch_space(running, sending))
    });

    let counters = Arc::new(Mutex::new(Counters::default()));
    let worker = {
        let link = Link { port, running: running.clone(), counters: counters.clone() };
        match args.mode {
            Mode::Sender => thread::spawn({
                let (size, rate) = (args.size, args.rate);
                move || run_sender(link, size, rate, sending)
            }),
            Mode::Receiver => thread::spawn(move || run_receiver(link)),
        }
    };

    let samples = report(&running, &counters, worker);
    if let Some(watcher) = watcher {
        watcher.join().unwrap();
    }

    if let Err(e) = write_results(&args.out, &args.mode, &samples) {
        eprintln!("failed to write results: {}", e);
    } else {
        println!("results written to {}", args.out.display());
    }
}

/**
 * Serial port plus the shared state the worker threads need
 */
struct Link {
    port: Box<dyn serialport::SerialPort>,
    running: Arc<AtomicBool>,
    counters: Arc<Mutex<Counters>>,
}

impl Link {
    fn running(&self) -> bool {
        self.running.load(Ordering::SeqCst)
    }

    fn count(&self, f: impl FnOnce(&mut Counters)) {
        f(&mut self.counters.lock().unwrap());
    }

    /**
     * Blocking read_exact that retries on timeout until shutdown
     */
    fn read_exact(&mut self, buf: &mut [u8]) -> io::Result<()> {
        let mut index = 0;
        while index < buf.len() {
            match self.port.read(&mut buf[index..]) {
                Ok(n) => {
                    index += n;
                    self.count(|c| c.wire_bytes += n as u64);
                }
                Err(e) if e.kind() == io::ErrorKind::TimedOut && self.running() => {}
                Err(e) => return Err(e),
            }
        }
        Ok(())
    }

    /**
     * Blocking write_all that retries on timeout until shutdown. Plain write_all gives up
     * mid-packet when the radio applies flow control, which corrupts the stream
     */
    fn write_all(&mut self, buf: &[u8]) -> io::Result<()> {
        let mut index = 0;
        while index < buf.len() {
            match self.port.write(&buf[index..]) {
                Ok(n) => index += n,
                Err(e) if e.kind() == io::ErrorKind::TimedOut && self.running() => {}
                Err(e) => return Err(e),
            }
        }
        Ok(())
    }
}

//
// Sending
//

/**
 * Same framing as generate_packet in serial_handler.rs
 */
fn generate_packet(seq: u32, size: usize) -> Vec<u8> {
    let mut payload = vec![0u8; size];
    payload[..SEQ_SIZE].copy_from_slice(&seq.to_le_bytes());
    rand::fill(&mut payload[SEQ_SIZE..]);

    let checksum: [u8; 4] = CRC32_CALC.checksum(&payload).to_le_bytes();

    let mut packet = Vec::with_capacity(HEADER_SIZE + size);
    packet.extend_from_slice(&MAGIC);
    packet.push(size as u8);
    packet.extend_from_slice(&checksum);
    packet.extend_from_slice(&payload);
    packet
}

/**
 * Writes packets as fast as the port (or --rate) allows, while `sending` is set
 */
fn run_sender(mut link: Link, size: usize, rate: Option<u64>, sending: Arc<AtomicBool>) {
    let mut window_start = Instant::now();
    let mut sent_bytes = 0u64;
    let mut seq = 0u32;

    while link.running() {
        if !sending.load(Ordering::SeqCst) {
            // restart the rate window so a pause doesn't bank up a burst
            window_start = Instant::now();
            sent_bytes = 0;
            thread::sleep(Duration::from_millis(1));
            continue;
        }

        // throttle so we don't overrun the radio's buffer
        if let Some(rate) = rate {
            let allowed = (window_start.elapsed().as_secs_f64() * rate as f64) as u64;
            if sent_bytes >= allowed {
                thread::sleep(Duration::from_millis(1));
                continue;
            }
        }

        let packet = generate_packet(seq, size);
        match link.write_all(&packet) {
            Ok(()) => {
                sent_bytes += packet.len() as u64;
                seq = seq.wrapping_add(1);
                link.count(|c| {
                    c.wire_bytes += packet.len() as u64;
                    c.payload_bytes += size as u64;
                    c.packets += 1;
                });
            }
            Err(e) => {
                // timeouts only surface here on shutdown
                if !link.running() {
                    break;
                }
                eprint!("write error: {}\r\n", e);
                link.running.store(false, Ordering::SeqCst);
            }
        }
    }
}

/**
 * Sets `sending` while space is held. Puts the terminal in raw mode, so ctrl-c is handled here
 */
fn watch_space(running: Arc<AtomicBool>, sending: Arc<AtomicBool>) {
    terminal::enable_raw_mode().unwrap();
    // key release events need the kitty keyboard protocol (iTerm2, kitty, WezTerm, Ghostty)
    let releases = terminal::supports_keyboard_enhancement().unwrap_or(false);
    if releases {
        execute!(io::stdout(), PushKeyboardEnhancementFlags(KeyboardEnhancementFlags::REPORT_EVENT_TYPES)).ok();
        print!("hold space to send, ctrl-c to stop\r\n");
    } else {
        print!(
            "hold space to send, ctrl-c to stop (terminal doesn't report key releases, sending stops ~{}ms after letting go)\r\n",
            HOLD_TIMEOUT.as_millis()
        );
    }

    let mut last_press: Option<Instant> = None;
    while running.load(Ordering::SeqCst) {
        if event::poll(Duration::from_millis(10)).unwrap_or(false) {
            if let Ok(Event::Key(key)) = event::read() {
                match key.code {
                    KeyCode::Char('c') if key.modifiers.contains(KeyModifiers::CONTROL) => {
                        running.store(false, Ordering::SeqCst);
                    }
                    KeyCode::Char(' ') => match key.kind {
                        KeyEventKind::Release => last_press = None,
                        _ => last_press = Some(Instant::now()),
                    },
                    _ => {}
                }
            }
        }
        let held = last_press.is_some_and(|t| releases || t.elapsed() < HOLD_TIMEOUT);
        sending.store(held, Ordering::SeqCst);
    }

    sending.store(false, Ordering::SeqCst);
    if releases {
        execute!(io::stdout(), PopKeyboardEnhancementFlags).ok();
    }
    terminal::disable_raw_mode().ok();
}

//
// Receiving
//

enum PacketError {
    Length,
    Crc,
    Io(io::Error),
}

/**
 * Reading bytes until legitimate packet is formed, same as read_packet in serial_handler.rs
 */
fn read_packet(link: &mut Link) -> Result<Vec<u8>, PacketError> {
    let mut header_buffer = [0x0; HEADER_SIZE];

    // keep reading until magic bytes, rotating the buffer on mismatch
    link.read_exact(&mut header_buffer[..2]).map_err(PacketError::Io)?;
    while header_buffer[0..2] != MAGIC {
        header_buffer.rotate_left(1);
        link.read_exact(&mut header_buffer[1..2]).map_err(PacketError::Io)?;
    }

    link.read_exact(&mut header_buffer[2..]).map_err(PacketError::Io)?;

    let payload_length = header_buffer[2] as usize;
    if payload_length > MAX_PAYLOAD_SIZE || payload_length < SEQ_SIZE {
        return Err(PacketError::Length);
    }

    let mut payload_buffer = vec![0u8; payload_length];
    link.read_exact(&mut payload_buffer).map_err(PacketError::Io)?;

    let expected_crc = u32::from_le_bytes(header_buffer[3..7].try_into().unwrap());
    if expected_crc != CRC32_CALC.checksum(&payload_buffer) {
        return Err(PacketError::Crc);
    }

    Ok(payload_buffer)
}

fn run_receiver(mut link: Link) {
    let mut expected_seq: Option<u32> = None;

    while link.running() {
        match read_packet(&mut link) {
            Ok(payload) => {
                let seq = u32::from_le_bytes(payload[..SEQ_SIZE].try_into().unwrap());
                let gap = expected_seq.map_or(0, |e| seq.wrapping_sub(e));
                link.count(|c| {
                    c.packets += 1;
                    c.payload_bytes += payload.len() as u64;
                    // huge gap means seq went backwards, i.e. sender restarted
                    if gap < u32::MAX / 2 {
                        c.dropped += gap as u64;
                    }
                });
                expected_seq = Some(seq.wrapping_add(1));
            }
            Err(PacketError::Length) => link.count(|c| c.length_errors += 1),
            Err(PacketError::Crc) => link.count(|c| c.crc_errors += 1),
            Err(PacketError::Io(e)) => {
                if link.running() {
                    eprintln!("read error: {}", e);
                    link.running.store(false, Ordering::SeqCst);
                }
                break;
            }
        }
    }
}

//
// Reporting
//

/**
 * Prints and records one sample per second until ctrl-c
 */
fn report(running: &AtomicBool, counters: &Mutex<Counters>, worker: thread::JoinHandle<()>) -> Vec<Sample> {
    let start = Instant::now();
    let mut last_tick = start;
    let mut samples = Vec::new();
    let mut total = Counters::default();
    let mut worker = Some(worker);

    loop {
        // on shutdown, fall through once more to drain the final partial second
        let stopping = !running.load(Ordering::SeqCst);
        if stopping {
            // wait for the worker so its last counts are included
            worker.take().unwrap().join().unwrap();
        } else {
            thread::sleep(Duration::from_millis(50));
            if last_tick.elapsed() < Duration::from_secs(1) {
                continue;
            }
        }

        let now = Instant::now();
        let c = std::mem::take(&mut *counters.lock().unwrap());
        let dt = (now - last_tick).as_secs_f64();
        last_tick = now;

        total.wire_bytes += c.wire_bytes;
        total.payload_bytes += c.payload_bytes;
        total.packets += c.packets;
        total.crc_errors += c.crc_errors;
        total.length_errors += c.length_errors;
        total.dropped += c.dropped;

        print!(
            "{:>8.1} B/s wire | {:>8.1} B/s payload | {:>5} pkt | crc err {} | len err {} | dropped {} | error {:.2}%\r\n",
            c.wire_bytes as f64 / dt,
            c.payload_bytes as f64 / dt,
            c.packets,
            c.crc_errors,
            c.length_errors,
            c.dropped,
            error_rate(&c),
        );
        samples.push(Sample { t: (now - start).as_secs_f64(), c });

        if stopping {
            break;
        }
    }

    let elapsed = start.elapsed().as_secs_f64();
    print!(
        "\r\ndone: {:.1}s | avg {:.1} B/s wire, {:.1} B/s payload | {} pkt | crc err {} | len err {} | dropped {} | error {:.2}%\r\n",
        elapsed,
        total.wire_bytes as f64 / elapsed,
        total.payload_bytes as f64 / elapsed,
        total.packets,
        total.crc_errors,
        total.length_errors,
        total.dropped,
        error_rate(&total),
    );
    samples
}

/**
 * Percentage of packets that didn't make it through intact
 */
fn error_rate(c: &Counters) -> f64 {
    let bad = c.crc_errors + c.length_errors + c.dropped;
    let total = c.packets + bad;
    if total == 0 { 0.0 } else { bad as f64 / total as f64 * 100.0 }
}

fn write_results(out: &Path, mode: &Mode, samples: &[Sample]) -> Result<(), Box<dyn std::error::Error>> {
    fs::create_dir_all(out)?;

    let mut csv = fs::File::create(out.join("samples.csv"))?;
    writeln!(csv, "t,wire_bytes,payload_bytes,packets,crc_errors,length_errors,dropped,error_rate")?;
    for s in samples {
        let c = &s.c;
        writeln!(
            csv,
            "{:.3},{},{},{},{},{},{},{:.4}",
            s.t, c.wire_bytes, c.payload_bytes, c.packets, c.crc_errors, c.length_errors, c.dropped, error_rate(c)
        )?;
    }

    let series = |f: fn(&Counters) -> f64| -> Vec<(f64, f64)> { samples.iter().map(|s| (s.t, f(&s.c))).collect() };

    plot(
        &out.join("throughput.svg"),
        "Throughput",
        "bytes/s",
        &[
            ("wire", BLUE, series(|c| c.wire_bytes as f64)),
            ("valid payload", GREEN, series(|c| c.payload_bytes as f64)),
        ],
    )?;
    plot(
        &out.join("bitrate.svg"),
        "Bitrate",
        "kbps",
        &[("wire", BLUE, series(|c| c.wire_bytes as f64 * 8.0 / 1000.0))],
    )?;

    if let Mode::Receiver = mode {
        plot(
            &out.join("packets.svg"),
            "Packets per second",
            "packets",
            &[
                ("valid", GREEN, series(|c| c.packets as f64)),
                ("crc errors", RED, series(|c| c.crc_errors as f64)),
                ("length errors", MAGENTA, series(|c| c.length_errors as f64)),
                ("dropped", BLACK, series(|c| c.dropped as f64)),
            ],
        )?;
        plot(
            &out.join("error_rate.svg"),
            "Packet error rate",
            "% of packets",
            &[("error rate", RED, series(error_rate))],
        )?;
    }
    Ok(())
}

/**
 * Line chart of one or more series against time
 */
fn plot(
    path: &Path,
    title: &str,
    y_desc: &str,
    series: &[(&str, RGBColor, Vec<(f64, f64)>)],
) -> Result<(), Box<dyn std::error::Error>> {
    let points = || series.iter().flat_map(|(_, _, p)| p.iter());
    let max_t = points().map(|p| p.0).fold(1.0, f64::max);
    let max_y = points().map(|p| p.1).fold(0.0, f64::max).max(1.0) * 1.1;

    let root = SVGBackend::new(path, (1000, 500)).into_drawing_area();
    root.fill(&WHITE)?;

    let mut chart = ChartBuilder::on(&root)
        .caption(title, ("sans-serif", 24))
        .margin(15)
        .x_label_area_size(40)
        .y_label_area_size(70)
        .build_cartesian_2d(0.0..max_t, 0.0..max_y)?;
    chart.configure_mesh().x_desc("time (s)").y_desc(y_desc).draw()?;

    for (label, color, points) in series {
        let color = *color;
        chart
            .draw_series(LineSeries::new(points.iter().copied(), color.stroke_width(2)))?
            .label(*label)
            .legend(move |(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], color.stroke_width(2)));
    }
    chart
        .configure_series_labels()
        .position(SeriesLabelPosition::UpperRight)
        .background_style(WHITE.mix(0.8))
        .border_style(BLACK)
        .draw()?;

    root.present()?;
    Ok(())
}
