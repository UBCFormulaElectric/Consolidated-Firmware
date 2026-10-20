//! Decodes a LogFS CAN log into signal samples and writes them out as MF4, CSV or XLSX.
//! Only used by the `logfs_cmd` binary, which includes it next to `can_log.rs` and `mf4.rs`.

use std::{
    borrow::Cow,
    collections::HashMap,
    fs::File,
    io::{BufWriter, Write},
    path::Path,
};

use chrono::{DateTime, NaiveDateTime};
use jsoncan_rust::can_database::CanDatabase;
use rust_xlsxwriter::{Format, Workbook, Worksheet, XlsxError};

use crate::{
    can_log::{decode_start_timestamp_ms, parse_can_log_frames},
    mf4::{Mf4Signal, write_mf4},
};

#[derive(Clone, Copy, PartialEq, Debug)]
pub enum ExportFormat {
    Mf4,
    Csv,
    Xlsx,
}

impl ExportFormat {
    /// Parse a format argument: `mf4`, `csv`, `xlsx` or `all`.
    pub fn parse_list(arg: &str) -> Option<Vec<ExportFormat>> {
        match arg.to_ascii_lowercase().as_str() {
            "mf4" => Some(vec![ExportFormat::Mf4]),
            "csv" => Some(vec![ExportFormat::Csv]),
            "xlsx" => Some(vec![ExportFormat::Xlsx]),
            "all" => Some(vec![ExportFormat::Mf4, ExportFormat::Csv, ExportFormat::Xlsx]),
            _ => None,
        }
    }

    pub fn extension(self) -> &'static str {
        match self {
            ExportFormat::Mf4 => "mf4",
            ExportFormat::Csv => "csv",
            ExportFormat::Xlsx => "xlsx",
        }
    }
}

const TABLE_HEADER: [&str; 6] = ["time_s", "timestamp", "signal", "value", "label", "unit"];

// Excel's row limit, minus the header row. Longer logs continue on extra sheets.
const XLSX_ROWS_PER_SHEET: usize = 1_048_576 - 1;

struct Sample {
    /// Milliseconds since the log's start timestamp.
    offset_ms: i64,
    /// Index into `DecodedLog::signals`.
    signal: usize,
    value: f64,
    /// Enum value name, as an index into `DecodedLog::labels`.
    label: Option<usize>,
}

struct SignalInfo {
    name: String,
    unit: String,
}

/// Every decoded signal sample of one log, in log order. Names, units and labels are interned so a long log
/// doesn't carry a copy of each string per sample.
pub struct DecodedLog {
    /// Log start (the car's RTC wall-clock time at boot) as milliseconds since 1970-01-01.
    start_ms: i64,
    pub frame_count: usize,
    pub unknown_frames: usize,
    pub unknown_ids: usize,
    signals: Vec<SignalInfo>,
    labels: Vec<String>,
    samples: Vec<Sample>,
}

/// Decode a raw CAN log (metadata + data sections) into signal samples.
pub fn decode_log(can_db: &CanDatabase, metadata: &[u8], data: &[u8]) -> Result<DecodedLog, String> {
    let start_ms = decode_start_timestamp_ms(metadata).ok_or("missing start timestamp metadata (not a CAN log?)")?;
    let frames = parse_can_log_frames(metadata, data);

    let mut log = DecodedLog {
        start_ms,
        frame_count: frames.len(),
        unknown_frames: 0,
        unknown_ids: 0,
        signals: Vec::new(),
        labels: Vec::new(),
        samples: Vec::new(),
    };
    let mut signal_index: HashMap<String, usize> = HashMap::new();
    let mut label_index: HashMap<String, usize> = HashMap::new();
    // Unknown IDs are counted once instead of letting `unpack` print for every frame.
    let mut known_ids: HashMap<u32, bool> = HashMap::new();

    for frame in frames {
        let known = *known_ids.entry(frame.id).or_insert_with(|| can_db.get_message_by_id(frame.id).is_ok());
        if !known {
            log.unknown_frames += 1;
            continue;
        }
        let offset_ms = frame.timestamp_ms - start_ms;
        for s in can_db.unpack(frame.id, frame.payload, None) {
            let signal = *signal_index.entry(s.name).or_insert_with_key(|name| {
                log.signals.push(SignalInfo { name: name.clone(), unit: s.unit.unwrap_or_default() });
                log.signals.len() - 1
            });
            let label = s.label.map(|label| {
                *label_index.entry(label).or_insert_with_key(|label| {
                    log.labels.push(label.clone());
                    log.labels.len() - 1
                })
            });
            log.samples.push(Sample { offset_ms, signal, value: s.value, label });
        }
    }

    log.unknown_ids = known_ids.values().filter(|known| !**known).count();
    Ok(log)
}

