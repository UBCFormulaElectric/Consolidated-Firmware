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

pub fn red(str: impl AsRef<str>) -> String {
    format_color(str, RED)
}

pub fn yellow(str: impl AsRef<str>) -> String {
    format_color(str, YEL)
}

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
        if cfg!(feature = "verbose") {
            println!("{}", format!($($arg)*));
        }
    }};
}

// enable even more verbose print with `--features debug`
#[macro_export]
macro_rules! dprintln {
    ($($arg:tt)*) => {{
        if cfg!(feature = "debug") {
            println!("{}", format!($($arg)*));
        }
    }};
}

pub fn rfc3339_to_utc(str: &String) -> Option<DateTime<Utc>> {
    let rfc3339_re = Regex::new(r"^((?:(\d{4}-\d{2}-\d{2})T(\d{2}:\d{2}:\d{2}(?:\.\d+)?))(Z|[\+-]\d{2}:\d{2})?)$").unwrap();

    if !rfc3339_re.is_match(str) {
        return None;
    }

    match str.parse::<DateTime<FixedOffset>>() {
        Ok(local) => {
            return Some(local.with_timezone(&Utc));
        }
        Err(_) => {
            return None;
        }
    }
}

/**
 * Check if string time is in format YYYY-MM-DDTHH:MM:SS[+-]00:00
 * Convert to UTC and return in RFC3339 format if valid, otherwise return None
 * E.g. 2024-01-01T00:00:00-07:00 -> 2024-01-01T07:10:00Z
 */
pub fn rfc3339_to_utc_str(str: &String) -> Option<String> {
    match rfc3339_to_utc(str) {
        Some(utc) => Some(utc.to_rfc3339_opts(chrono::SecondsFormat::Secs, true)),
        None => None
    }
}