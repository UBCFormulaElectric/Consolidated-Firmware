// I did not like the colorized library, so use own utils hehe

/*
    COLOR PRINT
 */

use chrono::{DateTime, FixedOffset, Utc};
use regex::Regex;

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

/**
 * Check if string time is in format YYYY-MM-DDTHH:MM:SS[+-]00:00
 * Convert to UTC and return in RFC3339 format if valid, otherwise return None
 * E.g. 2024-01-01T00:00:00-07:00 -> 2024-01-01T07:10:00Z
 */
pub fn rfc3339_to_utc(str: &String) -> Option<String> {
    let offset_re = Regex::new(r"^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}[+-]\d{2}:\d{2}$").unwrap();
    let utc_re = Regex::new(r"^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z$").unwrap();

    if utc_re.is_match(str) {
        return Some(str.clone());
    }

    if offset_re.is_match(str) {
        match str.parse::<DateTime<FixedOffset>>() {
            Ok(local) => {
                let utc = local.with_timezone(&Utc);
                return Some(utc.to_rfc3339());
            }
            Err(_) => {
                return None;
            }
        }
    }

    return None;
}