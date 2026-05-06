use std::{fs, path::Path, sync::Arc};

use axum::http::StatusCode;
use influxdb2::models::DataPoint;
use jsoncan_rust::can_database::CanDatabase;
use logfs::{LogFsErr, logfs::{LogFs, LogFsUnixDisk}};

use crate::{tasks::{can_data::influx_util::{MAX_BATCH_CAPACITY, build_data_point, flush_buffer}}, utils::yellow, vprintln};


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

pub async fn dump_sd_file(can_db: Arc<CanDatabase>, influxdb_client: Arc<influxdb2::Client>, drive: &String, path: &String) -> Result<(), StatusCode> {
    let bytes = {
        let mut logfs = match get_logfs(drive.clone()) {
            Ok(logfs) => logfs,
            Err(_) => return Err(StatusCode::BAD_REQUEST),
        };

        let mut file = match logfs.open(&path, logfs::LogFsOpenFlags_LOGFS_OPEN_RD_ONLY) {
            Ok(f) => f,
            Err(err) => return Err(StatusCode::BAD_REQUEST),
        };

        match file.read(None) {
            Ok(d) => d,
            Err(err) => return Err(StatusCode::INTERNAL_SERVER_ERROR),
        }
        // logfs and file are dropped here, before any await
    };

    // Parse based on how it's written in `io_canLogging.c`
    /*
        1. Magic number 0xBA (1 byte)
        2. DLC code (1 byte)
        3. Timestamp (2 bytes)
        4. ID (4 bytes)
        5. Data bytes (0-64 bytes)
        6. CRC8 checksum (1 byte)
     */
    let mut i: usize = 0;

    let mut influx_write_queue: Vec<DataPoint> = Vec::new();
    while i < bytes.len() {
        if bytes[i] != 0xBA {
            vprintln!("{}", yellow("Invalid magic number, skipping byte"));
            i += 1;
            continue;
        }
        let dlc = bytes[i+1];
        let timestamp = u16::from_le_bytes([bytes[i+2], bytes[i+3]]);
        let id = u32::from_le_bytes([bytes[i+4], bytes[i+5], bytes[i+6], bytes[i+7]]);
        let data = &bytes[i+8..i+8+(dlc as usize)];
        let _crc = bytes[i+8+(dlc as usize)];
        i += 9 + (dlc as usize);

        // TODO crc matching
        // if crc != CRC32_CALC.checksum(&data).to_le_bytes() {
        //     vprintln!("{}", yellow("CRC mismatch, skipping message"));
        //     continue;
        // }

        let decoded_signals = can_db.unpack(id, data.to_vec(), Some(timestamp as u64));

        //temp log
        println!("ID: {id}, Timestamp: {timestamp}, Data: {:02X?}, Decoded Signals: {:?}", data, decoded_signals);

        influx_write_queue.extend(decoded_signals.into_iter().filter_map(|x| match build_data_point(x) {
            Ok(dp) => Some(dp),
            Err(e) => {
                vprintln!("Error building data point: {}, skipping signal", e);
                None
            }
        }));

        if influx_write_queue.len() >= MAX_BATCH_CAPACITY {
            flush_buffer(&mut influx_write_queue, &influxdb_client).await;
        }

    }

    flush_buffer(&mut influx_write_queue, &influxdb_client).await;

    Ok(())
}