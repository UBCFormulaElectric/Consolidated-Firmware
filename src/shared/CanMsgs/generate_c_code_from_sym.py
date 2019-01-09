#!/bin/python3
"""
This file contains all the functionality required to generate C code from 
our .sym file
"""

from argparse import ArgumentParser
from cantools.database.can.c_source import generate
from cantools.database import load_file

def generate_code_from_sym_file(database_name):
    """
    Generates C source code for the given .sym file 
    """
    dbase = load_file(database_name + ".sym", database_format="sym")

    filename_h = database_name + '.h'
    filename_c = database_name + '.c'

    header, source = generate(dbase, database_name, filename_h)

    with open(filename_h, 'w') as fout:
        fout.write(header)

    with open(filename_c, 'w') as fout:
        fout.write(source)

if __name__ == "__main__":
    sym_filename = "CanMsgs"
    generate_code_from_sym_file(sym_filename)

