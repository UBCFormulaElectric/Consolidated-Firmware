use jsoncan_rust::can_database::CanDatabase;
use jsoncan_rust::codegen::cpp::CPPModule;
use jsoncan_rust::codegen::cpp::{
    AppCanAlertsModule, AppCanDataCaptureModule, AppCanRxModule, AppCanTxModule, AppCanUtilsModule,
    CPPGenerator, IoCanRerouteModule, IoCanRxModule, IoCanTxModule,
};
use jsoncan_rust::dbcgen::DbcGenerator;
use jsoncan_rust::parsing::JsonCanParser;
use jsoncan_rust::reroute::resolve_tx_rx_reroute;

use clap::Parser;
use std::path::Path;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(short, long)]
    board: String,
    #[arg(long)]
    can_data_dir: String,
    #[arg(long)]
    output_dir: String,
    #[arg(long)]
    dbc_output: String,
    #[arg(long, default_value_t = false)]
    only_dbc: bool,
}

fn main() {
    let args = Args::parse();

    // Parse JSON
    let can_db = CanDatabase::from(JsonCanParser::new(args.can_data_dir)).unwrap();

    let (tx_configs, rx_configs, reroute_config) = resolve_tx_rx_reroute(&can_db);

    // Generate DBC file
    write_text(
        DbcGenerator::new(&can_db, &rx_configs).source().unwrap(),
        &args.dbc_output,
    );
    if args.only_dbc {
        return;
    }

    if can_db
        .nodes
        .iter()
        .find(|node| node.name == args.board)
        .is_none()
    {
        panic!("Board {} not found in CAN database.", args.board);
    }

    let modules: Vec<(CPPModule, String)> = vec![
        (
            CPPModule::AppCanUtilsModule(AppCanUtilsModule::new(&can_db)),
            Path::new("app")
                .join("app_canUtils")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CPPModule::AppCanTxModule(AppCanTxModule::new(&can_db, &args.board)),
            Path::new("app")
                .join("app_canTx")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CPPModule::AppCanAlertsModule(AppCanAlertsModule::new(&can_db, &args.board)),
            Path::new("app")
                .join("app_canAlerts")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CPPModule::AppCanDataCaptureModule(AppCanDataCaptureModule::new(&can_db, &args.board)),
            Path::new("app")
                .join("app_canDataCapture")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CPPModule::AppCanRxModule(AppCanRxModule::new(
                &can_db,
                &args.board,
                &rx_configs[&args.board],
            )),
            Path::new("app")
                .join("app_canRx")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CPPModule::IoCanTxModule(IoCanTxModule::new(
                &can_db,
                &args.board,
                &tx_configs[&args.board],
            )),
            Path::new("io")
                .join("io_canTx")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CPPModule::IoCanRxModule(IoCanRxModule::new(
                &can_db,
                &rx_configs[&args.board],
                &args.board,
            )),
            Path::new("io")
                .join("io_canRx")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CPPModule::IoCanRerouteModule(IoCanRerouteModule::new(
                &can_db,
                &args.board,
                &reroute_config[&args.board],
            )),
            Path::new("io")
                .join("io_canReroute")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
    ];

    for (module, module_path) in modules {
        let module_full_path = Path::new(&args.output_dir).join(module_path);
        write_text(
            module.header_template().unwrap(),
            module_full_path
                .with_extension("hpp")
                .to_str()
                .expect("Invalid path"),
        );
        write_text(
            module.source_template().unwrap(),
            module_full_path
                .with_extension("cpp")
                .to_str()
                .expect("Invalid path"),
        );
    }
}

fn write_text(contents: String, path: &str) {
    let output_folder = std::path::Path::new(&path)
        .parent()
        .expect("Failed to get parent directory");
    if !output_folder.exists() {
        std::fs::create_dir_all(output_folder).expect("Failed to create output directory");
    }
    match std::fs::write(path, contents) {
        Ok(_) => (),
        Err(e) => panic!("Failed to write file: {}", e),
    }
}
