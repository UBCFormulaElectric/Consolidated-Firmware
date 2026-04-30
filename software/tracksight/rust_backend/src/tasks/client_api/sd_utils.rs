use std::{fs, path::Path, sync::Arc};

use logfs::{LogFsErr, logfs::{LogFs, LogFsUnixDisk}};


/**
 * Specific to Ubuntu
 */
pub fn find_detachable_drives() -> Vec<String> {
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

pub fn get_logfs(drive: String) -> Result<LogFs, ()> {
    if !find_detachable_drives().contains(&drive) {
        return Err(());
    }

    if let Ok(disk) = LogFsUnixDisk::new(512, 1024 * 1024 * 15, Path::new(&drive)) {
        let mut logfs: LogFs = LogFs::new(512, 1024 * 1024 * 15, Arc::new(disk), 0, false);
        logfs.mount().map_err(|_| ())?; // todo should have more verbose error
        return Ok(logfs);
    }
    return Err(());
}

/**
 * Unrestricted depth recursive file (not dir) search, shouldn't be an issue since sd card should be shallow enough
 */
pub fn ls_deep(mut logfs: LogFs) -> Result<Vec<String>, LogFsErr> {
    let mut results = Vec::new();
    ls_recursive(&mut logfs, "/", &mut results)?;
    Ok(results)
}

fn ls_recursive(logfs: &mut LogFs, dir: &str, results: &mut Vec<String>) -> Result<(), LogFsErr> {
    let entries = logfs.ls(dir)?;
    for name in entries {
        let full_path = format!("{}/{}", dir.trim_end_matches('/'), name);
        // todo maybe check if it's possible to distinguish file from dir ?
        results.push(full_path.clone());
        let _ = ls_recursive(logfs, &full_path, results);
    }
    Ok(())
}