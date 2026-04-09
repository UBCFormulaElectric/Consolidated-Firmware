use bindgen::Builder;
use std::path::PathBuf;

fn main() {
    let logfs_root = PathBuf::from(env!("CARGO_MANIFEST_DIR")).parent().unwrap().to_path_buf();
    let src_dir = logfs_root.join("src");

    println!("cargo:rerun-if-changed={}", src_dir.display());

    let bindings = Builder::default()
        .header(src_dir.join("logfs.h").to_str().unwrap())
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("src/bindings.rs");
    bindings
        .write_to_file(&out_path)
        .expect("Couldn't write bindings!");
}
