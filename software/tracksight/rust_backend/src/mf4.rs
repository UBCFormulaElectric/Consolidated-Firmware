//! Minimal ASAM MDF 4.10 (.mf4) writer for decoded CAN signals.
//!
//! Each signal gets its own data group holding a sorted record of `[time: f64, value: f64]`, so signals with
//! different sample rates don't need a common timebase. Kept free of crate-internal dependencies so the
//! `logfs_cmd` binary can include it too.
//!
//! Blocks are written so that every link points backwards to an already-written block, except the header's
//! first-data-group link, which is patched once at the end.

use std::{
    fs::File,
    io::{self, BufWriter, Seek, SeekFrom, Write},
    path::Path,
};

pub struct Mf4Signal {
    pub name: String,
    pub unit: String,
    /// `(seconds since the measurement start, value)` pairs, in time order.
    pub samples: Vec<(f64, f64)>,
}

const ID_BLOCK_SIZE: u64 = 64;
const HD_LINK_COUNT: usize = 6;
const HD_DG_FIRST_LINK: usize = 0;
const HD_FH_FIRST_LINK: usize = 1;

// cn_type / cn_sync_type / cn_data_type values from the MDF 4 spec.
const CN_TYPE_VALUE: u8 = 0;
const CN_TYPE_MASTER: u8 = 2;
const CN_SYNC_NONE: u8 = 0;
const CN_SYNC_TIME: u8 = 1;
const CN_DATA_FLOAT_LE: u8 = 4;

/// Record layout: f64 time followed by f64 value.
const RECORD_BYTES: u32 = 16;

struct BlockWriter {
    out: BufWriter<File>,
    pos: u64,
}

impl BlockWriter {
    fn raw(&mut self, bytes: &[u8]) -> io::Result<()> {
        self.out.write_all(bytes)?;
        self.pos += bytes.len() as u64;
        Ok(())
    }

    /// Write a block (`##<id>` header, links, data, zero padding to 8 bytes). Returns its file offset.
    fn block(&mut self, id: &[u8; 2], links: &[u64], data: &[u8]) -> io::Result<u64> {
        let offset = self.pos;
        let unpadded = 24 + 8 * links.len() + data.len();
        let padding = (8 - unpadded % 8) % 8;

        let mut buf = Vec::with_capacity(unpadded + padding);
        buf.extend_from_slice(b"##");
        buf.extend_from_slice(id);
        buf.extend_from_slice(&[0; 4]);
        buf.extend_from_slice(&((unpadded + padding) as u64).to_le_bytes());
        buf.extend_from_slice(&(links.len() as u64).to_le_bytes());
        for link in links {
            buf.extend_from_slice(&link.to_le_bytes());
        }
        buf.extend_from_slice(data);
        buf.resize(unpadded + padding, 0);
        self.raw(&buf)?;
        Ok(offset)
    }

    /// Zero-terminated UTF-8 text block.
    fn text(&mut self, id: &[u8; 2], text: &str) -> io::Result<u64> {
        let mut data = text.as_bytes().to_vec();
        data.push(0);
        self.block(id, &[], &data)
    }

    /// DT block streamed straight from the samples, so big signals aren't copied into another buffer.
    fn data_block(&mut self, samples: &[(f64, f64)]) -> io::Result<u64> {
        let offset = self.pos;
        let len = 24 + samples.len() as u64 * RECORD_BYTES as u64;
        let mut header = Vec::with_capacity(24);
        header.extend_from_slice(b"##DT");
        header.extend_from_slice(&[0; 4]);
        header.extend_from_slice(&len.to_le_bytes());
        header.extend_from_slice(&0u64.to_le_bytes());
        self.raw(&header)?;
        for (t, v) in samples {
            self.raw(&t.to_le_bytes())?;
            self.raw(&v.to_le_bytes())?;
        }
        // Records are 16 bytes, so the block always ends 8-byte aligned.
        Ok(offset)
    }

    fn channel(
        &mut self,
        next: u64,
        name: u64,
        unit: u64,
        cn_type: u8,
        sync_type: u8,
        byte_offset: u32,
    ) -> io::Result<u64> {
        let mut data = Vec::with_capacity(72);
        data.push(cn_type);
        data.push(sync_type);
        data.push(CN_DATA_FLOAT_LE);
        data.push(0); // cn_bit_offset
        data.extend_from_slice(&byte_offset.to_le_bytes());
        data.extend_from_slice(&64u32.to_le_bytes()); // cn_bit_count
        data.extend_from_slice(&0u32.to_le_bytes()); // cn_flags
        data.extend_from_slice(&0u32.to_le_bytes()); // cn_inval_bit_pos
        data.push(0); // cn_precision
        data.push(0); // reserved
        data.extend_from_slice(&0u16.to_le_bytes()); // cn_attachment_count
        data.extend_from_slice(&[0; 6 * 8]); // value range / limits (unused, flags not set)
        // Links: cn_next, composition, tx_name, si_source, cc_conversion, data, md_unit, md_comment.
        self.block(b"CN", &[next, 0, name, 0, 0, 0, unit, 0], &data)
    }
}

