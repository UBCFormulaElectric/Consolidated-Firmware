Serial Debugger (Using Nanopb)
==============================

See ../third_party/nanopb for more information on how to use nanopb or visit https://github.com/nanopb/nanopb.

Changing the serial debugger protocol buffer workflow:

1. Make desired changes to .proto file.

2. Run generate_proto_py.sh to generate the corresponding python library (ending in _pb2.py).

3. Load cmake and then build all embedded binaries to generate the corresponding C library (ending in .pb.c and .pb.h).