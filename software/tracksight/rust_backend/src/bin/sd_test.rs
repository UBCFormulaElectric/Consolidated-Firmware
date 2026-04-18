use std::{fs, path::Path, sync::Arc};

use logfs::{LogFsOpenFlags_LOGFS_OPEN_RD_WR, logfs::*};

fn main() {
    // test 1: check for sd card ✅
    let drives = find_detachable_drives();
    println!("Found drives: {:?}", drives);

    // test 2: format sd card with logfs
    let sd_card_dir = "/dev/sdb";
    let disk = Arc::new(LogFsUnixDisk::new(512, 1024 * 1024 * 15, Path::new(sd_card_dir)).unwrap());
    let mut logfs: LogFs = LogFs::new(512, 1024 * 1024 * 15, disk, 0, false);

    logfs.format().unwrap();
    logfs.mount().unwrap();
    let mut file = logfs.open("/.txt", LogFsOpenFlags_LOGFS_OPEN_RD_WR).unwrap();
    file.write(b"bruh").unwrap();

    let read = file.read(None).unwrap();

    println!("read from file: {}", read.iter().map(|&b| b as char).collect::<String>());
}

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