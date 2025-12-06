from abc import ABC


class CppModule(ABC):
    """
    ABC for a Cpp module (i.e. pair of header .h and source .cpp files)
    """

    def header_template(self) -> str:
        ...

    def source_template(self) -> str:
        ...
