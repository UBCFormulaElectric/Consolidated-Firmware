use std::{ffi::CStr, sync::Arc};

use crate::{
    bindings::*,
    logfs::{LogFs, LogFsDisk, LogFsFile, LogFsRamDisk},
};

const FS_SIZE_BYTES: u32 = 2 * 1024 * 1024;
const SMALL_BLOCK_COUNT: u32 = 11;
const SMALL_WRITE_CYCLES: u32 = 10;

fn make_fs(block_size: u32, block_count: u32, write_cycles: u32) -> LogFs {
    let disk = Arc::new(LogFsRamDisk::new(block_size, block_count).unwrap());
    let mut fs = LogFs::new(block_size, block_count, disk, write_cycles, false);
    fs.format().unwrap();
    fs.mount().unwrap();
    fs
}

fn make_small_fs() -> LogFs {
    make_fs(512, SMALL_BLOCK_COUNT, SMALL_WRITE_CYCLES)
}

fn data(size: usize, seed: usize) -> Vec<u8> {
    (0..size)
        .map(|i| {
            let value = (i * 37 + seed) % 36;
            if value < 10 {
                b'0' + value as u8
            } else {
                b'A' + (value - 10) as u8
            }
        })
        .collect()
}

fn create_file(fs: &mut LogFs, path: &str) -> LogFsFile {
    fs.open(
        path,
        (LogFsOpenFlags_LOGFS_OPEN_RD_WR | LogFsOpenFlags_LOGFS_OPEN_CREATE) as u32,
    )
    .unwrap()
}

fn read_all(file: &mut LogFsFile) -> Vec<u8> {
    file.read(None).unwrap()
}

fn list_paths(fs: &mut LogFs) -> Vec<String> {
    let mut path = unsafe { std::mem::zeroed::<LogFsPath>() };
    let mut paths = Vec::new();
    let mut err = unsafe { logfs_firstPath(&mut *fs.fs, &mut path) };

    while err == LogFsErr_LOGFS_ERR_OK {
        let path_str = unsafe { CStr::from_ptr(path.path.as_ptr()) }
            .to_string_lossy()
            .into_owned();
        if path_str != "/.root" {
            paths.push(path_str);
        }
        err = unsafe { logfs_nextPath(&mut *fs.fs, &mut path) };
    }

    assert_eq!(err, LogFsErr_LOGFS_ERR_NO_MORE_FILES);
    paths
}

#[test]
fn rw_big_file() {
    for block_size in [128, 512, 1024] {
        let block_count = FS_SIZE_BYTES / block_size;
        for write_cycles in [0, 1, 10, 100] {
            for size in [1, 10, 100, 1_000, 10_000, 100_000, 1_000_000] {
                let mut fs = make_fs(block_size, block_count, write_cycles);
                let expected = data(size, 1);
                let mut file = create_file(&mut fs, "/test.txt");

                assert_eq!(read_all(&mut file), b"");
                assert_eq!(file.size().unwrap(), 0);

                file.write(&expected).unwrap();
                assert_eq!(read_all(&mut file), expected);
                assert_eq!(file.size().unwrap(), size as u32);
                file.close().unwrap();
            }
        }
    }
}

#[test]
fn rw_multiple_files() {
    for block_size in [128, 512, 1024] {
        let block_count = FS_SIZE_BYTES / block_size;
        for write_cycles in [0, 1, 10, 100] {
            for num_files in [1, 5, 10, 100] {
                for data_size in [1, 10, 1_000] {
                    let mut fs = make_fs(block_size, block_count, write_cycles);
                    let file_data: Vec<_> = (0..num_files)
                        .map(|i| data(data_size, i as usize))
                        .collect();
                    let mut files: Vec<_> = (0..num_files)
                        .map(|i| create_file(&mut fs, &format!("/test{i}.txt")))
                        .collect();

                    for (file, expected) in files.iter_mut().zip(&file_data) {
                        assert_eq!(read_all(file), b"");
                        assert_eq!(file.size().unwrap(), 0);
                        file.write(expected).unwrap();
                    }
                    for (file, expected) in files.iter_mut().zip(&file_data) {
                        assert_eq!(read_all(file), *expected);
                        assert_eq!(file.size().unwrap(), expected.len() as u32);
                    }

                    for (file, expected) in files.iter_mut().zip(&file_data) {
                        file.write(expected).unwrap();
                    }
                    for (file, expected) in files.iter_mut().zip(&file_data) {
                        assert_eq!(read_all(file), [expected.as_slice(), expected].concat());
                        assert_eq!(file.size().unwrap(), (expected.len() * 2) as u32);
                    }

                    for (file, expected) in files.iter_mut().zip(&file_data) {
                        file.write(expected).unwrap();
                    }
                    for (file, expected) in files.iter_mut().zip(&file_data) {
                        assert_eq!(
                            read_all(file),
                            [expected.as_slice(), expected, expected].concat()
                        );
                        assert_eq!(file.size().unwrap(), (expected.len() * 3) as u32);
                    }
                    for file in &mut files {
                        file.close().unwrap();
                    }
                }
            }
        }
    }
}

