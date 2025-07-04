from abc import ABC


class CModule(ABC):
    """
    ABC for a C module (i.e. pair of header .h and source .c files)
    """

    def header_template(self) -> str:
        ...

    def source_template(self) -> str:
        ...
