use std::{fs, path::Path, sync::Arc};

use chrono::NaiveDate;
use crc::{Crc, CRC_8_SMBUS};
use influxdb2::models::DataPoint;
use jsoncan_rust::can_database::{CanDatabase, DecodedSignal};
use logfs::{LogFsErr, logfs::{LogFs, LogFsUnixDisk}};

use crate::{tasks::can_data::influx_util::{InfluxSignalSource, MAX_BATCH_CAPACITY, build_data_point, flush_buffer}, utils::yellow, vprintln};

/// CRC-8/SMBUS (poly 0x07, init 0x00, no reflection, no xorout) — matches the
/// firmware `app_crc8` used by `io_canLogging.c`.
const CRC8: Crc<u8> = Crc::<u8>::new(&CRC_8_SMBUS);

// 1 byte magic, 1 byte DLC, 2 bytes timestamp, 4 bytes ID.
const MSG_HDR_SIZE: usize = 1 + 1 + 2 + 4;
const MSG_MAGIC: u8 = 0xBA;


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

/// Decode the 7-byte start-timestamp metadata block (`IoRtcTime`):
/// `second, minute, hour, day, weekday, month, year-2000`. Returns epoch ms.
pub fn decode_start_timestamp_ms(metadata: &[u8]) -> Option<i64> {
    if metadata.len() < 7 {
        return None;
    }
    let second = metadata[0] as u32;
    let minute = metadata[1] as u32;
    let hour = metadata[2] as u32;
    let day = metadata[3] as u32;
    let month = metadata[5] as u32;
    let year = 2000 + metadata[6] as i32;

    NaiveDate::from_ymd_opt(year, month, day)?
        .and_hms_opt(hour, minute, second)
        .map(|dt| dt.and_utc().timestamp_millis())
}

/// A single decoded log frame: CAN id, absolute timestamp (epoch ms), payload.
pub struct CanLogFrame {
    pub id: u32,
    pub timestamp_ms: i64,
    pub payload: Vec<u8>,
}

/// Parse a raw CAN log into frames with absolute, overflow-corrected timestamps,
/// mirroring `Decoder.decode` in `firmware/logfs/python/logfs/can_logger.py`.
/// Validates each message's CRC8 and skips corrupt frames. Db-free so the
/// framing/overflow/CRC logic is unit-testable on its own.
pub fn parse_can_log_frames(metadata: &[u8], data: &[u8]) -> Vec<CanLogFrame> {
    // Layout per message (from `io_canLogging.c`):
    //   1. Magic 0xBA (1 byte)
    //   2. DLC (1 byte)
    //   3. Timestamp, ms (2 bytes)
    //   4. ID (4 bytes)
    //   5. Data bytes (DLC bytes)
    //   6. CRC8 over header+payload (1 byte)
    let start_ms = decode_start_timestamp_ms(metadata).unwrap_or(0);

    let mut frames: Vec<CanLogFrame> = Vec::new();
    let mut last_ms: i64 = 0;
    let mut overflow_fix_ms: i64 = 0;

    let mut i: usize = 0;
    while i < data.len() {
        // Scan for the magic byte marking the start of the next message.
        if data[i] != MSG_MAGIC {
            i += 1;
            continue;
        }

        // If a full header can't fit, the log is truncated mid-frame — done.
        // Without this the `data[i + 1]` etc. accesses below could panic on
        // pathologically truncated input (e.g. a trailing stray 0xBA).
        if i + MSG_HDR_SIZE > data.len() {
            break;
        }
        let dlc = data[i + 1] as usize;
        let msg_end = i + MSG_HDR_SIZE + dlc; // index of the CRC byte
        if msg_end >= data.len() {
            vprintln!("{}", yellow("Insufficient bytes for expected data length, skipping byte"));
            i += 1;
            continue;
        }

        let timestamp_ms = u16::from_le_bytes([data[i + 2], data[i + 3]]) as i64;
        let id = u32::from_le_bytes([data[i + 4], data[i + 5], data[i + 6], data[i + 7]]);
        let payload = data[i + MSG_HDR_SIZE..msg_end].to_vec();
        let rxed_crc = data[msg_end];

        if CRC8.checksum(&data[i..msg_end]) != rxed_crc {
            vprintln!("{}", yellow("CRC mismatch, skipping byte"));
            i += 1;
            continue;
        }
        i = msg_end + 1;

        // Undo the 16-bit timestamp overflow: a large backwards jump means the
        // counter wrapped past 2^16 ms (~65 s).
        let mut delta = timestamp_ms + overflow_fix_ms;
        if delta < last_ms - 30_000 {
            overflow_fix_ms += 1 << 16;
            delta += 1 << 16;
        }
        last_ms = delta;

        frames.push(CanLogFrame { id, timestamp_ms: start_ms + delta, payload });
    }

    frames
}

