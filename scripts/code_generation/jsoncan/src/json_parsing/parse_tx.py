from math import ceil
import typing
from typing import Dict, Tuple

from schema import Schema, And, Optional, Or, SchemaError

from .parse_error import InvalidCanJson
from .parse_utils import load_json_file, get_optional_value
from ..can_database import CanMessage, CanSignal
from ..can_signals import CanEnum
from ..utils import max_uint_for_bits


def _calc_signal_scale_and_offset(
    max_val: int, min_val: int, num_bits: int
) -> Tuple[float, float]:
    """
    Calculate scale and offset for DBC file.
    """
    scale = (max_val - min_val) / max_uint_for_bits(num_bits)
    offset = min_val
    return scale, offset


_tx_signal_schema = Schema(
    # 4 options to define a signal"s representation...
    Or(
        Schema(
            {
                # Just bits, and signal will be a uint of X bits, i.e. offset=0, min=0, max=(2^bits-1)
                "bits": int,
                Optional("unit"): str,
                Optional("start_value"): int,
                Optional("start_bit"): int,
                Optional("signed"): bool,
                Optional("scale"): Or(int, float),
            }
        ),
        Schema(
            {
                # Bits/min/max, and signal will range from min to max in X bits, scale/offset will be calculated accordingly
                "bits": int,
                "min": Or(int, float),
                "max": Or(int, float),
                Optional("unit"): str,
                Optional("start_value"): Or(int, float),
                Optional("start_bit"): int,
                Optional("signed"): bool,
            }
        ),
        Schema(
            {
                # Resolution/min/max, and signal will range from min to max such that scale=resolution, bits/offset will be calculated accordingly
                "resolution": Or(int, float),
                "min": Or(int, float),
                "max": Or(int, float),
                Optional("unit"): str,
                Optional("start_value"): Or(int, float),
                Optional("start_bit"): int,
                Optional("signed"): bool,
            }
        ),
        Schema(
            {
                # Enum, signal will be generated with minimum # of bits to hold all possible enum values
                "enum": str,
                Optional("start_value"): Or(int, float),
                Optional("start_bit"): int,
            }
        ),
        Schema(
            {
                # Scale/offset/bits/signedness: Basically if you want to configure like a DBC file
                "scale": Or(int, float),
                "offset": Or(int, float),
                "bits": int,
                "min": Or(int, float),
                "max": Or(int, float),
                Optional("big_endian"): bool,
                Optional("start_value"): Or(int, float),
                Optional("start_bit"): int,
                Optional("signed"): bool,
                Optional("unit"): str,
            }
        ),
    )
)

_tx_msg_schema = Schema(
    {
        "msg_id": And(int, lambda x: 0 <= x <= 2**30 - 1),
        "signals": Or(
            Schema({
                str: _tx_signal_schema,
            }),
            Schema({}),
        ),
        "cycle_time": Or(int, Schema(None), lambda x: x >= 0),
        Optional("disabled"): bool,
        Optional("description"): str,
        Optional("allowed_modes"): Schema([str]),
        Optional("data_capture"): {
            Optional("log_cycle_time"): Or(int, Schema(None)),
            Optional("telem_cycle_time"): Or(int, Schema(None)),
        },
    }
)


def _validate_tx_json(json: Dict) -> Dict[str, dict]:
    return Or(Schema({str: _tx_msg_schema}), Schema({})).validate(json)


def _get_parsed_can_signal(
    signal_name: str,
    signal_json_data: Dict,
    next_available_bit: int,
    enums: dict[str, CanEnum],
) -> tuple[CanSignal, bool]:
    """
    Parse JSON data dictionary representing a CAN signal.
    """
    # max_val = 0
    # min_val = 0
    # scale = 0
    # offset = 0
    # bits = 0
    enum = None

    # Parse unit and starting bit position
    unit = signal_json_data.get("unit", "")
    start_bit, specified_start_bit = get_optional_value(
        signal_json_data, "start_bit", next_available_bit
    )
    signed = signal_json_data.get("signed", False)
    assert type(signed) is bool, "Signed must be a boolean value"
    big_endian = signal_json_data.get("big_endian", False)

    # Get signal value data. Method depends on which data provided in JSON file.
    # Option 1: Provide DBC data
    if all(
        datum in signal_json_data for datum in ("min", "max", "scale", "offset", "bits")
    ):
        bits = signal_json_data["bits"]
        max_val = signal_json_data["max"]
        min_val = signal_json_data["min"]
        scale = signal_json_data["scale"]
        offset = signal_json_data["offset"]

    # Option 2: Provide min, max, and bit length. Scale and offset are calculated.
    elif all(datum in signal_json_data for datum in ("min", "max", "bits")):
        max_val = signal_json_data["max"]
        min_val = signal_json_data["min"]
        bits = signal_json_data["bits"]

        scale, offset = _calc_signal_scale_and_offset(
            max_val=max_val, min_val=min_val, num_bits=bits
        )

    # Option 3: Provide min, max, and resolution (scale). Offset and bit length are calculated.
    elif all(datum in signal_json_data for datum in ("min", "max", "resolution")):
        max_val = signal_json_data["max"]
        min_val = signal_json_data["min"]
        sig_resolution = signal_json_data["resolution"]
        scale = sig_resolution
        offset = min_val

        max_raw_val = ceil((max_val - min_val) / sig_resolution)
        bits = max_raw_val.bit_length()

    # Option 4: Provide an enum. Min, max, bits, have to be calculated.
    # Scale and offset are assumed to be 1 and 0, respectively (so start your enums at 0!)
    elif "enum" in signal_json_data:
        enum_name = signal_json_data["enum"]
        if enum_name not in enums:
            raise InvalidCanJson(
                f"Signal '{signal_name}' requests an enum named '{enum_name}', but an enum by that name was not defined."
            )

        enum = enums[enum_name]
        max_val = enum.max_val()
        min_val = enum.min_val()
        bits = enum.num_bits()
        scale = 1
        offset = 0

    # Option 5: Just provide bits, and will be considered to be an unsigned int of however many bits.
    elif "bits" in signal_json_data:
        bits = signal_json_data["bits"]
        if signed:
            max_val = max_uint_for_bits(bits - 1) - 1
            min_val = -max_uint_for_bits(bits - 1)
        else:
            max_val = max_uint_for_bits(bits)
            min_val = 0
        scale = signal_json_data.get("scale", 1)
        offset = 0

    # Otherwise, payload data was not inputted correctly
    else:
        raise InvalidCanJson(
            f"Signal '{signal_name}' has invalid payload representation, and could not be parsed."
        )

    # Parse start value
    start_val = signal_json_data.get("start_value", min_val)

    # Signals can"t be longer than 32 bits, to maintain atomic read/write
    if bits < 1 or bits > 32:
        raise InvalidCanJson(
            f"Signal '{signal_name}' has invalid bit length {bits}. Bit length must be between 1 and 32."
        )

    return (
        CanSignal(
            name=signal_name,
            bits=bits,
            scale=scale,
            offset=offset,
            min_val=min_val,
            max_val=max_val,
            start_bit=start_bit,
            unit=unit,
            enum=enum,
            start_val=start_val,
            signed=signed,
            big_endian=big_endian,
        ),
        specified_start_bit,
    )


