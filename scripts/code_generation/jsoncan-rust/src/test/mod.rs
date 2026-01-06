use std::collections::HashMap;
use std::fmt::Debug;
use std::hash::Hash;

use crate::{
    can_database::{CanDatabase, error::CanDBError},
    parsing::JsonCanParser,
};

pub fn setup() -> Result<CanDatabase, CanDBError> {
    let _can_data_dir = std::path::Path::new(file!()).parent().unwrap();
    CanDatabase::from(JsonCanParser::new(format!(
        "{}/json_configs/valid_json1",
        _can_data_dir.display()
    )))
}

pub fn assert_setequal<S, T, I1, I2>(iter1: I1, iter2: I2)
where
    S: Eq + PartialEq + Hash + Debug,
    T: Eq + PartialEq + Hash + Debug,
    I1: Iterator<Item = S>,
    I2: Iterator<Item = T>,
    HashMap<S, i32>: PartialEq<HashMap<T, i32>>,
{
    let mut counts1 = HashMap::new();
    for item in iter1 {
        *counts1.entry(item).or_insert(0) += 1;
    }

    let mut counts2 = HashMap::new();
    for item in iter2 {
        *counts2.entry(item).or_insert(0) += 1;
    }

    assert!(counts1 == counts2, "{:?} {:?}", counts1, counts2)
}

#[cfg(test)]
mod node_tests;

#[cfg(test)]
mod bus_tests;

#[cfg(test)]
mod consistency_tests;
