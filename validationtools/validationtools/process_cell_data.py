import pandas as pd
import os
import numpy as np

path = r"C:\Users\lkevi\OneDrive\Desktop\Coding\UBCFE\Data\CellCharData"

def main():


    sumCellCapacity = 0
    numCells = 0
    voltageDfList = []
    for cellFileName in os.listdir(path):
        cellFilePath = os.path.join(path, cellFileName)
        rawCellData = pd.read_excel(cellFilePath, sheet_name=None)
        if rawCellData["Discharging Coulombs [Ah]"].iloc[-1] > 2.6: # The datasheet states cell capcity is 2.7-2.9 Ah
            cellCapacity = rawCellData["Discharging Coulombs [Ah]"].iloc[-1]
            sumCellCapacity += cellCapacity
            numCells += 1

        restingData = rawCellData[["Current [A]"] > 0.001].copy()
        voltageDf = restingData[["Voltage [V]", "Discharging Coulombs [Ah]"]]

        voltageDfList.append(voltageDf)

    avgCellCapacity = sumCellCapacity / numCells

    summedVoltageDf = pd.DataFrame()
    socLUT = pd.DataFrame()
    for voltageDf in voltageDfList:
        voltageDf["Normalized SOC [%]"] = (1- voltageDf["Discharging Coulombs [Ah]"] / avgCellCapacity )* 100
        voltageDf.set_index("Normalized SOC [%]", inplace=True)
        newSOC = np.arange(0, 101, 1)

        resampledDf = voltageDf.reindex(newSOC)
        resampledDf['Voltage [V]'] = resampledDf['Voltage [V]'].interpolate(method='linear')

        resampledDf = resampledDf.reset_index().rename(columns={'index': 'SOC'})
        resampledDf["Number of Values Averaged"] = 1

        if summedVoltageDf.empty:
            summedVoltageDf = resampledDf
        else:
            summedVoltageDf["Voltage [V]"] += resampledDf["Voltage [V]"]
            summedVoltageDf["Number of Values Averaged"] += resampledDf["Number of Values Averaged"]
    
    socLUT["Voltage [V]"] = summedVoltageDf["Voltage [V]"] / summedVoltageDf["Number of Values Averaged"]
    socLUT["Normalized SOC [%]"] = summedVoltageDf["Normalized SOC [%]"]

    socLUT.to_csv(os.path.join(path, "SOC_LUT.csv"), index=False)

        

if __name__ == "__main__":
    main()