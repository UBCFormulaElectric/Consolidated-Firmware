import pandas as pd
import os

path = r"C:\Users\lkevi\OneDrive\Desktop\Coding\UBCFE\Data\CellCharData"

def main():
    for cellFileName in os.listdir(path):
        cellFilePath = os.path.join(path, cellFileName)


if __name__ == "__main__":
    main()