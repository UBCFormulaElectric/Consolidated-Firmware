import pandas as pd
import os
import numpy as np

# Replace with the path to your directory containing the cell data files
path = r"C:\Users\lkevi\OneDrive\Desktop\Coding\UBCFE\Data\CellCharData"

def main():


    sumCellCapacity = 0
    numCells = 0
    voltageDfList = []
    for cellFileName in os.listdir(path):
        # Checks for temporary xlsx files and also ensures you have xlsx files
        if not cellFileName.endswith(".xlsx") or cellFileName.startswith("~$"):
            continue
        else:
            cellFilePath = os.path.join(path, cellFileName)
            rawCellData = pd.read_excel(cellFilePath, sheet_name=cellFileName.split(".")[0])

            # Checks for incomplete discharge cycles and does not include them in average cell capacity calculation
            if rawCellData["Discharging Coulombs [Ah]"].iloc[-1] > 2.6: # The datasheet states cell capcity is 2.7-2.9 Ah
                cellCapacity = rawCellData["Discharging Coulombs [Ah]"].iloc[-1]
                sumCellCapacity += cellCapacity
                numCells += 1

            restingData = rawCellData[rawCellData["Current [A]"] < 0.001].copy()
            voltageDf = restingData[["Voltage [V]", "Discharging Coulombs [Ah]"]]

            voltageDfList.append(voltageDf)

    avgCellCapacity = sumCellCapacity / numCells

    print("Average Cell Capacity: ", avgCellCapacity)

    summedVoltageDf = pd.DataFrame({"Normalized SOC [%]": np.arange(0, 100.5, 0.5)[::-1], "Voltage [V]": np.zeros(201), "Number of Values Averaged": np.zeros(201)})
    socLUT = pd.DataFrame()
    for voltageDf in voltageDfList:
        voltageDf["Normalized SOC [%]"] = (1- voltageDf["Discharging Coulombs [Ah]"] / avgCellCapacity )* 100

        # Create resampling bounds for cycle
        minSOC = voltageDf["Normalized SOC [%]"].min().round(0)
        if not minSOC > 0:
            minSOC = 0
        newSOC = np.arange(minSOC, 100.5, 0.5).astype(float)

        # Numpy needs strictly sorted arrays for interpolation, we used discharge cycles so reverse is needed
        reversedSOCs = voltageDf["Normalized SOC [%]"][::-1]
        reversedVoltages = voltageDf["Voltage [V]"][::-1]

        newVoltage = (np.interp(newSOC, reversedSOCs, reversedVoltages))

        resampledDf = pd.DataFrame({'Normalized SOC [%]': newSOC, 'Voltage [V]': newVoltage})

        resampledDf = resampledDf[::-1].reset_index()

        resampledDf["Number of Values Averaged"] = 1                # Tracks # of datapoints to get average for mismatched discharge cycles
        resampledDf = resampledDf.reindex(range(201), fill_value=0)    
        summedVoltageDf["Voltage [V]"] += resampledDf["Voltage [V]"]
        summedVoltageDf["Number of Values Averaged"] += resampledDf["Number of Values Averaged"]
    
    socLUT["Voltage [V]"] = summedVoltageDf["Voltage [V]"] / summedVoltageDf["Number of Values Averaged"]
    socLUT["Normalized SOC [%]"] = summedVoltageDf["Normalized SOC [%]"]

    socLUT.to_csv(os.path.join(path, "SOC_LUT.csv"), index=False)

        

if __name__ == "__main__":
    main()