#[test]
fn open_existing_file_and_append() {
    for block_size in [128, 512, 1024] {
        for write_cycles in [0, 1, 10, 100] {
            for size in [1, 10, 100, 1_000, 10_000] {
                let mut fs = make_fs(block_size, FS_SIZE_BYTES / block_size, write_cycles);
                let first = data(size, 2);
                let second = data(size, 3);
                let mut file = create_file(&mut fs, "/test.txt");
                assert_eq!(read_all(&mut file), b"");
                assert_eq!(file.size().unwrap(), 0);
                file.write(&first).unwrap();
                assert_eq!(read_all(&mut file), first);
                assert_eq!(file.size().unwrap(), size as u32);
                file.close().unwrap();

                let mut file = fs
                    .open("/test.txt", LogFsOpenFlags_LOGFS_OPEN_RD_WR as u32)
                    .unwrap();
                assert_eq!(read_all(&mut file), first);
                file.write(&second).unwrap();
                assert_eq!(read_all(&mut file), [first.as_slice(), &second].concat());
                assert_eq!(file.size().unwrap(), (size * 2) as u32);
                file.close().unwrap();

                let mut file = fs
                    .open("/test.txt", LogFsOpenFlags_LOGFS_OPEN_RD_ONLY as u32)
                    .unwrap();
                assert_eq!(read_all(&mut file), [first, second].concat());
                file.close().unwrap();
            }
        }
    }
}

#[test]
fn remount_preserves_file_data() {
    for block_size in [128, 512, 1024] {
        for write_cycles in [0, 1, 10, 100] {
            for size in [1, 10, 100, 1_000, 10_000] {
                let mut fs = make_fs(block_size, FS_SIZE_BYTES / block_size, write_cycles);
                let expected = data(size, 4);
                let mut file = create_file(&mut fs, "/test.txt");
                file.write(&expected).unwrap();
                assert_eq!(read_all(&mut file), expected);
                assert_eq!(file.size().unwrap(), size as u32);
                file.close().unwrap();

                fs.mount().unwrap();
                let mut file = fs
                    .open("/test.txt", LogFsOpenFlags_LOGFS_OPEN_RD_WR as u32)
                    .unwrap();
                assert_eq!(read_all(&mut file), expected);
                assert_eq!(file.size().unwrap(), size as u32);
                file.write(&expected).unwrap();
                assert_eq!(
                    read_all(&mut file),
                    [expected.as_slice(), &expected].concat()
                );
                assert_eq!(file.size().unwrap(), (size * 2) as u32);
                file.close().unwrap();
            }
        }
    }
}

#[test]
fn read_entire_file_including_large_files() {
    for block_size in [128, 512, 1024] {
        for write_cycles in [0, 1, 10, 100] {
            for size in [1, 10, 100, 1_000, 10_000, 1_100_000] {
                let mut fs = make_fs(block_size, FS_SIZE_BYTES / block_size, write_cycles);
                let expected = data(size, 5);
                let mut file = create_file(&mut fs, "/test.txt");
                file.write(&expected).unwrap();
                assert_eq!(read_all(&mut file), expected);
                file.close().unwrap();
            }
        }
    }
}

#[test]
fn list_files_and_nested_directories() {
    for block_size in [128, 512, 1024] {
        for write_cycles in [0, 1, 10, 100] {
            let mut fs = make_fs(block_size, FS_SIZE_BYTES / block_size, write_cycles);
            let files = [
                "/test1.txt",
                "/test2.txt",
                "/dir1/test3.txt",
                "/dir1/test4.txt",
                "/dir2/dir3/test5.txt",
                "/dir2/dir3/test6.txt",
                "/dir4/dir5/dir6/test7.txt",
                "/test8.txt",
            ];
            for path in files {
                create_file(&mut fs, path).close().unwrap();
            }

            let listed = list_paths(&mut fs);
            assert_eq!(listed, files);
            for (prefix, expected) in [
                ("/dir1", vec!["/dir1/test3.txt", "/dir1/test4.txt"]),
                (
                    "/dir2",
                    vec!["/dir2/dir3/test5.txt", "/dir2/dir3/test6.txt"],
                ),
                ("/dir4/dir5/dir6", vec!["/dir4/dir5/dir6/test7.txt"]),
                ("/test8.txt", vec!["/test8.txt"]),
                ("mismatch", vec![]),
            ] {
                let matches: Vec<_> = listed
                    .iter()
                    .filter(|path| path.starts_with(prefix))
                    .map(String::as_str)
                    .collect();
                assert_eq!(matches, expected);
            }
        }
    }
}

