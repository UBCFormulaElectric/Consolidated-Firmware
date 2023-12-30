import os
from typing import List
from dataclasses import dataclass
from argparse import ArgumentParser
import pyclibrary
import jinja2


@dataclass(frozen=True)
class CFunction:
    name: str
    return_type: str
    param_types: List[str]


def parse_header_file(file: str) -> List[CFunction]:
    """
    Use `pyclibrary` to extract functions from a header file. 
    Return them as a list of CFunctions.

    """
    parser = pyclibrary.CParser(files=[file])

    # Get parsed function definitions.
    parsed_functions = parser.defs["functions"]
    functions = []
    for func_name, func_type in parsed_functions.items():
        # pyclibrary encodes return value into the `type_spec` variable.
        return_type = parse_type(func_type.type_spec)

        # pyclibrary encodes parameters into the `declarators` variable, as a tuple of
        # 3-tuples of the form:
        # (<parameter-name-or-None>, <param-type>, None)
        # (don't ask me why, this is dumb)
        param_types = []
        for _, param_type, _ in func_type.declarators[0]:
            param_types.append(parse_type(param_type))

        if len(param_types) == 1 and param_types[0] == "void":
            param_types.clear()

        functions.append(
            CFunction(name=func_name, return_type=return_type, param_types=param_types)
        )

    return functions


def parse_type(type: pyclibrary.c_parser.Type) -> str:
    """
    Helper to parse the Type class from `pyclibrary` into text representing the type.

    """
    # `pyclibrary` parsed types are basically tuples where the seperate "parts" of the type are
    # elements of the tuple.
    # Ex. uint8_t* -> ("uint8_t", "*"")
    # So join each type to get the C syntax equivalent.
    # Similarly, qualifiers are stored in the `type_quals` variable.
    qualifiers = "".join(type.type_quals[0])
    types = ""
    for part in type:
        if isinstance(part, list):
            types += "*"
        else:
            types += part
            
    return f"{qualifiers} {types}" if qualifiers != "" else types


def generate_output(
    header_path: str, output_header: str, output_source: str, functions: List[CFunction]
) -> None:
    """
    Use jinja2 templates to generate a file which fakes a list of functions. Creates:
    - Faked header file: Functions declarations to interact with the fake (set return value, 
        set call count, reset the fake).
    - Faked source file: Definitions for functions to interact with the fake, as well as 
        a faked implementation of the function being faked.

    """
    # Load jinja2 templates.
    module_dir = os.path.dirname(os.path.relpath(__file__))
    template_dir = os.path.join(module_dir, "templates")
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(template_dir))
    header_template = env.get_template("fake_template.hpp.j2")
    source_template = env.get_template("fake_template.cpp.j2")

    fake_declarations_arr = []
    fake_definitions_arr = []
    for function in functions:
        if function.return_type == "void":
            if len(function.param_types) == 0:
                template = "void_func_no_params"
            else:
                template = "void_func_params"
        else:
            if len(function.param_types) == 0:
                template = "value_returning_func_no_params"
            else:
                template = "value_returning_func_params"

        declarations_template = env.get_template(f"{template}.hpp.j2")
        definition_template = env.get_template(f"{template}.cpp.j2")

        data = {
            "name": function.name,
            "return_type": function.return_type,
            "params": [
                {
                    "type": param_type,
                    "name": f"arg{i}",
                }
                for i, param_type in enumerate(function.param_types)
            ],
        }
        fake_declarations = declarations_template.render(data)
        fake_definitions = definition_template.render(data)

        fake_declarations_arr.append(fake_declarations)
        fake_definitions_arr.append(fake_definitions)

    file_name = os.path.basename(header_path)
    file_name_without_extension = os.path.splitext(file_name)[0]

    # Write faked header output.
    with open(output_header, "w") as file:
        file.write(
            header_template.render(
                {
                    "module": file_name_without_extension,
                    "declarations": fake_declarations_arr,
                }
            )
        )

    # Write faked source output.
    with open(output_source, "w") as file:
        file.write(
            source_template.render(
                {
                    "module": file_name_without_extension,
                    "definitions": fake_definitions_arr,
                }
            )
        )


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--header", type=str, required=False)
    parser.add_argument("--output-header", type=str, required=False)
    parser.add_argument("--output-source", type=str, required=False)
    args = parser.parse_args()

    if args.header:
        if os.path.isfile(args.header):
            # Parse input.
            functions = parse_header_file(args.header)

            # Output module name.
            header_name = os.path.split(args.header)[-1]
            module_name = header_name.split(".")[0]

            # Generate output.
            input_module = args.header.split(".")[0]
            generate_output(
                header_path=args.header,
                output_header=args.output_header,
                output_source=args.output_source,
                functions=functions,
            )
        else:
            raise OSError(f"Cannot find header: {args.header}")
