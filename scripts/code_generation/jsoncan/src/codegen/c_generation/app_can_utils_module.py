import jinja2 as j2

from .routing import CanTxConfig, CanRxConfig
from ...can_database import CanDatabase, CanMessage
from ...utils import *
from .utils import load_template
from .cmodule import CModule


def calculate_packing_iterations(signal):
    packed_bits = 0
    iterations = []
    bit_start = signal.start_bit
    starting_byte = signal.start_bit // BYTE_SIZE

    while packed_bits < signal.bits:
        bits_to_pack = min(
            BYTE_SIZE - (bit_start % BYTE_SIZE), signal.bits - packed_bits
        )
        shift = packed_bits - (bit_start % BYTE_SIZE)
        mask = (1 << bits_to_pack) - 1 << (bit_start % BYTE_SIZE)
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
        bit_start += bits_to_pack
        starting_byte += 1

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
        )


def signal_placement_comment(msg: CanMessage):
    chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    signals = ["_"] * (msg.bytes() * 8)
    signals.extend(["x"] * ((8 - msg.bytes()) * 8))
    for signal_cnt, signal in enumerate(msg.signals):
        for i in range(signal.start_bit, signal.start_bit + signal.bits):
            signals[i] = chars[signal_cnt % len(chars)]

    signals = list(reversed(signals))
    return f'|{"|".join("".join(signals[i * 8:(i + 1) * 8]) for i in range(0, 8))}|'