#[test]
fn metadata_is_independent_and_persists_after_reopen() {
    for block_size in [128, 512, 1024] {
        for write_cycles in [0, 1, 10, 100] {
            for metadata_size in [1, 10, 100] {
                let mut fs = make_fs(block_size, FS_SIZE_BYTES / block_size, write_cycles);
                let metadata = data(metadata_size, 6);
                let file_data = b"Hello world!";
                let mut file = create_file(&mut fs, "/test.txt");
                assert_eq!(file.read_metadata(None).unwrap(), b"");
                assert_eq!(file.metadata_size().unwrap(), 0);
                file.write_metadata(&metadata).unwrap();
                assert_eq!(file.read_metadata(None).unwrap(), metadata);
                file.write(file_data).unwrap();
                assert_eq!(read_all(&mut file), file_data);
                assert_eq!(file.read_metadata(None).unwrap(), metadata);
                assert_eq!(file.metadata_size().unwrap(), metadata_size as u32);
                file.close().unwrap();

                let mut file = fs
                    .open("/test.txt", LogFsOpenFlags_LOGFS_OPEN_RD_ONLY as u32)
                    .unwrap();
                assert_eq!(read_all(&mut file), file_data);
                assert_eq!(file.read_metadata(None).unwrap(), metadata);
                assert_eq!(file.metadata_size().unwrap(), metadata_size as u32);
                file.close().unwrap();
            }
        }
    }
}

#[test]
fn list_file_sizes_and_metadata_sizes() {
    for block_size in [128, 512, 1024] {
        for write_cycles in [0, 1, 10, 100] {
            let mut fs = make_fs(block_size, FS_SIZE_BYTES / block_size, write_cycles);
            let files: [(&str, &[u8], &[u8]); 5] = [
                ("/test1.txt", b"Hello, world!", b""),
                ("/test2.txt", b"", b"Let's get this money"),
                ("/test3.txt", b"UBC For", b"mula Electric"),
                ("/test4.txt", b"", b""),
                ("/test5.txt", &[0xAB; 10_000], b""),
            ];

            for (path, file_data, metadata) in files {
                let mut file = create_file(&mut fs, path);
                file.write(file_data).unwrap();
                file.write_metadata(metadata).unwrap();
                file.close().unwrap();
            }

            let mut sizes = Vec::new();
            for path in list_paths(&mut fs) {
                let mut file = fs
                    .open(&path, LogFsOpenFlags_LOGFS_OPEN_RD_ONLY as u32)
                    .unwrap();
                sizes.push((path, file.size().unwrap(), file.metadata_size().unwrap()));
                file.close().unwrap();
            }
            assert_eq!(
                sizes,
                vec![
                    ("/test1.txt".to_string(), 13, 0),
                    ("/test2.txt".to_string(), 0, 20),
                    ("/test3.txt".to_string(), 7, 13),
                    ("/test4.txt".to_string(), 0, 0),
                    ("/test5.txt".to_string(), 10_000, 0),
                ]
            );
        }
    }
}

#[test]
fn mounting_corrupt_filesystem_fails_then_format_recovers() {
    for block_size in [128, 512, 1024] {
        for write_cycles in [0, 1, 10, 100] {
            let block_count = FS_SIZE_BYTES / block_size;
            let disk = Arc::new(LogFsRamDisk::new(block_size, block_count).unwrap());
            let mut fs = LogFs::new(block_size, block_count, disk.clone(), write_cycles, false);
            fs.format().unwrap();
            fs.mount().unwrap();

            let corrupt_block = vec![0xAB; block_size as usize];
            disk.write_block(0, &corrupt_block).unwrap();
            disk.write_block(1, &corrupt_block).unwrap();
            assert!(fs.mount().is_err());

            fs.format().unwrap();
            fs.mount().unwrap();
        }
    }
}

#[test]
fn out_of_memory_is_reported_on_data_write() {
    let mut fs = make_small_fs();
    let mut file = create_file(&mut fs, "/test.txt");
    let chunk = data(256, 7);
    file.write(&chunk).unwrap();
    assert_eq!(file.write(&chunk), Err(LogFsErr_LOGFS_ERR_NOMEM));
}

#[test]
fn out_of_memory_is_reported_when_creating_file() {
    let mut fs = make_small_fs();
    create_file(&mut fs, "/test.txt").close().unwrap();
    assert!(fs
        .open(
            "/test2.txt",
            (LogFsOpenFlags_LOGFS_OPEN_RD_WR | LogFsOpenFlags_LOGFS_OPEN_CREATE) as u32
        )
        .is_err());
}

#[test]
fn out_of_memory_is_reported_when_replacing_metadata() {
    let mut fs = make_small_fs();
    let mut file = create_file(&mut fs, "/test.txt");
    let metadata = data(256, 8);

    for _ in 0..(2 * SMALL_WRITE_CYCLES - 1) {
        file.write_metadata(&metadata).unwrap();
    }
    assert!(file.write_metadata(&metadata).is_err());
}
