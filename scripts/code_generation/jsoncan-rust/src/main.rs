use clap::Parser;
use parsing::ParseError;

mod can_database;
mod codegen;
mod parsing;

use parsing::JsonCanParser;

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

fn report_error_and_exit(e: ParseError) -> ! {
    match e {
        ParseError::DuplicateTxMsgName {
            tx_node_name,
            msg_name,
            msg2_name,
        } => {
            eprintln!(
                "Error: Duplicate transmitted message name '{}' found in nodes '{}' and '{}'. Message names must be unique across all transmitting nodes.",
                msg_name, tx_node_name, msg2_name
            );
        }
        ParseError::DuplicateTxMsgID {
            tx_node_name,
            msg_name,
            msg2_name,
        } => {
            eprintln!(
                "Error: Duplicate transmitted message ID found for message '{}' in nodes '{}' and '{}'. Message IDs must be unique across all transmitting nodes.",
                msg_name, tx_node_name, msg2_name
            );
        }
        ParseError::DuplicateTxSignalName {
            signal_name,
            msg_name,
            msg2_name,
        } => {
            eprintln!(
                "Error: Duplicate transmitted signal name '{}' found in messages '{}' and '{}'. Signal names must be unique across all transmitted messages.",
                signal_name, msg_name, msg2_name
            );
        }
        ParseError::TxFDUnsupported {
            fd_msg_name,
            non_fd_node_name,
        } => {
            eprintln!(
                "Error: Message '{}' is configured as CAN FD but node '{}' does not support CAN FD. All transmitting nodes for a message must support CAN FD if the message is CAN FD.",
                fd_msg_name, non_fd_node_name
            );
        }
    }
    std::process::exit(1);
}

fn main() {
    let args = Args::parse();

    // Parse JSON
    let can_database = match JsonCanParser::new(args.can_data_dir) {
        Err(e) => report_error_and_exit(e),
        Ok(p) => p,
    }
    .make_database();

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
