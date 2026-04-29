use std::{fs, io::{self, Write}, path::Path, sync::Arc};

use logfs::{LogFsOpenFlags_LOGFS_OPEN_CREATE, LogFsOpenFlags_LOGFS_OPEN_RD_WR, logfs::*};

const COMMAND_SELECT_DISK: &str = "selectdisk";
const COMMAND_LS_DISK: &str = "lsdisk";
const COMMAND_FORMAT: &str = "format";
const COMMAND_MOUNT: &str = "mount";
const COMMAND_LS: &str = "ls";
const COMMAND_CD: &str = "cd";
const COMMAND_CAT: &str = "cat";
const COMMAND_WRITE: &str = "write";
const COMMAND_EXIT: &str = "exit";
const COMMAND_HELP: &str = "help";

static COMMANDS: &[(&str, &[&str])] = &[
    (COMMAND_SELECT_DISK, &["disk"]),
    (COMMAND_LS_DISK, &[]),
    (COMMAND_FORMAT, &[]),
    (COMMAND_MOUNT, &[]),
    (COMMAND_CAT, &["file"]),
    (COMMAND_WRITE, &["file", "data"]),
    (COMMAND_EXIT, &[]),
    (COMMAND_HELP, &[]),
];

/*
    Simple command interface to debug and interact with LogFS. Run with `cargo run --bin logfs_cmd`
    Not fully functional and working kinda jank
*/

fn main() {
    let stdin = io::stdin();
    let mut stdout = io::stdout();

    let mut logfs: Option<LogFs> = None;
    let mut curr_dir: Option<String> = None;

    loop {
        print!("{} > ", curr_dir.clone().unwrap_or_default());
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
                if find_detachable_drives().iter().any(|d| d == disk) {
                    println!("Disk '{}' selected.", disk);
                    let disk = Arc::new(LogFsUnixDisk::new(512, 1024 * 1024 * 15, Path::new(disk)).unwrap());
                    logfs = Some(LogFs::new(512, 1024 * 1024 * 15, disk, 0, false));
                } else {
                    println!("Disk '{}' not found among detachable drives.", disk);
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
                curr_dir = Some("/".to_string());
            }
            COMMAND_LS => {
                if let Some(ref mut l) = logfs {
                    let path = curr_dir.as_deref().unwrap_or("/");
                    match l.ls(path) {
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
                    let path = args[0];
                    let dir: String = curr_dir.clone().unwrap_or("/".to_string());
                    if let Ok(dirs) = l.ls(&dir) {
                        if dirs.contains(&path.to_string()) {
                            curr_dir = Some(dir + path);
                        } else if path.starts_with('/') {
                            // TODO absolute dir
                            curr_dir = Some("/".to_string()); 
                        } else {
                            eprintln!("Directory '{}' not found.", path);
                        }
                    } else {
                        eprintln!("Error changing directory.");
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_CAT => {
                if let Some(ref mut l) = logfs {
                    let path = curr_dir.as_deref().unwrap_or("/");

                    let file_name = args[0];
                    match l.cat(&format!("{}{}", path, file_name), None) {
                        Ok(data) => {
                            println!("{:?}", String::from_utf8(data));
                        },
                        Err(e) => eprintln!("Error opening file: {}", e),
                    }
                } else {
                    println!("No disk selected.");
                }
            }
            COMMAND_WRITE => {
                if let Some(ref mut l) = logfs {
                    let file_name = args[0];
                    let path = curr_dir.as_deref().unwrap_or("/");
                    let data = args[1].as_bytes();
                    println!("Writing to '{}{}': {}", path, file_name, String::from_utf8_lossy(data));
                    match l.open(&format!("{}{}", path, file_name), LogFsOpenFlags_LOGFS_OPEN_RD_WR | LogFsOpenFlags_LOGFS_OPEN_CREATE) {
                        Ok(mut file) => { 
                            match file.write(data) {
                                Ok(_) => {},
                                Err(e) => eprintln!("Error writing to file: {}", e),
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

fn validate_args(command: &str, arg_count: usize) -> bool {
    if let Some((_, expected)) = COMMANDS.iter().find(|(cmd, _)| *cmd == command) {
        if arg_count != expected.len() {
            println!(
                "Error: '{}' expects {} argument(s), got {}",
                command,
                expected.len(),
                arg_count
            );
            return false;
        }
    }
    true
}

fn print_help() {
    println!("Available commands:");
    for (cmd, args) in COMMANDS {
        println!("  {} {}", cmd, args.iter().map(|x| format!("<{x}>")).collect::<Vec<_>>().join(" "));
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