/// Decode a raw CAN log into signals, mirroring `Decoder.decode` in
/// `firmware/logfs/python/logfs/can_logger.py`.
pub fn decode_can_log(can_db: &CanDatabase, metadata: &[u8], data: &[u8]) -> Vec<DecodedSignal> {
    parse_can_log_frames(metadata, data)
        .into_iter()
        .flat_map(|frame| can_db.unpack(frame.id, frame.payload, Some(frame.timestamp_ms as u64)))
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn metadata(second: u8, minute: u8, hour: u8, day: u8, month: u8, year_off: u8) -> Vec<u8> {
        vec![second, minute, hour, day, 0, month, year_off]
    }

    /// Encode one message exactly as the firmware does, with a valid CRC8.
    fn encode_msg(id: u32, timestamp_ms: u16, payload: &[u8]) -> Vec<u8> {
        let mut header = vec![MSG_MAGIC, payload.len() as u8];
        header.extend_from_slice(&timestamp_ms.to_le_bytes());
        header.extend_from_slice(&id.to_le_bytes());

        let mut msg = header;
        msg.extend_from_slice(payload);
        let crc = CRC8.checksum(&msg);
        msg.push(crc);
        msg
    }

    #[test]
    fn crc8_matches_smbus_check_value() {
        // CRC-8/SMBUS check value for the ASCII string "123456789".
        assert_eq!(CRC8.checksum(b"123456789"), 0xF4);
    }

    #[test]
    fn start_timestamp_parses_to_epoch_ms() {
        // 2024-01-02 03:04:05 UTC.
        let md = metadata(5, 4, 3, 2, 1, 24);
        let expected = NaiveDate::from_ymd_opt(2024, 1, 2)
            .unwrap()
            .and_hms_opt(3, 4, 5)
            .unwrap()
            .and_utc()
            .timestamp_millis();
        assert_eq!(decode_start_timestamp_ms(&md), Some(expected));
    }

    #[test]
    fn start_timestamp_none_when_too_short() {
        assert_eq!(decode_start_timestamp_ms(&[1, 2, 3]), None);
    }

    #[test]
    fn frames_carry_absolute_timestamps() {
        let md = metadata(0, 0, 0, 1, 1, 24); // 2024-01-01 00:00:00 UTC
        let start_ms = decode_start_timestamp_ms(&md).unwrap();

        let mut data = encode_msg(0x100, 10, &[0xAA, 0xBB]);
        data.extend(encode_msg(0x200, 250, &[0x01]));

        let frames = parse_can_log_frames(&md, &data);
        assert_eq!(frames.len(), 2);
        assert_eq!(frames[0].id, 0x100);
        assert_eq!(frames[0].timestamp_ms, start_ms + 10);
        assert_eq!(frames[0].payload, vec![0xAA, 0xBB]);
        assert_eq!(frames[1].id, 0x200);
        assert_eq!(frames[1].timestamp_ms, start_ms + 250);
    }

    #[test]
    fn overflow_correction_adds_65536_on_wrap() {
        let md = metadata(0, 0, 0, 1, 1, 24);
        let start_ms = decode_start_timestamp_ms(&md).unwrap();

        // 60000 ms, then the 16-bit counter wraps back to 1000 ms.
        let mut data = encode_msg(0x1, 60_000, &[0x00]);
        data.extend(encode_msg(0x2, 1_000, &[0x00]));

        let frames = parse_can_log_frames(&md, &data);
        assert_eq!(frames.len(), 2);
        assert_eq!(frames[0].timestamp_ms, start_ms + 60_000);
        // Wrap detected: 1000 < 60000 - 30000, so add 2^16.
        assert_eq!(frames[1].timestamp_ms, start_ms + 1_000 + 65_536);
    }

    #[test]
    fn truncated_trailing_magic_does_not_panic() {
        // Regression: a stray 0xBA at/near the end (or any input too short to fit
        // a full header) must not panic via out-of-bounds indexing on `data[i+1]`.
        let md = metadata(0, 0, 0, 1, 1, 24);
        assert_eq!(parse_can_log_frames(&md, &[0xBA]).len(), 0);
        assert_eq!(parse_can_log_frames(&md, &[0x00, 0x00, 0xBA]).len(), 0);
        // A trailing magic right after a complete good frame must also not panic.
        let mut data = encode_msg(0x1, 100, &[0xAA]);
        data.push(0xBA);
        assert_eq!(parse_can_log_frames(&md, &data).len(), 1);
    }

    #[test]
    fn corrupt_crc_message_is_skipped() {
        let md = metadata(0, 0, 0, 1, 1, 24);
        let start_ms = decode_start_timestamp_ms(&md).unwrap();

        let mut bad = encode_msg(0x1, 100, &[0xDE, 0xAD]);
        *bad.last_mut().unwrap() ^= 0xFF; // corrupt the CRC byte
        let good = encode_msg(0x2, 200, &[0xBE]);

        let mut data = bad;
        data.extend(good);

        let frames = parse_can_log_frames(&md, &data);
        // The corrupt frame is dropped; the good one (eventually re-synced on the
        // next magic byte) is decoded.
        assert_eq!(frames.len(), 1);
        assert_eq!(frames[0].id, 0x2);
        assert_eq!(frames[0].timestamp_ms, start_ms + 200);
    }
}