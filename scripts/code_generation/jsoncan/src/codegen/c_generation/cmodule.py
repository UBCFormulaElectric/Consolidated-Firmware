from abc import ABC


class CModule(ABC):
    """
    ABC for a C module (i.e. pair of header .h and source .c files)
    """

    def header(self) -> str: ...

    def source(self) -> str: ...

    def header_template(self): ...

    def source_template(self): ...
