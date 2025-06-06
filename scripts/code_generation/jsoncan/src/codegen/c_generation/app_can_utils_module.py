import jinja2 as j2

from ...can_database import CanDatabase, CanMessage, CanSignal
from ...utils import *
from .cmodule import CModule
from .routing import CanRxConfig, CanTxConfig
from .utils import load_template


def calculate_packing_iterations(signal: CanSignal):
    packed_bits = 0
    iterations = []

    while packed_bits < signal.bits:
        bit_start = signal.start_bit + packed_bits
        starting_byte = bit_start // BYTE_SIZE
        bit_in_byte = bit_start % BYTE_SIZE
        bits_to_pack = min(BYTE_SIZE - bit_in_byte, signal.bits - packed_bits)

        if signal.big_endian:
            shift = signal.bits - packed_bits - bits_to_pack - bit_in_byte
        else:
            shift = packed_bits - bit_in_byte

        mask = (1 << bits_to_pack) - 1 << bit_in_byte
        mask_text = f"0x{mask:X}"
        comment_data = ["_"] * BYTE_SIZE
        for i in range(bit_start % BYTE_SIZE, (bit_start % BYTE_SIZE) + bits_to_pack):
            comment_data[i] = "#"

        iterations.append(
            {
                "starting_byte": starting_byte,
                "shift": shift,
                "mask_text": mask_text,
                "comment_data": "".join(reversed(comment_data)),
                "bits_to_pack": bits_to_pack,
            }
        )

        packed_bits += bits_to_pack

    return iterations


class AppCanUtilsModule(CModule):
    def __init__(self, db: CanDatabase, tx_config: CanTxConfig, rx_config: CanRxConfig):
        self._db = db
        self._messages = [
            db.msgs[msg_name] for msg_name in tx_config.get_all_msg_names()
        ] + [db.msgs[msg_name] for msg_name in rx_config.get_all_rx_msgs_names()]
        self._all_enums = db.enums.values()

    def source_template(self):
        j2_env = j2.Environment(
            loader=j2.BaseLoader(), extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(load_template("app_canUtils.c.j2"))
        return template.render(
            messages=self._messages,
            signal_placement_comment=signal_placement_comment,
            iterations=calculate_packing_iterations,
            max_uint_for_bits=max_uint_for_bits,
        )

    def header_template(self):
        j2_env = j2.Environment(loader=j2.BaseLoader())
        template = j2_env.from_string(load_template("app_canUtils.h.j2"))
        return template.render(
            all_messages=self._db.msgs.values(),
            messages=self._messages,
            enums=self._all_enums,
            nodes=self._db.nodes.keys(),
        )


def signal_placement_comment(msg: CanMessage):
    chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    signals = ["_"] * (msg.dlc() * 8)
    for signal_cnt, signal in enumerate(msg.signals):
        for i in range(signal.start_bit, signal.start_bit + signal.bits):
            signals[i] = chars[signal_cnt % len(chars)]

    signals = list(reversed(signals))

    placement_part = f"|{'|'.join(''.join(signals[i * 8 : (i + 1) * 8]) for i in range(0, msg.dlc()))}|"
    if msg.dlc() == 0:
        return "(0 data bytes)"
    elif msg.dlc() == 1:
        return placement_part + " (1 data byte)"
    else:
        return placement_part + f" ({msg.dlc()} data bytes)"