/// Write `signals` to an .mf4 file at `path`. `start_time_ns` is the measurement start as nanoseconds since
/// 1970-01-01 in the car's local wall-clock time (the RTC has no timezone), which is flagged as local time.
pub fn write_mf4(path: &Path, start_time_ns: u64, signals: &[Mf4Signal]) -> io::Result<()> {
    let mut w = BlockWriter { out: BufWriter::new(File::create(path)?), pos: 0 };

    // ID block.
    let mut id = Vec::with_capacity(ID_BLOCK_SIZE as usize);
    id.extend_from_slice(b"MDF     ");
    id.extend_from_slice(b"4.10    ");
    id.extend_from_slice(b"logfscmd");
    id.extend_from_slice(&[0; 4]);
    id.extend_from_slice(&410u16.to_le_bytes());
    id.resize(ID_BLOCK_SIZE as usize, 0); // reserved + unfinalized flags (0 = finalized)
    w.raw(&id)?;

    // HD block; the data group and file history links are patched at the end.
    let mut hd = Vec::with_capacity(32);
    hd.extend_from_slice(&start_time_ns.to_le_bytes());
    hd.extend_from_slice(&0i16.to_le_bytes()); // tz offset (invalid with local time flag)
    hd.extend_from_slice(&0i16.to_le_bytes()); // dst offset
    hd.push(1); // hd_time_flags: bit 0 = local time
    hd.push(0); // hd_time_class: local PC reference time
    hd.push(0); // hd_flags: start angle/distance invalid
    hd.push(0); // reserved
    hd.extend_from_slice(&[0; 16]); // start angle, start distance
    let hd_offset = w.block(b"HD", &[0; HD_LINK_COUNT], &hd)?;

    // File history (mandatory in MDF 4).
    let fh_comment = w.text(
        b"MD",
        "<FHcomment><TX>Exported from LogFS</TX><tool_id>logfs_cmd</tool_id>\
         <tool_vendor>UBC Formula Electric</tool_vendor><tool_version>1.0</tool_version></FHcomment>",
    )?;
    let mut fh = Vec::with_capacity(16);
    fh.extend_from_slice(&start_time_ns.to_le_bytes());
    fh.extend_from_slice(&[0; 4]); // tz / dst offsets
    fh.push(1); // local time
    fh.extend_from_slice(&[0; 3]);
    let fh_offset = w.block(b"FH", &[0, fh_comment], &fh)?;

    // Data groups, written last-to-first so each DG's "next" link is already known.
    let mut next_dg = 0u64;
    for signal in signals.iter().rev().filter(|s| !s.samples.is_empty()) {
        let dt = w.data_block(&signal.samples)?;

        let time_name = w.text(b"TX", "time")?;
        let time_unit = w.text(b"TX", "s")?;
        let value_name = w.text(b"TX", &signal.name)?;
        let value_unit = if signal.unit.is_empty() { 0 } else { w.text(b"TX", &signal.unit)? };

        let value_cn = w.channel(0, value_name, value_unit, CN_TYPE_VALUE, CN_SYNC_NONE, 8)?;
        let time_cn = w.channel(value_cn, time_name, time_unit, CN_TYPE_MASTER, CN_SYNC_TIME, 0)?;

        let mut cg = Vec::with_capacity(32);
        cg.extend_from_slice(&0u64.to_le_bytes()); // cg_record_id
        cg.extend_from_slice(&(signal.samples.len() as u64).to_le_bytes()); // cg_cycle_count
        cg.extend_from_slice(&0u16.to_le_bytes()); // cg_flags
        cg.extend_from_slice(&0u16.to_le_bytes()); // cg_path_separator
        cg.extend_from_slice(&[0; 4]);
        cg.extend_from_slice(&RECORD_BYTES.to_le_bytes()); // cg_data_bytes
        cg.extend_from_slice(&0u32.to_le_bytes()); // cg_inval_bytes
        // Links: cg_next, cn_first, tx_acq_name, si_acq_source, sr_first, md_comment.
        let cg_offset = w.block(b"CG", &[0, time_cn, 0, 0, 0, 0], &cg)?;

        // Links: dg_next, cg_first, data, md_comment. Data: dg_rec_id_size (0 = sorted) + reserved.
        next_dg = w.block(b"DG", &[next_dg, cg_offset, dt, 0], &[0; 8])?;
    }

    // Patch the header links: 24-byte block header, then 8 bytes per link.
    let mut out = w.out.into_inner().map_err(|e| e.into_error())?;
    out.seek(SeekFrom::Start(hd_offset + 24 + 8 * HD_DG_FIRST_LINK as u64))?;
    out.write_all(&next_dg.to_le_bytes())?;
    out.seek(SeekFrom::Start(hd_offset + 24 + 8 * HD_FH_FIRST_LINK as u64))?;
    out.write_all(&fh_offset.to_le_bytes())?;
    out.flush()
}
