import jinja2 as j2

from .c_writer import *
from .utils import load_template

PACK_SHIFT_LEFT_FUNC = "packShiftLeft"
PACK_SHIFT_RIGHT_FUNC = "packShiftRight"
UNPACK_SHIFT_LEFT_FUNC = "unpackShiftLeft"
UNPACK_SHIFT_RIGHT_FUNC = "unpackShiftRight"

CAN_ENCODE_MACRO = "CAN_ENCODE"
CAN_SIGNED_ENCODE_MACRO = "CAN_SIGNED_ENCODE"
CAN_DECODE_MACRO = "CAN_DECODE"
SIGNED_DECODE_MACRO = "SIGNED_DECODE"

PACK_TEMPLATE = "out_data[{byte_index}] |= {func}({value}, {shift}, {mask});   // Packs bits {bits_comment} of byte {byte_index}"
UNPACK_TEMPLATE = "{var_name} |= {func}({value}, {shift}, {mask});   // Unpacks bits {bits_comment} of msg byte {byte_index}"


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
    def __init__(self, db: CanDatabase, node: str):
        self._db = db
        self._node = node

    def source_template(self):
        template = load_template("app_canUtils.c.j2")
        j2_env = j2.Environment(
            loader=j2.BaseLoader, extensions=["jinja2.ext.loopcontrols"]
        )
        template = j2_env.from_string(template)
        return template.render(
            messages=self._db.msgs_for_node(self._node),
            signal_placement_comment=signal_placement_comment,
            iterations=calculate_packing_iterations,
            max_uint_for_bits=max_uint_for_bits,
        )

    def header_template(self):
        can_enums = list(self._db.shared_enums.values())
        for msg in self._db.msgs_for_node(self._node):
            for signal in msg.signals:
                if signal.enum and signal.enum not in can_enums:
                    can_enums.append(signal.enum)

        template = load_template("app_canUtils.h.j2")
        j2_env = j2.Environment(loader=j2.BaseLoader)
        template = j2_env.from_string(template)

        return template.render(
            all_messages=self._db.msgs.values(),
            messages=self._db.msgs_for_node(self._node),
            enums=can_enums,
        )


def signal_placement_comment(msg: CanMessage):
    chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    signals = ["_"] * (msg.bytes() * 8)
    signals.extend(["x"] * ((8 - msg.bytes()) * 8))
    for signal_cnt, signal in enumerate(msg.signals):
        for i in range(signal.start_bit, signal.start_bit + signal.bits):
            signals[i] = chars[signal_cnt % len(chars)]

    signals = list(reversed(signals))
    return f'|{"|".join("".join(signals[i*8:(i+1)*8]) for i in range(0, 8))}|'
