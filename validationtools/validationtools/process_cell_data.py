import pandas as pd
import os

path = r"C:\Users\lkevi\OneDrive\Desktop\Coding\UBCFE\Data\CellCharData"

def main():
    for cellFileName in os.listdir(path):
        cellFilePath = os.path.join(path, cellFileName)
        rawCellData = pd.read_excel(cellFilePath, sheet_name=None)
        


if __name__ == "__main__":
    main()