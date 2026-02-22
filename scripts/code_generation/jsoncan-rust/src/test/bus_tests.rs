use crate::test::{assert_setequal, setup};

#[test]
fn test_busses_present() {
    let cdb = setup().unwrap();
    assert_setequal(
        cdb.buses.iter().map(|bus| bus.name.clone()),
        ["can1".to_string(), "can2".to_string(), "can3".to_string()].into_iter(),
    );
}

#[test]
fn test_bus1_properties() {
    let cdb = setup().unwrap();
    let can1 = cdb.buses.iter().find(|b| b.name == "can1").unwrap();
    assert_eq!(can1.name, "can1");
    assert_eq!(can1.bus_speed, 4000);
    assert_eq!(can1.fd, true);
    assert_eq!(can1.default_mode, "default");
    assert_setequal(
        can1.modes.iter(),
        ["default".to_string(), "debug".to_string()].iter(),
    );
}

#[test]
fn test_bus2_properties() {
    let cdb = setup().unwrap();
    let can2 = cdb.buses.iter().find(|b| b.name == "can2").unwrap();
    assert_eq!(can2.name, "can2");
    assert_eq!(can2.bus_speed, 1000);
    assert_eq!(can2.fd, false);
    assert_eq!(can2.default_mode, "default");
    assert_setequal(
        can2.modes.iter(),
        ["default".to_string(), "debug".to_string()].iter(),
    );
}

#[test]
fn test_bus3_properties() {
    let cdb = setup().unwrap();
    let can3 = cdb.buses.iter().find(|b| b.name == "can3").unwrap();
    assert_eq!(can3.name, "can3");
    assert_eq!(can3.bus_speed, 500);
    assert_eq!(can3.fd, false);
    assert_eq!(can3.default_mode, "default");
    assert_setequal(
        can3.modes.iter(),
        ["default".to_string(), "debug".to_string()].iter(),
    );
}
