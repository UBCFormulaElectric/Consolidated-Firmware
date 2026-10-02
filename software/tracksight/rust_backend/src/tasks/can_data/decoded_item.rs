use jsoncan_rust::can_database::DecodedSignal;

#[derive(Debug, Clone)]
pub struct DecodedMarker {
    pub name: String,
    pub timestamp: u64,
}

#[derive(Debug, Clone)]
pub enum DecodedItem {
    Signal(DecodedSignal),
    Marker(DecodedMarker),
}
