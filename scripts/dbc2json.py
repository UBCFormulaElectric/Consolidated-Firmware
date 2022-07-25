# dbc to json converter

# RUN: python dbc2json.py <file_name.dbc> <file_name.json>
#  eg. python dbc2json.py App_canMsgs.dbc myoutput.json

# script assumption (.dbc file requirement):  blank line == end of message

import json
import re
import sys
#from pprint import pprint # for testing

json_dict = dict()
msg = False
msg_name = ""
sig_list = []
with open(sys.argv[1], 'r') as infile:
    for line in infile:
        if msg: # we already read a message so this line could be a signal
            if re.search(r"^SG_\s", line): # start of message
                result_1 = re.match(r"^SG_\s(.+):\s*(\d+)\|(\d+)@([01])([+-])\s+\((.*?),(\d+)\)\s+\[(.*?)\|(.*?)\]\s+\"(.*?)\"\s+(.*?)$", line)
                sig_list.append({"name": result_1[1].strip(),
                                "bit_start": result_1[2].strip(),
                                "length": result_1[3].strip(),
                                "little_endian": (result_1[4] == "1"),
                                "unsigned": (result_1[5] == "+"),
                                "scale": result_1[6].strip(),
                                "offset": result_1[7].strip(),
                                "min": result_1[8].strip(),
                                "max": result_1[9].strip(),
                                "unit": result_1[10].strip(),
                                "receiver": result_1[11].strip()
                                })
            elif not line.strip(): # empty line means end of a message, reset
                msg = False
                json_dict[msg_name]["signals"] = sig_list
                msg_name = ""
                continue
        if re.search(r"^BO_\s", line): # start of message
            result_0 = re.match(r"^BO_\s(\d+)\s(.+):\s(\d+)\s(.*)", line)
            msg_name = result_0[2]
            json_dict[msg_name] = {"can_id": result_0[1].strip(),
                                   "bytes": result_0[3].strip(),
                                   "sender": result_0[4].strip(),
                                   "signals": {}
                                   }
            msg = True
            sig_list = []
            continue
        else:
            pass # TODO: dbc header stuff (not messages)

#pprint(json_dict)          # for testing
#print(len(json_dict))

with open(sys.argv[2], 'w') as outfile:
    # json.dump(json_dict, outfile)
    # print(json.dumps(json_dict, sort_keys=True, indent=4))
    output_str = json.dumps(json_dict, sort_keys=True, indent=4)
    outfile.write(output_str)
