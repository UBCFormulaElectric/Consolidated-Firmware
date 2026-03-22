use crate::{
    can_database::{CanDatabase, error::CanDBError},
    parsing::JsonCanParser,
};

#[allow(dead_code)]
pub fn setup() -> Result<CanDatabase, CanDBError> {
    let _can_data_dir = std::path::Path::new(file!()).parent().unwrap();
    CanDatabase::from(JsonCanParser::new(format!(
        "{}/json_configs/valid_json1",
        _can_data_dir.display()
    )))
}

#[cfg(test)]
mod node_tests;

#[cfg(test)]
mod bus_tests;
