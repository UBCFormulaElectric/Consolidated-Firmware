use std::{fs, path::Path, sync::Arc};

use influxdb2::models::DataPoint;
use jsoncan_rust::can_database::{CanDatabase, DecodedSignal};
use logfs::{LogFsErr, logfs::{LogFs, LogFsUnixDisk}};

use crate::{can_log::parse_can_log_frames, tasks::can_data::influx_util::{InfluxSignalSource, MAX_BATCH_CAPACITY, build_data_point, flush_buffer}, vprintln};


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

#[derive(Debug)]
pub enum SdFormatError {
    NotDetachable,
    DiskOpenFailed,
    FormatFailed(LogFsErr),
}

pub fn format_drive(drive: &String) -> Result<(), SdFormatError> {
    if !find_detachable_drives().contains(drive) {
        return Err(SdFormatError::NotDetachable);
    }

    let disk = LogFsUnixDisk::new(512, 1024 * 1024 * 15, Path::new(drive))
        .map_err(|_| SdFormatError::DiskOpenFailed)?;
    let mut logfs = LogFs::new(512, 1024 * 1024 * 15, Arc::new(disk), 0, false);

    logfs.format().map_err(SdFormatError::FormatFailed)
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

#[derive(Debug)]
pub enum SdCardDumpError {
    LogFsError,
    FileNotFound,
    FileReadError,
}

pub async fn dump_sd_file(
    can_db: Arc<CanDatabase>,
    influxdb_client: Arc<influxdb2::Client>,
    drive: &String, path: &String
) -> Result<(), SdCardDumpError> {
    let (metadata, bytes) = {
        let mut logfs = match get_logfs(drive.clone()) {
            Ok(logfs) => logfs,
            Err(_) => return Err(SdCardDumpError::LogFsError),
        };

        let mut file = match logfs.open(&path, logfs::LogFsOpenFlags_LOGFS_OPEN_RD_ONLY) {
            Ok(f) => f,
            Err(_) => return Err(SdCardDumpError::FileNotFound),
        };

        let metadata = file.read_metadata(None).map_err(|e| {
            vprintln!("read_metadata failed: {:?}", e);
            SdCardDumpError::FileReadError
        })?;
        let data = file.read(None).map_err(|e| {
            vprintln!("read failed: {:?}", e);
            SdCardDumpError::FileReadError
        })?;
        (metadata, data)
        // logfs and file are dropped here, before any await
    };

    let decoded_signals = decode_can_log(&can_db, &metadata, &bytes);

    let mut influx_write_queue: Vec<DataPoint> = Vec::new();
    for signal in decoded_signals {
        match build_data_point(signal, InfluxSignalSource::SdCard) {
            Ok(dp) => influx_write_queue.push(dp),
            Err(e) => vprintln!("Error building data point: {}, skipping signal", e),
        }

        if influx_write_queue.len() >= MAX_BATCH_CAPACITY {
            flush_buffer(&mut influx_write_queue, &influxdb_client).await;
        }
    }

    flush_buffer(&mut influx_write_queue, &influxdb_client).await;

    Ok(())
}

/// Decode a raw CAN log into signals, mirroring `Decoder.decode` in
/// `firmware/logfs/python/logfs/can_logger.py`.
pub fn decode_can_log(can_db: &CanDatabase, metadata: &[u8], data: &[u8]) -> Vec<DecodedSignal> {
    parse_can_log_frames(metadata, data)
        .into_iter()
        .flat_map(|frame| can_db.unpack(frame.id, frame.payload, Some(frame.timestamp_ms as u64)))
        .collect()
}

