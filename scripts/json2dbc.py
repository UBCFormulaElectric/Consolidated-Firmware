# json to dbc converter

# RUN: python json2dbc.py <file_name.json> <file_name.dbc>
#  eg. python json2dbc.py myoutput.json foo.dbc

import json
import re
import sys
from pprint import pprint # for testing

with open(sys.argv[1]) as json_file, open(sys.argv[2], 'w') as f:
    d = json.load(json_file)
    for m in d:
        msg = d[m]
        f.write(f"BO_ {msg['can_id']} {m}: {msg['bytes']} {msg['sender']}\n")
        for sig in msg["signals"]:
                # print(sig)
                # print(type(sigs))
                # print()
                str = ""
                if sig['little_endian']:
                    str = str + "1"
                else:
                    str = str + "0"
                if sig['unsigned']:
                    str = str + "+"
                else:
                    str = str + "-"
                f.write(f"SG_ {sig['name']} : {sig['bit_start']}|{sig['length']}@{str} ({sig['scale']},{sig['offset']}) [{sig['min']}|{sig['max']}] \"{sig['unit']}\" {sig['receiver']}\n")
        #    print(d[msg]["messages"])
        #    print()
        f.write("\n")
    # pprint(d)
