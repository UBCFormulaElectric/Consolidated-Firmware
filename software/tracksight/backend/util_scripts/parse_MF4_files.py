from asammdf import MDF
import pandas as pd
import sys, os
from pathlib import Path
import cantools
from collections import defaultdict

project_root = os.path.dirname(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
)
sys.path.append(project_root)
import Definitions

# Specify the path to your MF4 file and DBC file
MF4_DIR = Path(Definitions.ROOT_DIR) / "app" / "mf4_logs"
CSV_DIR = Path(Definitions.ROOT_DIR) / "app" / "data"
DBC_DIR = Path(Definitions.ROOT_DIR) / "app" / "dbcs"
outputFileName = "parsedData.csv"

# load MDF/DBC files from an input folder and specify output destination
files = os.listdir(MF4_DIR)
logfiles = list(filter(lambda x: x[-4:] == ".MF4", files))
for i in range(len(logfiles)):
    logfiles[i] = MF4_DIR / logfiles[i]

dest = "output"
dbcFileList = os.listdir(DBC_DIR)
dbcList = []
for file in dbcFileList:
    dbcList.append(cantools.db.load_file(DBC_DIR / file))

if dbcList:
    dbcIdSets = defaultdict(set)
    for i, dbc in enumerate(dbcList):
        for msg in dbc.messages:
            dbcIdSets[i].add(msg.frame_id)

    mdf = MDF.concatenate(logfiles)
    df1 = mdf.to_dataframe()
    res = {}
    for index, row in df1.iterrows():
        label = row["CAN_DataFrame.CAN_DataFrame.ID"]
        data = row["CAN_DataFrame.CAN_DataFrame.DataBytes"]
        time = index
        dataBytes = data.tobytes()
        for i, dbc in enumerate(dbcList):
            if label in dbcIdSets[i]:
                msg = dbc.get_message_by_frame_id(label)
                decode = dbc.decode_message(label, dataBytes)
                res[time] = decode
                break
    resdf = pd.DataFrame.from_dict(res)
    resdf = resdf.T
    resdf.to_csv(outputFileName)
    print(f"Parsed data to {outputFileName}")
else:
    print("No dbc found")