impl DecodedLog {
    pub fn signal_count(&self) -> usize {
        self.signals.len()
    }

    pub fn write(&self, format: ExportFormat, out: &Path) -> Result<(), String> {
        match format {
            ExportFormat::Mf4 => self.write_mf4(out).map_err(|e| e.to_string()),
            ExportFormat::Csv => self.write_csv(out).map_err(|e| e.to_string()),
            ExportFormat::Xlsx => self.write_xlsx(out).map_err(|e| e.to_string()),
        }
    }

    fn timestamp(&self, sample: &Sample) -> NaiveDateTime {
        DateTime::from_timestamp_millis(self.start_ms + sample.offset_ms).unwrap_or_default().naive_utc()
    }

    fn write_mf4(&self, out: &Path) -> std::io::Result<()> {
        let mut signals: Vec<Mf4Signal> = self
            .signals
            .iter()
            .map(|info| Mf4Signal { name: info.name.clone(), unit: info.unit.clone(), samples: Vec::new() })
            .collect();
        for sample in &self.samples {
            signals[sample.signal].samples.push((sample.offset_ms as f64 / 1000.0, sample.value));
        }
        signals.sort_by(|a, b| a.name.cmp(&b.name));

        let start_ns = u64::try_from(self.start_ms).unwrap_or(0) * 1_000_000;
        write_mf4(out, start_ns, &signals)
    }

    /// One row per sample, in log order.
    fn write_csv(&self, out: &Path) -> std::io::Result<()> {
        let mut w = BufWriter::new(File::create(out)?);
        writeln!(w, "{}", TABLE_HEADER.join(","))?;
        for sample in &self.samples {
            let info = &self.signals[sample.signal];
            writeln!(
                w,
                "{:.3},{},{},{},{},{}",
                sample.offset_ms as f64 / 1000.0,
                self.timestamp(sample).format("%Y-%m-%d %H:%M:%S%.3f"),
                csv_field(&info.name),
                sample.value,
                csv_field(sample.label.map_or("", |l| &self.labels[l])),
                csv_field(&info.unit),
            )?;
        }
        w.flush()
    }

    /// Same table as the CSV. Uses constant-memory worksheets, which stream rows to disk as they're written.
    fn write_xlsx(&self, out: &Path) -> Result<(), XlsxError> {
        let mut workbook = Workbook::new();
        let header = Format::new().set_bold();
        let time_format = Format::new().set_num_format("0.000");
        let timestamp_format = Format::new().set_num_format("yyyy-mm-dd hh:mm:ss.000");

        let chunks: Vec<&[Sample]> = if self.samples.is_empty() {
            vec![&[]]
        } else {
            self.samples.chunks(XLSX_ROWS_PER_SHEET).collect()
        };
        for (i, chunk) in chunks.into_iter().enumerate() {
            let sheet = workbook.add_worksheet_with_constant_memory();
            sheet.set_name(if i == 0 { "Data".to_string() } else { format!("Data {}", i + 1) })?;
            setup_sheet(sheet, &header)?;

            for (row, sample) in chunk.iter().enumerate() {
                let row = row as u32 + 1;
                let info = &self.signals[sample.signal];
                sheet.write_number_with_format(row, 0, sample.offset_ms as f64 / 1000.0, &time_format)?;
                sheet.write_datetime_with_format(row, 1, self.timestamp(sample), &timestamp_format)?;
                sheet.write_string(row, 2, &info.name)?;
                sheet.write_number(row, 3, sample.value)?;
                if let Some(label) = sample.label {
                    sheet.write_string(row, 4, &self.labels[label])?;
                }
                if !info.unit.is_empty() {
                    sheet.write_string(row, 5, &info.unit)?;
                }
            }
        }

        workbook.save(out)
    }
}

fn setup_sheet(sheet: &mut Worksheet, header: &Format) -> Result<(), XlsxError> {
    for (col, (title, width)) in TABLE_HEADER.iter().zip([10, 24, 40, 14, 24, 8]).enumerate() {
        sheet.set_column_width(col as u16, width)?;
        sheet.write_string_with_format(0, col as u16, *title, header)?;
    }
    sheet.set_freeze_panes(1, 0)?;
    Ok(())
}

/// Quote a CSV field if it contains a separator, quote or newline.
fn csv_field(s: &str) -> Cow<'_, str> {
    if s.contains([',', '"', '\n', '\r']) {
        Cow::Owned(format!("\"{}\"", s.replace('"', "\"\"")))
    } else {
        Cow::Borrowed(s)
    }
}
