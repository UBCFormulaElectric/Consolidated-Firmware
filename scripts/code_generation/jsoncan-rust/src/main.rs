mod can_database;
mod codegen;
mod parsing;

use clap::Parser;
use parsing::{JsonCanParser, make_database_from_parser, report_parse_err_exit};

/// Simple program to greet a person
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
    let can_database = make_database_from_parser(match JsonCanParser::new(args.can_data_dir) {
        Err(e) => report_parse_err_exit(e),
        Ok(p) => p,
    });

    // tx_configs, rx_configs, reroute_config = resolve_tx_rx_reroute(can_db);

    // Generate DBC file
    // write_text(
    //     DbcGenerator(database=can_db, rx_configs=rx_configs).source(), dbc_output
    // )
    if args.only_dbc {
        return;
    }

    // if !can_db.nodes.contains(&args.board) {
    //     panic!("Board {} not found in CAN database.", args.board);
    // }

    // let modules: Vec<(CModule, &str)> = vec![
    //     (
    //         AppCanUtilsModule(can_db, tx_configs[board], rx_configs[board]),
    //         os.path.join("app", "app_canUtils"),
    //     ),
    //     (AppCanTxModule(can_db, tx_configs[board]), os.path.join("app", "app_canTx")),
    //     (AppCanAlertsModule(can_db, board), os.path.join("app", "app_canAlerts")),
    //     (AppCanDataCaptureModule(can_db), os.path.join("app", "app_canDataCapture")),
    //     (
    //         AppCanRxModule(can_db, board, rx_configs[board]),
    //         os.path.join("app", "app_canRx"),
    //     ),
    //     (
    //         IoCanTxModule(can_db, board, tx_configs[board]),
    //         os.path.join("io", "io_canTx"),
    //     ),
    //     (
    //         IoCanRxModule(can_db, board, rx_configs[board]),
    //         os.path.join("io", "io_canRx"),
    //     ),
    //     (
    //         IoCanRerouteModule(can_db, board, reroute_config.get(board)),
    //         os.path.join("io", "io_canReroute"),
    //     ),
    // ]

    // for module, module_path in modules:
    //     module_full_path = os.path.join(output_dir, module_path)
    //     write_text(module.header_template(), module_full_path + ".h")
    //     write_text(module.source_template(), module_full_path + ".c")
}