def _get_parsed_can_message(
    msg_name: str,
    msg_json_data: Dict,
    node_name: str,
    enums: dict[str, CanEnum],
) -> CanMessage:
    """
    Parse JSON data dictionary representing a CAN message.
    """
    msg_id = msg_json_data["msg_id"]
    description = msg_json_data.get("description", "")
    msg_cycle_time = msg_json_data["cycle_time"]
    max_len_bits = 64 * 8

    # will use mode from bus if none
    msg_modes = msg_json_data.get("allowed_modes", None)

    log_cycle_time = msg_cycle_time
    telem_cycle_time = msg_cycle_time
    if "data_capture" in msg_json_data:
        log_cycle_time = msg_json_data["data_capture"].get(
            "log_cycle_time", msg_cycle_time
        )
        telem_cycle_time = msg_json_data["data_capture"].get(
            "telem_cycle_time", msg_cycle_time
        )

    signals = []
    next_available_bit = 0
    occupied_bits: list[typing.Optional[str]] = [None] * max_len_bits
    require_start_bit_specified = False

    # Parse message signals
    for signal_name, signal_data in msg_json_data["signals"].items():
        signal_node_name = f"{node_name}_{signal_name}"
        signal, specified_start_bit = _get_parsed_can_signal(
            signal_name=signal_node_name,
            signal_json_data=signal_data,
            next_available_bit=next_available_bit,
            enums=enums,
        )

        # If we specify one start bit, we require that the rest of the message specify start bit too
        if specified_start_bit:
            require_start_bit_specified = True
        elif require_start_bit_specified:
            raise InvalidCanJson(
                f"Signal '{signal.name}' in '{msg_name}' must specify a start bit positions, because other signals in this message have specified start bits."
            )

        # Mark a signal's bits as occupied, by inserting the signal's name
        for idx in range(signal.start_bit, signal.start_bit + signal.bits):
            if idx < 0 or idx >= max_len_bits:
                raise InvalidCanJson(
                    f"Signal '{signal.name}' in '{msg_name}' is requesting to put a bit at invalid position {idx}. Messages have a maximum length of 64 bytes."
                )
            elif occupied_bits[idx] is not None:
                raise InvalidCanJson(
                    f"Signal '{signal.name}' in '{msg_name}' is requesting to put a bit at invalid position {idx}. That position is already occupied by the signal '{occupied_bits[idx]}'."
                )

            occupied_bits[idx] = signal.name

        signals.append(signal)
        next_available_bit += signal.bits

    return CanMessage(
        name=msg_name,
        id=msg_id,
        description=description,
        signals=signals,
        cycle_time=msg_cycle_time,
        tx_node_name=node_name,
        modes=msg_modes,
        log_cycle_time=log_cycle_time,
        telem_cycle_time=telem_cycle_time,
    )


def parse_tx_data(
    can_data_dir: str,
    tx_node_name: str,
    enums_map: dict[str, CanEnum],
) -> list[CanMessage]:
    """
    Parses TX messages from file, adds them to message list
    :param can_data_dir: :|
    :param tx_node_name: name of the node
    :param enums_map: just used to access the enums we have
    :return: list of names of messages associated with the given node
    """
    try:
        node_tx_json_data = _validate_tx_json(
            load_json_file(f"{can_data_dir}/{tx_node_name}/{tx_node_name}_tx")
        )
    except SchemaError:
        raise InvalidCanJson(f"TX json file is not valid for {tx_node_name}")

    msgs: list[CanMessage] = []
    for tx_msg_name_json, tx_msg_json in node_tx_json_data.items():
        # Skip if message is disabled
        if bool(tx_msg_json.get("disabled", False)):
            continue

        tx_msg_name = f"{tx_node_name}_{tx_msg_name_json}"
        msgs.append(
            _get_parsed_can_message(
                msg_name=tx_msg_name,
                msg_json_data=tx_msg_json,
                node_name=tx_node_name,
                enums=enums_map,
            )
        )
    return msgs
