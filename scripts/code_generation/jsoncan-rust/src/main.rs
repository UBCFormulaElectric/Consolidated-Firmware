mod can_database;
mod codegen;
mod dbcgen;
mod parsing;
mod reroute;
mod test;

use std::path::Path;

use crate::{can_database::CanDatabase, dbcgen::DbcGenerator, reroute::resolve_tx_rx_reroute};
use clap::Parser;
use codegen::cpp::CModule;
use parsing::{JsonCanParser, report_parse_err_exit};

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
    let can_db = CanDatabase::from(match JsonCanParser::new(args.can_data_dir) {
        Err(e) => report_parse_err_exit(e),
        Ok(p) => p,
    });

    let (tx_configs, rx_configs, reroute_config) = resolve_tx_rx_reroute(&can_db);

    // Generate DBC file
    write_text(
        DbcGenerator::new(&can_db, &rx_configs).source(),
        args.dbc_output,
    );
    if args.only_dbc {
        return;
    }

    if !can_db.nodes.contains_key(&args.board) {
        panic!("Board {} not found in CAN database.", args.board);
    }

    let modules: Vec<(CModule, String)> = vec![
        (
            CModule::AppCanUtilsModule {
                can_db: &can_db,
                tx_config: &tx_configs[&args.board],
                rx_config: &rx_configs[&args.board],
            },
            Path::new("app")
                .join("app_canUtils")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CModule::AppCanTxModule {
                can_db: &can_db,
                tx_config: &tx_configs[&args.board],
            },
            Path::new("app")
                .join("app_canTx")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CModule::AppCanAlertsModule {
                can_db: &can_db,
                board: &args.board,
            },
            Path::new("app")
                .join("app_canAlerts")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CModule::AppCanDataCaptureModule { can_db: &can_db },
            Path::new("app")
                .join("app_canDataCapture")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CModule::AppCanRxModule {
                can_db: &can_db,
                board: &args.board,
                rx_config: &rx_configs[&args.board],
            },
            Path::new("app")
                .join("app_canRx")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CModule::IoCanTxModule {
                can_db: &can_db,
                board: &args.board,
                tx_config: &tx_configs[&args.board],
            },
            Path::new("io")
                .join("io_canTx")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CModule::IoCanRxModule {
                can_db: &can_db,
                board: &args.board,
                rx_config: &rx_configs[&args.board],
            },
            Path::new("io")
                .join("io_canRx")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        ),
        (
            CModule::IoCanRerouteModule {
                can_db: &can_db,
                board: &args.board,
                reroute_config: &reroute_config
                    .get(&args.board)
                    .expect("Reroute config not found for board"),
            },
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
            module.header_template(),
            module_full_path
                .with_extension("h")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        );
        write_text(
            module.source_template(),
            module_full_path
                .with_extension("c")
                .to_str()
                .expect("Invalid path")
                .to_string(),
        );
    }
}

fn write_text(contents: String, path: String) {
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
