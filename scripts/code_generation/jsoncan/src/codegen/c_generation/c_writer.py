"""
Provides an API for writing C code, used to generate all C CAN modules.
"""

from abc import ABC
from typing import Optional

from .c_config import *
from ...can_database import *

INDENT_4_SPACES = "    "
PREAMBLE = """\
/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off"""


class CModule(ABC):
    """
    ABC for a C module (i.e. pair of header .h and source .c files)
    """

    def header(self) -> str: ...

    def source(self) -> str: ...


@dataclass
class CVar:
    name: str
    type: Optional[str] = ""
    value: Optional[Union[int, float, str]] = 0
    comment: Optional[str] = ""


class CEnum:
    def __init__(self, name: str):
        self.name = name
        self.values = []

    def add_value(self, value: CVar):
        self.values.append(value)


class CStruct:
    def __init__(self, name: str, comment: Optional[str] = ""):
        self.name = name
        self.members = []
        self.comment = comment

    def add_member(self, member: CVar):
        self.members.append(member)


class CFunc:
    def __init__(
        self,
        name: str,
        return_type: str,
        args: List[CVar],
        qualifier: Optional[str] = "",
        comment: Optional[str] = "",
    ):
        self.name = name
        self.return_type = return_type
        self.arguments = args
        self.qualifier = qualifier
        self.comment = comment
        self.body = CWriter()


@dataclass
class CLiteral:
    value: Union[int, float]

    def __str__(self) -> str:
        if isinstance(self.value, float):
            return f"{self.value}f"
        else:
            return str(self.value)


