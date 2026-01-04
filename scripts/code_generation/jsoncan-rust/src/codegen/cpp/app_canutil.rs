use askama::Template;

use crate::{
    can_database::{CanDatabase, CanEnum, CanMessage, CanSignal},
    codegen::cpp::CPPGenerator,
    reroute::{CanRxConfig, CanTxConfig},
};

struct Iteration {
    starting_byte: u16,
    shift: u16,
    mask_text: String,
    comment_data: String,
    bits_to_pack: u16,
}

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canUtils.c.j2")]
struct AppCanUtilsModuleSource<'a> {
    messages: &'a Vec<CanMessage>,
}
impl AppCanUtilsModuleSource<'_> {
    fn signal_placement_comment(self: &Self, msg: &CanMessage) -> std::string::String {
        let chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        let mut signals = vec!['_'; (msg.dlc() * 8) as usize];
        for (signal_cnt, signal) in msg.signals.iter().enumerate() {
            for i in (signal.start_bit as usize)..(signal.start_bit + signal.bits) as usize {
                signals[i] = chars.chars().nth(signal_cnt % chars.len()).unwrap();
            }
        }
        signals.reverse();

        let placement_part = format!(
            "|{}|",
            (0..msg.dlc())
                .map(|i| {
                    signals[i as usize * 8..(i as usize + 1) * 8]
                        .iter()
                        .collect::<String>()
                })
                .collect::<Vec<String>>()
                .join("|")
        );
        if msg.dlc() == 0 {
            return "(0 data bytes)".to_string();
        } else if msg.dlc() == 1 {
            return format!("{} (1 data byte)", placement_part);
        } else {
            return format!("{} ({} data bytes)", placement_part, msg.dlc());
        }
    }

    fn iterations(self: &Self, signal: &CanSignal) -> Vec<Iteration> {
        static BYTE_SIZE: u16 = 8;
        let mut packed_bits = 0;
        let mut iterations: Vec<Iteration> = Vec::new();

        while packed_bits < signal.bits {
            let bit_start = signal.start_bit + packed_bits;
            let starting_byte = bit_start / BYTE_SIZE;
            let bit_in_byte = bit_start % BYTE_SIZE;
            let bits_to_pack = std::cmp::min(BYTE_SIZE - bit_in_byte, signal.bits - packed_bits);

            let shift = if signal.big_endian {
                signal.bits - packed_bits - bits_to_pack - bit_in_byte
            } else {
                packed_bits - bit_in_byte
            };

            let mask = (1 << bits_to_pack) - 1 << bit_in_byte;
            let mask_text = format!("0x{:X}", mask);
            let mut comment_data = vec!["_"; BYTE_SIZE as usize];
            for i in (bit_start % BYTE_SIZE)..((bit_start % BYTE_SIZE) + bits_to_pack) {
                comment_data[i as usize] = "#";
            }

            iterations.push(Iteration {
                starting_byte,
                shift,
                mask_text,
                comment_data: comment_data.into_iter().rev().collect::<String>(),
                bits_to_pack,
            });

            packed_bits += bits_to_pack;
        }

        return iterations;
    }

    fn max_uint_for_bits(self: &Self, bits: &u16) -> u32 {
        (1u32 << bits) - 1
    }
}

mod filters {}

#[derive(Template)]
#[template(path = "../src/codegen/cpp/template/app_canUtils.h.j2")]
struct AppCanUtilsModuleHeader<'a> {
    messages: &'a Vec<CanMessage>,
    node_names: &'a Vec<String>,
    enums: &'a Vec<CanEnum>,
}

pub struct AppCanUtilsModule {
    messages: Vec<CanMessage>,
    node_names: Vec<String>,
    enums: Vec<CanEnum>,
}

impl AppCanUtilsModule {
    pub fn new(can_db: &CanDatabase) -> AppCanUtilsModule {
        AppCanUtilsModule {
            messages: can_db.get_all_msgs().expect("surely"),
            node_names: can_db.nodes.iter().map(|n| n.name.clone()).collect(),
            enums: can_db
                .nodes
                .iter()
                .flat_map(|n| n.enums.clone())
                .chain(can_db.shared_enums.clone())
                .collect(),
        }
    }
}

impl CPPGenerator for AppCanUtilsModule {
    fn header_template(&self) -> Result<String, askama::Error> {
        AppCanUtilsModuleHeader {
            messages: &self.messages,
            node_names: &self.node_names,
            enums: &self.enums,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        AppCanUtilsModuleSource {
            messages: &self.messages,
        }
        .render()
    }
}
