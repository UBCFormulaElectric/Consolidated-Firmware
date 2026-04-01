// I did not like the colorized library, so use own utils hehe

/*
    COLOR PRINT
 */

const RED: &str = "31m";
const GRE: &str = "32m";
const YEL: &str = "33m";
const NON: &str = "0m";

#[allow(dead_code)]
pub fn red(str: impl AsRef<str>) -> String {
    format_color(str, RED)
}

#[allow(dead_code)]
pub fn yellow(str: impl AsRef<str>) -> String {
    format_color(str, YEL)
}

#[allow(dead_code)]
pub fn green(str: impl AsRef<str>) -> String {
    format_color(str, GRE)
}

fn format_color(str: impl AsRef<str>, color: &str) -> String {
    format!("\x1b[{color}{}\x1b[{NON}", str.as_ref())
}

/*
    VERBOSE/DEBUG MACROS
*/

// First level: verbose
// Second level: verbose AND debug

// macro for printing red hehe
#[macro_export]
macro_rules! error_println {
    ($($arg:tt)*) => {{
        let s = format!($($arg)*);
        eprintln!("{}", crate::red(&s));
    }};
}

// enable verbose print with `--features verbose`
#[macro_export]
macro_rules! vprintln {
    ($($arg:tt)*) => {{
        #[cfg(feature = "verbose")] {
            println!("{}", format!($($arg)*));
        }
    }};
}

// enable even more verbose print with `--features debug`
#[macro_export]
macro_rules! dprintln {
    ($($arg:tt)*) => {{
        #[cfg(feature = "debug")] {
            println!("{}", format!($($arg)*));
        }
    }};
}
