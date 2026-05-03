use askama::Template;

use crate::can_database::CanBus;
use crate::{
    can_database::{CanDatabase, CanEnum, CanMessage, CanSignal},
    codegen::cpp::CPPGenerator,
};

mod filters {}

struct Iteration {
    starting_byte: u16,
    shift: i16,
    mask_text: String,
    comment_data: String,
}

#[derive(Template)]
#[template(path = "app_canUtils.cpp.j2")]
struct AppCanUtilsModuleSource<'a> {
    messages: &'a Vec<CanMessage>,
}
impl AppCanUtilsModuleSource<'_> {
    fn signal_placement_comment(self: &Self, msg: &CanMessage) -> String {
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
            "(0 data bytes)".to_string()
        } else if msg.dlc() == 1 {
            format!("{} (1 data byte)", placement_part)
        } else {
            format!("{} ({} data bytes)", placement_part, msg.dlc())
        }
    }

    fn iterations(self: &Self, signal: &CanSignal) -> Vec<Iteration> {
        static BYTE_SIZE: u16 = 8;
        let mut packed_bits = 0;
        let mut out: Vec<Iteration> = Vec::new();

        // while there are still bits left to pack for the given signal
        while packed_bits < signal.bits {
            let start_bit = signal.start_bit + packed_bits;
            let start_bit_mod_byte = start_bit % BYTE_SIZE;
            let bits_to_pack_in_this_byte = std::cmp::min(
                BYTE_SIZE - start_bit_mod_byte, // number of bits you can fit into this byte
                signal.bits - packed_bits,      // number of bits you have left to pack
            );

            // building the comment
            let mut comment_data: Vec<&str> = vec!["_"; BYTE_SIZE as usize];
            for i in start_bit_mod_byte..(start_bit_mod_byte + bits_to_pack_in_this_byte) {
                comment_data[i as usize] = "#";
            }

            // push iteration
            out.push(Iteration {
                starting_byte: start_bit / BYTE_SIZE,
                shift: if signal.big_endian {
                    signal.bits as i16
                        - packed_bits as i16
                        - bits_to_pack_in_this_byte as i16
                        - start_bit_mod_byte as i16
                } else {
                    // little endian case
                    packed_bits as i16 - start_bit_mod_byte as i16
                },
                mask_text: format!(
                    "0x{:X}",
                    (1u16 << (bits_to_pack_in_this_byte + start_bit_mod_byte))
                        - (1u16 << start_bit_mod_byte) // this should be a u8
                ),
                comment_data: comment_data.into_iter().rev().collect::<String>(),
            });

            packed_bits += bits_to_pack_in_this_byte;
        }

        out
    }

    fn max_uint_for_bits(self: &Self, bits: &u16) -> u32 {
        (1u32 << bits) - 1
    }
}

#[derive(Template)]
#[template(path = "app_canUtils.hpp.j2")]
struct AppCanUtilsModuleHeader<'a> {
    messages: &'a Vec<CanMessage>,
    node_names: &'a Vec<String>,
    enums: &'a Vec<CanEnum>,
    fd: bool,
    node_buses: &'a Vec<&'a CanBus>,
}

pub struct AppCanUtilsModule<'a> {
    messages: Vec<CanMessage>,
    node_names: Vec<String>,
    enums: Vec<CanEnum>,
    fd: bool, // if we need fd features at any point
    // TODO make a JsonCanFdMsg to handle the fd issue on a message by message basis
    // TODO this would also force with the type system that certain busses must transmit fd messages?
    node_buses: Vec<&'a CanBus>,
}

impl AppCanUtilsModule<'_> {
    pub fn new<'a>(can_db: &'a CanDatabase, board: &String) -> AppCanUtilsModule<'a> {
        let node_buses: Vec<&'a CanBus> = can_db
            .buses
            .iter()
            .filter(|b| b.node_names.contains(board))
            .collect();
        AppCanUtilsModule {
            messages: can_db.get_all_msgs().expect("get all msgs should not fail"),
            node_names: can_db.nodes.iter().map(|n| n.name.clone()).collect(),
            enums: can_db
                .nodes
                .iter()
                .flat_map(|n| n.enums.clone())
                .chain(can_db.shared_enums.clone())
                .collect(),
            fd: node_buses.iter().any(|b| b.fd),
            node_buses,
        }
    }
}

impl CPPGenerator for AppCanUtilsModule<'_> {
    fn header_template(&self) -> Result<String, askama::Error> {
        AppCanUtilsModuleHeader {
            messages: &self.messages,
            node_names: &self.node_names,
            enums: &self.enums,
            fd: self.fd,
            node_buses: &self.node_buses,
        }
        .render()
    }
    fn source_template(&self) -> Result<String, askama::Error> {
        AppCanUtilsModuleSource {
            messages: &self.messages,
        }
        .render()
    }
    fn file_stem(&self) -> String {
        "app_canUtils".to_string()
    }
}
