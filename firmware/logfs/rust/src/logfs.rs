use std::fs::OpenOptions;
use std::fs::File;
use std::io::{Read, Seek, SeekFrom, Write};
use std::os::unix::fs::OpenOptionsExt;
use std::path::Path;
use std::sync::{Arc, Mutex};

use crate::bindings;
use crate::bindings::*;

/// LogFsUnixDisk: Disk abstraction for a logfs image on a Unix machine.
pub struct LogFsUnixDisk {
    pub block_size: u32,
    pub block_count: u32,
    file: Arc<Mutex<File>>,
}

impl LogFsUnixDisk {
    pub fn new<P: AsRef<Path>>(block_size: u32, block_count: u32, disk_path: P) -> std::io::Result<Self> {
        let file = OpenOptions::new()
            .read(true)
            .write(true)
            .custom_flags(libc::O_SYNC)
            .open(disk_path)?;
        Ok(Self {
            block_size,
            block_count,
            file: Arc::new(Mutex::new(file)),
        })
    }

    pub fn write_block(&self, block: u32, data: &[u8]) -> std::io::Result<()> {
        let mut file = self.file.lock().unwrap();
        file.seek(SeekFrom::Start(self.block_size as u64 * block as u64))?;
        file.write_all(data)?;
        Ok(())
    }

    pub fn read_block(&self, block: u32) -> std::io::Result<Vec<u8>> {
        let mut file = self.file.lock().unwrap();
        file.seek(SeekFrom::Start(self.block_size as u64 * block as u64))?;
        let mut buf = vec![0u8; self.block_size as usize];
        file.read_exact(&mut buf)?;
        Ok(buf)
    }
}

impl Drop for LogFsUnixDisk {
    fn drop(&mut self) {
        // File will be closed automatically
    }
}

/// LogFsFile: Thin wrapper around a logfs file for Rust.
pub struct LogFsFile {
    pub file: bindings::LogFsFile,
    pub fs: *mut bindings::LogFs,
    pub block_size: u32,
}

impl LogFsFile {
    pub fn write(&mut self, data: &[u8]) -> Result<(), LogFsErr> {
        let err = unsafe {
            logfs_write(self.fs, &mut self.file, data.as_ptr() as *const _, data.len() as u32)
        };
        if err == LogFsErr_LOGFS_ERR_OK {
            Ok(())
        } else {
            Err(err)
        }
    }

    pub fn read(&mut self, size: Option<u32>) -> Result<Vec<u8>, LogFsErr> {
        let size = size.unwrap_or(self.block_size);
        let mut buf = vec![0u8; size as usize];
        let mut num_read: u32 = 0;
        let err = unsafe {
            logfs_read(
                self.fs,
                &mut self.file,
                buf.as_mut_ptr() as *mut _,
                size,
                LogFsReadFlags_LOGFS_READ_END,
                &mut num_read,
            )
        };
        if err == LogFsErr_LOGFS_ERR_OK {
            buf.truncate(num_read as usize);
            Ok(buf)
        } else {
            Err(err)
        }
    }

    pub fn close(&mut self) -> Result<(), LogFsErr> {
        let err = unsafe { logfs_close(self.fs, &mut self.file) };
        if err == LogFsErr_LOGFS_ERR_OK {
            Ok(())
        } else {
            Err(err)
        }
    }
}

/// LogFs: Thin wrapper around a logfs filesystem for Rust.
pub struct LogFs {
    pub fs: Box<bindings::LogFs>,
    pub cfg: Box<bindings::LogFsCfg>,
    pub disk: Arc<LogFsUnixDisk>,
}

impl LogFs {
    pub fn new(block_size: u32, block_count: u32, disk: Arc<LogFsUnixDisk>, write_cycles: u32, rd_only: bool) -> Self {
        let cfg = Box::new(bindings::LogFsCfg {
            context: std::ptr::null_mut(),
            block_size,
            block_count,
            read: None, // TODO: Implement read callback
            write: None, // TODO: Implement write callback
            cache: std::ptr::null_mut(),
            write_cycles,
            rd_only,
        });
        let fs = Box::new(bindings::LogFs {
            cfg: &*cfg as *const _,
            ..unsafe { std::mem::zeroed() }
        });
        Self { fs, cfg, disk }
    }

    pub fn mount(&mut self) -> Result<(), LogFsErr> {
        let err = unsafe { logfs_mount(&mut *self.fs, &*self.cfg) };
        if err == LogFsErr_LOGFS_ERR_OK {
            Ok(())
        } else {
            Err(err)
        }
    }

    pub fn format(&mut self) -> Result<(), LogFsErr> {
        let err = unsafe { logfs_format(&mut *self.fs, &*self.cfg) };
        if err == LogFsErr_LOGFS_ERR_OK {
            Ok(())
        } else {
            Err(err)
        }
    }

    pub fn open(&mut self, path: &str, flags: u32) -> Result<LogFsFile, LogFsErr> {
        let mut file = bindings::LogFsFile {
            ..unsafe { std::mem::zeroed() }
        };
        let c_path = std::ffi::CString::new(path).unwrap();
        let mut file_cfg = bindings::LogFsFileCfg {
            path: c_path.as_ptr(),
            cache: std::ptr::null_mut(),
        };
        let err = unsafe {
            logfs_open(&mut *self.fs, &mut file, &mut file_cfg, flags)
        };
        if err == LogFsErr_LOGFS_ERR_OK {
            Ok(LogFsFile {
                file,
                fs: &mut *self.fs,
                block_size: self.cfg.block_size,
            })
        } else {
            Err(err)
        }
    }

    pub fn cat(&mut self, path: &str, decode: Option<&str>) {
        match self.open(path, LogFsOpenFlags_LOGFS_OPEN_RD_ONLY) {
            Ok(mut file) => {
                // Read metadata
                // TODO: Implement metadata read
                let data = file.read(None);
                match data {
                    Ok(data) => {
                        println!("Data: {:?}", data);
                    }
                    Err(e) => {
                        println!("Read error: {:?}", e);
                    }
                }
                if let Some("can") = decode {
                    // TODO: Implement CAN decode branch
                    println!("TODO: decode == 'can' branch");
                }
            }
            Err(e) => {
                println!("Open error: {:?}", e);
            }
        }
    }
}
