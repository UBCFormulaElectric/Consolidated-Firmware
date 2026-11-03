use socketcan::{
    CanFrame,
    Frame,
    CanSocket,
    Socket
};

mod logger;

use logger::{
    encode,
    Logger
};

const HELP: &str = "\
usage:
  libd [OPTIONS] --interface PATH --outdir PATH
options:
  -h, --help            prints a help message
args:
  -i, --interface PATH  path to interface receiving CAN frames
  -o, --outdir PATH     path to directory where logs will be written
";

const SUCCESS: i32 = 0;
const FAILURE: i32 = 1;

struct Args {
    interface: String,
    outdir: std::path::PathBuf,
}

fn parse_args() -> Result<Args, pico_args::Error> {
    let mut pargs = pico_args::Arguments::from_env();

    // -h, --help has higher priority and is
    // handled separately before anything else
    if pargs.contains(["-h", "--help"]) {
        print!("{}", HELP);
        std::process::exit(SUCCESS);
    }

    let args = Args {
        interface: pargs.value_from_str(["-i", "--interface"])?,
        outdir: pargs.value_from_str(["-o", "--outdir"])?,
    };

    // fail when we get an unexpected number of
    // args, really this shouldn't ever happen 
    // in prod unless the service file is wrong
    let remaining = pargs.finish();
    if !remaining.is_empty() {
        eprintln!("error: unused arguments: {:?}", remaining);
        std::process::exit(FAILURE);
    }

    Ok(args)
}

fn run(args: &Args) -> std::io::Result<()> {
    let sock = CanSocket::open(&args.interface)?;
    Ok(())
}

fn main() {
    let args = parse_args()
        .unwrap_or_else(|e| {
            eprintln!("error: {}", e);
            std::process::exit(FAILURE);
        });

    // todo: check if interface is up?
    // todo: and if the outdir exists

    println!("{}", args.interface);
    println!("{}", args.outdir.display());

    run(&args).unwrap_or_else(|e| {
        eprintln!("error: {}", e);
        std::process::exit(FAILURE);
    });
}
