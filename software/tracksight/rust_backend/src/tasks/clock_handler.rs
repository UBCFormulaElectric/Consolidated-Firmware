pub enum TimeMessage {
    NTPTimeMessage,
    NTPDateMessage,
    BaseTimeRegMessage {
        base_time: u32, // TODO some time format
    }
}