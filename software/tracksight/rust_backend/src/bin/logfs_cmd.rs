use std::{fs, io::{self, Write}, os::unix::fs::FileTypeExt, path::{Path, PathBuf}, sync::Arc};

use jsoncan_rust::{can_database::CanDatabase, parsing::JsonCanParser};
use logfs::{LogFsOpenFlags_LOGFS_OPEN_CREATE, LogFsOpenFlags_LOGFS_OPEN_RD_WR, logfs::*};

#[path = "../can_log.rs"]
mod can_log;
#[path = "../mf4.rs"]
mod mf4;
#[path = "../log_export.rs"]
mod log_export;

use log_export::{ExportFormat, decode_log};

const COMMAND_SELECT_DISK: &str = "selectdisk";
const COMMAND_LS_DISK: &str = "lsdisk";
const COMMAND_FORMAT: &str = "format";
const COMMAND_MOUNT: &str = "mount";
const COMMAND_LS: &str = "ls";
const COMMAND_CD: &str = "cd";
const COMMAND_CAT: &str = "cat";
const COMMAND_WRITE: &str = "write";
const COMMAND_EXPORT: &str = "export";
const COMMAND_EXIT: &str = "exit";
const COMMAND_HELP: &str = "help";

// Arguments ending in '?' are optional.
static COMMANDS: &[(&str, &[&str])] = &[
    (COMMAND_SELECT_DISK, &["disk"]),
    (COMMAND_LS_DISK, &[]),
    (COMMAND_FORMAT, &[]),
    (COMMAND_MOUNT, &[]),
    (COMMAND_LS, &[]),
    (COMMAND_CD, &["dir"]),
    (COMMAND_CAT, &["file"]),
    (COMMAND_WRITE, &["file", "data"]),
    (COMMAND_EXPORT, &["file|*", "mf4|csv|xlsx|all?"]),
    (COMMAND_EXIT, &[]),
    (COMMAND_HELP, &[]),
];

// Resolved at build time; the launcher always builds from this checkout.
const CAN_DB_DIR: &str = concat!(env!("CARGO_MANIFEST_DIR"), "/../../../can_bus/hexray");
const EXPORT_DIR: &str = concat!(env!("CARGO_MANIFEST_DIR"), "/../utils/logfs_cmd/exports");

// Not a CAN log (firmware boot counter), skipped by `export *`.
const BOOTCOUNT_FILE: &str = "bootcount.txt";

/*
    Simple command interface to debug and interact with LogFS. Run with `cargo run --bin logfs_cmd` 
    Not fully functional and working kinda jank
*/