class CWriter:
    """
    Class for generating C code. Define it as an object and call the public functions to add C constructs, indentation, etc.
    """

    def __init__(self):
        self._indent_level = 0
        self.indent_text = INDENT_4_SPACES
        self._text = ""
        self.indented_in_switch = False

    def add_line(self, text: Optional[str] = "", prefix: Optional[str] = "") -> None:
        self._text += f"{self.indent_text * self._indent_level}{prefix}{text}\n"

    def add_lines(self, text: str = "", prefix: Optional[str] = "") -> None:
        for line in text.splitlines():
            self.add_line(line, prefix)

    def add_comment(self, text: str = "") -> None:
        self.add_line(f"// {text}")

    def add_code(self, cw: "CWriter") -> None:
        self.add_lines(str(cw))

    def indent(self) -> None:
        self._indent_level += 1

    def unindent(self) -> None:
        self._indent_level -= 1

    def start_comment(self) -> None:
        self.add_line("/**")

    def end_comment(self) -> None:
        self.add_line(" */")

    def __str__(self) -> str:
        return self._text

    def add_preamble(self) -> None:
        self.add_lines(PREAMBLE)

    def add_header_comment(self, text: str) -> None:
        COMMENT_LENGTH = 70
        BAR_CHAR = "-"
        left_chars = (COMMENT_LENGTH - len(text)) // 2
        right_chars = (COMMENT_LENGTH - len(text)) - left_chars
        self.add_line(f"/* {BAR_CHAR * left_chars} {text} {BAR_CHAR * right_chars} */")

    def add_include(self, name: str) -> None:
        self.add_line(f"#include {name}")

    def add_macro(
        self,
        name: str,
        value: str,
        args: Optional[list[str]] = None,
        comment: Optional[str] = "",
    ) -> None:
        if args:
            if comment != "":
                self.start_comment()
                self.add_lines(comment, prefix=" * ")
                self.end_comment()
            self.add_line(f"#define {name}({', '.join(args)}) ({value})")
        else:
            if comment != "":
                self.add_line(f"#define {name} ({value}) // {comment}")
            else:
                self.add_line(f"#define {name} ({value})")

    def add_var_declaration(self, var: CVar, qualifier: Optional[str] = ""):
        if qualifier != "":
            self.add_line(f"{qualifier} {var.type} {var.name};")
        else:
            self.add_line(f"{var.type} {var.name};")

    def add_enum(self, enum: CEnum) -> None:
        self.add_line("typedef enum")
        self.add_line("{")
        self.indent()
        for enum_val in enum.values:
            self.add_line(f"{enum_val.name} = {enum_val.value},")
        self.unindent()
        self.add_line(f"}} {enum.name};")

    def add_struct(self, struct: CStruct) -> None:
        if struct.comment != "":
            self.start_comment()
            self.add_lines(struct.comment, prefix=" * ")
            self.end_comment()

        self.add_line("typedef struct")
        self.add_line("{")
        self.indent()

        for member in struct.members:
            if member.comment != "":
                self.start_comment()
                self.add_lines(member.comment, prefix=" * ")
                self.end_comment()
            self.add_line(f"{member.type} {member.name};")

        self.unindent()
        self.add_line(f"}} {struct.name};")

    def add_function_prototype(self, func: CFunc) -> None:
        if func.comment != "":
            self.start_comment()
            self.add_lines(func.comment, prefix=" * ")
            self.end_comment()

        arguments = ", ".join(f"{arg.type} {arg.name}" for arg in func.arguments)
        if arguments == "":
            arguments = "void"

        qualifier = f"{func.qualifier} " if func.qualifier else ""
        self.add_line(f"{qualifier}{func.return_type} {func.name}({arguments});")

    def add_function_definition(
        self, func: CFunc, add_comment: Optional[bool] = False
    ) -> None:
        if add_comment and func.comment != "":
            self.start_comment()
            self.add_lines(func.comment, prefix=" * ")
            self.end_comment()

        arguments = ", ".join(f"{arg.type} {arg.name}" for arg in func.arguments)
        qualifier = f"{func.qualifier} " if func.qualifier else ""
        self.add_line(f"{qualifier}{func.return_type} {func.name}({arguments})")
        self.add_line("{")
        self.indent()
        self.add_code(func.body)
        self.unindent()
        self.add_line("}")

    def start_switch(self, value: str) -> None:
        self.add_line(f"switch ({value})")
        self.add_line("{")
        self.indent()

    def add_switch_case(self, case: str) -> None:
        self.add_line(f"case {case}:")

    def add_switch_default(self) -> None:
        self.add_line(f"default:")

    def start_switch_case(self) -> None:
        self.add_line("{")
        self.indent()

    def add_switch_break(self) -> None:
        self.add_line("break;")
        self.unindent()
        self.add_line("}")

    def end_switch(self) -> None:
        self.unindent()
        self.add_line("}")

    def start_if(self, condition: str) -> None:
        self.add_line(f"if ({condition})")
        self.add_line("{")
        self.indent()

    def start_else(self) -> None:
        self.unindent()
        self.add_line("}")
        self.add_line("else")
        self.add_line("{")
        self.indent()

    def end_if(self) -> None:
        self.unindent()
        self.add_line("}")

    def start_for(self, condition) -> None:
        self.add_line(f"for({condition})")
        self.add_line("{")
        self.indent()

    def end_for(self) -> None:
        self.unindent()
        self.add_line("}")


def clamp_signal_code(signal: CanSignal, msg: CanMessage, dest: str, src: str) -> str:
    """
    Generates code for setting signal value var "dest" to equal "src" w/ clamping.
    Used by TX and RX signal value setters.
    """
    cw = CWriter()

    # If a float, don't set anything if the value is NaN
    if signal.datatype() == CanSignalDatatype.FLOAT:
        cw.start_if(f"{src} == NAN")
        cw.add_line("return;")
        cw.end_if()
        cw.add_line()

    min_val = CMacrosConfig.min(msg.name, signal.name)
    max_val = CMacrosConfig.max(msg.name, signal.name)

    if any(
        [
            # Don't need to clamp booleans
            signal.datatype() == CanSignalDatatype.BOOL,
            # Get a warning if trying to clamp less than the max val for uint32
            signal.representation() == CanSignalDatatype.UINT
            and signal.max_val == max_uint_for_bits(32),
        ]
    ):
        cw.add_line(f"{dest} = {src};")

    # Uints set a warning if clamping > 0 (since they're >=0 by definition)
    elif signal.representation() == CanSignalDatatype.UINT and signal.min_val == 0:
        cw.add_line(f"{dest} = ({src} > {max_val}) ? {max_val} : {src};")

    # Otherwise, clamp between min and max
    else:
        cw.add_line(
            f"const {signal.datatype()} tmp = {src} < {min_val} ? {min_val} : {src};"
        )
        cw.add_line(f"{dest} = tmp > {max_val} ? {max_val} : tmp;")

    return str(cw)
