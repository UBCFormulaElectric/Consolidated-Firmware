from asammdf import MDF
import pandas as pd
import sys, os
from pathlib import Path
import cantools

project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.append(project_root)
import Definitions 

# Specify the path to your MF4 file and DBC file
MF4_DIR = Path(Definitions.ROOT_DIR) / "app" / "mf4_logs" 
CSV_DIR = Path(Definitions.ROOT_DIR) / "app" / "data" 

# load MDF/DBC files from an input folder and specify output destination
files = os.listdir(MF4_DIR)
print(files)
logfiles = list(filter(lambda x: x[-4:] == ".MF4", files))
for i in range(len(logfiles)):
    logfiles[i] = MF4_DIR / logfiles[i]
dbc_file_path = 'canMsgs.dbc'
dest = "output"
print(dbc_file_path)
dbc = cantools.db.load_file(dbc_file_path)
print(dbc.messages)

# concatenate & DBC convert + convert to pandas dataframe
if dbc:
    mdf = MDF.concatenate(logfiles)
    df1 = mdf.to_dataframe()
    #print(df1.index)
    res = {}
    for index, row in df1.iterrows():
        label = row["CAN_DataFrame.CAN_DataFrame.ID"]
        data = row["CAN_DataFrame.CAN_DataFrame.DataBytes"]
        try:
            msg = dbc.get_message_by_frame_id(label)
            #print(msg)
            time = index
            #print(time)
            decode = dbc.decode_message(label, data)
            #print(decode)
            res[time] = decode
            #print(decode)
        except:
            pass
            #print("Could not find key")
        #print(label, data)
    #print(df1)
    resdf = pd.DataFrame.from_dict(res)
    resdf = resdf.T
    print(resdf)
    #print(df1.columns)
else:
    print("No dbc found")