fn main() {
    let stdin = io::stdin();
    let mut stdout = io::stdout();

    let cli = match parse_cli_args() {
        Ok(cli) => cli,
        Err(message) => {
            if !message.is_empty() {
                eprintln!("{}", message);
            }
            return;
        }
    };

    let mut logfs: Option<LogFs> = None;
    let mut can_db: Option<CanDatabase> = None; // loaded on first export

    let mut curr_disk: Option<String> = None;
    let mut curr_dir: Vec<String> = vec!["/".to_string()]; // always starts with root

    if let Some(disk) = cli.disk {
        if !is_block_device(&disk) {
            eprintln!("Disk '{}' is not an existing block device.", disk);
            return;
        }
        let unix_disk = Arc::new(LogFsUnixDisk::new(512, 1024 * 1024 * 15, Path::new(&disk)).unwrap());
        logfs = Some(LogFs::new(512, 1024 * 1024 * 15, unix_disk, 0, false));
        curr_disk = Some(disk);

        if let Some(export_file_name) = cli.export {
            let Some(ref mut filesystem) = logfs else { unreachable!() };
            if let Err(error) = filesystem.mount() {
                eprintln!("Error mounting disk: {}", error);
                return;
            }
            can_db = load_can_database();
            let Some(ref database) = can_db else { return };
            let files = if export_file_name == "*" {
                match filesystem.ls("/") {
                    Ok(entries) => entries.into_iter().filter(|entry| entry != BOOTCOUNT_FILE).collect(),
                    Err(error) => {
                        eprintln!("Error listing directory: {}", error);
                        return;
                    }
                }
            } else {
                vec![export_file_name]
            };
            for file_name in files {
                if let Err(error) = export_file(filesystem, database, &join_path("/", &file_name), &[cli.format]) {
                    eprintln!("Error exporting '{}': {}", file_name, error);
                }
            }
            return;
        }
    }

    loop {
        print!("<{}> {} > ", curr_disk.as_deref().unwrap_or_default(), dir_to_path(&curr_dir));
        stdout.flush().unwrap();

        let mut input = String::new();
        stdin.read_line(&mut input).unwrap();

        let parts: Vec<&str> = input.trim().split_whitespace().collect();
        if parts.is_empty() {
            continue;
        }

        let command = parts[0];
        let args = &parts[1..];
        if !validate_args(command, args.len()) {
            continue;
        }

        match command {
            COMMAND_SELECT_DISK => {
                let disk = args[0];
                if find_detachable_drives().iter().any(|d| d == disk) || is_block_device(disk) {
                    println!("Disk '{}' selected.", disk);
                    let unix_disk = Arc::new(LogFsUnixDisk::new(512, 1024 * 1024 * 15, Path::new(disk)).unwrap());
                    logfs = Some(LogFs::new(512, 1024 * 1024 * 15, unix_disk, 0, false));
                    curr_disk = Some(disk.to_string());
                } else {
                    println!("Disk '{}' is not an existing block device.", disk);
                }
            }
            COMMAND_LS_DISK => {
                let drives = find_detachable_drives();
                for drive in drives {
                    println!("  {}", drive);
                }
            }
            COMMAND_FORMAT => {
                if let Some(ref mut l) = logfs {
                    match l.format() {
                        Ok(_) => println!("Disk formatted."),
                        Err(e) => eprintln!("Error formatting disk: {}", e),
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_MOUNT => {
                if let Some(ref mut l) = logfs {
                    match l.mount() {
                        Ok(_) => println!("Disk mounted."),
                        Err(e) => eprintln!("Error mounting disk: {}", e),
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_LS => {
                if let Some(ref mut l) = logfs {
                    let path = dir_to_path(&curr_dir);
                    match l.ls(&path) {
                        Ok(entries) => {
                            for entry in entries {
                                println!("  {}", entry);
                            }
                        }
                        Err(e) => eprintln!("Error listing directory: {}", e),
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_CD => {
                if let Some(ref mut l) = logfs {
                    let target = args[0];
                    let new_dir = resolve_dir(&curr_dir, target);

                    // Root is always valid
                    if new_dir.len() <= 1 {
                        curr_dir = new_dir;
                    } else {
                        // Parent is everything except the last segment
                        let parent = &new_dir[..new_dir.len() - 1];
                        let parent_path = dir_to_path(parent);
                        let target_segment = new_dir.last().unwrap();

                        match l.ls(&parent_path) {
                            Ok(entries) if entries.contains(target_segment) => {
                                curr_dir = new_dir;
                            }
                            Ok(_) => eprintln!("Directory '{}' not found.", target),
                            Err(e) => eprintln!("Error changing directory: {}", e),
                        }
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_CAT => {
                if let Some(ref mut l) = logfs {
                    let file_name = args[0];
                    let path = join_path(&dir_to_path(&curr_dir), file_name);
                    match l.cat(&path) {
                        Ok((metadata, data)) => {
                            println!("metadata ({} bytes): {:02X?}", metadata.len(), metadata);
                            // Metadata layout written by firmware (7 bytes):
                            // [0] seconds, [1] minutes, [2] hours,
                            // [3] weekday (1=Mon..7=Sun, ignored here),
                            // [4] date (day), [5] month, [6] year-2000.
                            if metadata.len() >= 7 {
                                println!(
                                    "start timestamp: {:04}-{:02}-{:02} {:02}:{:02}:{:02}",
                                    2000 + metadata[6] as u32,
                                    metadata[5], metadata[4],
                                    metadata[2], metadata[1], metadata[0],
                                );
                            }
                            println!("data ({} bytes): {:02X?}", data.len(), data);
                        }
                        Err(e) => eprintln!("Error opening file: {}", e),
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_WRITE => {
                if let Some(ref mut l) = logfs {
                    let file_name = args[0];
                    let path = join_path(&dir_to_path(&curr_dir), file_name);
                    let data = args[1].as_bytes();
                    println!("Writing to '{}': {}", path, String::from_utf8_lossy(data));
                    match l.open(&path, LogFsOpenFlags_LOGFS_OPEN_RD_WR | LogFsOpenFlags_LOGFS_OPEN_CREATE) {
                        Ok(mut file) => {
                            if let Err(e) = file.write(data) {
                                eprintln!("Error writing to file: {}", e);
                            }
                            if file.close().is_err() {
                                eprintln!("Error closing file after write.");
                            }
                        }
                        Err(e) => eprintln!("Error opening file: {}", e),
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_EXPORT => {
                if let Some(ref mut l) = logfs {
                    let formats = match args.get(1) {
                        None => vec![ExportFormat::Mf4],
                        Some(arg) => match ExportFormat::parse_list(arg) {
                            Some(formats) => formats,
                            None => {
                                println!("Unknown export format '{}'. Use mf4, csv, xlsx or all.", arg);
                                continue;
                            }
                        },
                    };
                    if can_db.is_none() {
                        can_db = load_can_database();
                    }
                    let Some(ref db) = can_db else { continue };

                    let dir = dir_to_path(&curr_dir);
                    let files = if args[0] == "*" {
                        match l.ls(&dir) {
                            Ok(entries) => entries.into_iter().filter(|e| e != BOOTCOUNT_FILE).collect(),
                            Err(e) => {
                                eprintln!("Error listing directory: {}", e);
                                continue;
                            }
                        }
                    } else {
                        vec![args[0].to_string()]
                    };

                    for file_name in files {
                        let path = join_path(&dir, &file_name);
                        if let Err(e) = export_file(l, db, &path, &formats) {
                            eprintln!("Error exporting '{}': {}", path, e);
                        }
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_EXIT => {
                break;
            }
            COMMAND_HELP => {
                print_help();
            }
            _ => println!("Unknown command: '{}'. Type 'help' for available commands.", command),
        }
    }
}

fn load_can_database() -> Option<CanDatabase> {
    let dir = match fs::canonicalize(CAN_DB_DIR) {
        Ok(dir) => dir,
        Err(e) => {
            eprintln!("CAN database folder '{}' not found: {}", CAN_DB_DIR, e);
            return None;
        }
    };
    println!("Loading CAN database from {}...", dir.display());
    match CanDatabase::from(JsonCanParser::new(dir.to_string_lossy().into_owned())) {
        Ok(db) => Some(db),
        Err(e) => {
            eprintln!("Failed to load CAN database: {:?}", e);
            None
        }
    }
}

struct CliArgs {
    disk: Option<String>,
    export: Option<String>,
    format: ExportFormat,
}

fn parse_cli_args() -> Result<CliArgs, String> {
    let mut args = std::env::args().skip(1);
    let mut disk = None;
    let mut export = None;
    let mut format = ExportFormat::Mf4;

    while let Some(argument) = args.next() {
        match argument.as_str() {
            "--disk" => disk = Some(args.next().ok_or("--disk requires /dev/sdX")?),
            "--export" => export = Some(args.next().ok_or("--export requires a file name or *")?),
            "--format" => {
                let value = args.next().ok_or("--format requires mf4, csv, xlsx or all")?;
                let formats = ExportFormat::parse_list(&value).ok_or("--format requires mf4, csv, xlsx or all")?;
                if formats.len() != 1 {
                    return Err("command-line export supports one format at a time".to_string());
                }
                format = formats[0];
            }
            "-h" | "--help" => {
                println!("Usage: logfs_cmd [--disk /dev/sdX --export <file|*> [--format mf4|csv|xlsx]]");
                return Err(String::new());
            }
            unknown => return Err(format!("Unknown option '{}'. Use --help for usage.", unknown)),
        }
    }

    if export.is_some() && disk.is_none() {
        return Err("--export requires --disk /dev/sdX".to_string());
    }
    Ok(CliArgs { disk, export, format })
}

/// Export a CAN log on the card to `EXPORT_DIR/<file name>.<ext>` in each of `formats`.
fn export_file(logfs: &mut LogFs, can_db: &CanDatabase, path: &str, formats: &[ExportFormat]) -> Result<(), String> {
    let (metadata, data) = logfs.cat(path).map_err(|e| e.to_string())?;
    let log = decode_log(can_db, &metadata, &data)?;
    println!(
        "'{}': {} frames, {} signals{}",
        path,
        log.frame_count,
        log.signal_count(),
        if log.unknown_ids > 0 {
            format!(" ({} frames with {} IDs not in the CAN database skipped)", log.unknown_frames, log.unknown_ids)
        } else {
            String::new()
        }
    );

    let export_dir = Path::new(EXPORT_DIR);
    fs::create_dir_all(export_dir).map_err(|e| e.to_string())?;
    // logfs_cmd runs under sudo; hand the exports back to the invoking user so they aren't root-owned.
    chown_to_sudo_user(export_dir);
    let stem = Path::new(path).file_stem().ok_or("invalid file name")?;
    let base: PathBuf = fs::canonicalize(export_dir).map_err(|e| e.to_string())?.join(stem);

    for format in formats {
        let out = base.with_extension(format.extension());
        log.write(*format, &out)?;
        chown_to_sudo_user(&out);
        println!("  -> {}", out.display());
    }
    Ok(())
}

fn chown_to_sudo_user(path: &Path) {
    let id = |var| std::env::var(var).ok().and_then(|v| v.parse::<u32>().ok());
    if let (Some(uid), Some(gid)) = (id("SUDO_UID"), id("SUDO_GID")) {
        let _ = std::os::unix::fs::chown(path, Some(uid), Some(gid));
    }
}

fn validate_args(command: &str, arg_count: usize) -> bool {
    if let Some((_, expected)) = COMMANDS.iter().find(|(cmd, _)| *cmd == command) {
        let required = expected.iter().filter(|a| !a.ends_with('?')).count();
        if arg_count < required || arg_count > expected.len() {
            let count = if required == expected.len() {
                required.to_string()
            } else {
                format!("{}-{}", required, expected.len())
            };
            println!("Error: '{}' expects {} argument(s), got {}", command, count, arg_count);
            return false;
        }
    }
    true
}

fn print_help() {
    println!("Available commands:");
    for (cmd, args) in COMMANDS {
        println!("  {} {}", cmd, args.iter().map(|x| match x.strip_suffix('?') {
            Some(optional) => format!("[{optional}]"),
            None => format!("<{x}>"),
        }).collect::<Vec<_>>().join(" "));
    }
}

/**
 * Specific to Ubuntu 22.04
 */
fn find_detachable_drives() -> Vec<String> {
    let mut drives = Vec::new();
    let sys_block_path = "/sys/block";

    if let Ok(entries) = fs::read_dir(sys_block_path) {
        for entry in entries.flatten() {
            let path = entry.path();
            // Check if it's a removable device
            let removable_path = path.join("removable");
            if let Ok(content) = fs::read_to_string(&removable_path) {
                if content.trim() == "1" {
                    if let Some(name) = path.file_name() {
                        if let Some(name_str) = name.to_str() {
                            drives.push(format!("/dev/{}", name_str));
                        }
                    }
                }
            }
        }
    }
    drives
}

fn is_block_device(disk: &str) -> bool {
    fs::metadata(disk)
        .map(|metadata| metadata.file_type().is_block_device())
        .unwrap_or(false)
}

fn dir_to_path(dir: &[String]) -> String {
    if dir.len() == 1 {
        return "/".to_string();
    }
    // Skip the root "/" entry and join the rest with slashes
    format!("/{}", dir[1..].join("/"))
}

/// Join a directory path with a file name, avoiding a double leading slash when
/// the directory is root ("/"). logfs matches paths with an exact strcmp, so
/// "//log.bin" would not match a stored "/log.bin".
fn join_path(dir: &str, file_name: &str) -> String {
    format!("{}/{}", dir.trim_end_matches('/'), file_name)
}

fn resolve_dir(curr_dir: &[String], target: &str) -> Vec<String> {
    let mut new_dir = curr_dir.to_vec();

    // Absolute path: reset to root and parse from there
    let segments = if target.starts_with('/') {
        new_dir = vec!["/".to_string()];
        target.trim_matches('/').split('/').collect::<Vec<_>>()
    } else {
        target.split('/').collect::<Vec<_>>()
    };

    for segment in segments {
        match segment {
            "" | "." => {} // ignore empty segments and current-dir dots
            ".." => {
                // Never pop the root entry
                if new_dir.len() > 1 {
                    new_dir.pop();
                }
            }
            s => new_dir.push(s.to_string()),
        }
    }

    new_dir
}