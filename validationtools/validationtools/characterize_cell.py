from loadBank._loadBank import *
from powerSupply._powerSupply import *
from logger.logger import *

# Accoding to datasheet, cell has a peak charging voltage of 4.2V and a cutoff voltage of 2.5V
# Value of 3.315V for 12% SOC was found through testing
socVoltageEstimationMap = {12: 3.315, 100: 4.2, 0: 2.5}
totalCellCapacity = 2.800 # Ah From datasheet on google drive

minRestingTimeFirstPortion = 180 # Change back to 3 mins for 100% soc
maxRestingTimeFirstPortion = 660 # Ideally we have something like 10-15 mins for the 10-12% soc

# Second channel of the power supply allows for lower voltages, higher current
powerSupplyChannel = 2
initialSOC = 100
firstPortionCutoffSoc = 12

dataPath = r"C:\Users\Formula Electric\Documents\consolitdated_firm\Consolidated-Firmware\validationtools\validationtools\CellCharacterizationData"

def getCoulombs( current, timeStepSeconds)->float:
    coulombsAh = current * timeStepSeconds /3600 # seconds to hrs conversion
    return coulombsAh

def getChargingRow(logger: Logger, powerSupply: PowerSupply):
    lastRow = logger.getLastRow()
    voltage = powerSupply.measure_voltage(powerSupplyChannel)
    current = powerSupply.measure_current(powerSupplyChannel)
    if lastRow is not None:
        chargingCoulombs = round(getCoulombs(current, logger.getTimeIncrement()) + lastRow["Charging Coulombs [Ah]"], 7) # Charging Coulombs Col
        dischargingCoulombs = lastRow["Discharging Coulombs [Ah]"]
        soc = round(lastRow["Estimated SOC"] + (chargingCoulombs-lastRow["Charging Coulombs [Ah]"]) / totalCellCapacity * 100, 5)
    else:
        chargingCoulombs = 0
        dischargingCoulombs = None
        soc = round(initialSOC + chargingCoulombs / totalCellCapacity * 100, 5)

    row = [voltage, current, chargingCoulombs, dischargingCoulombs, soc]

    return row

def getDischargingRow(logger: Logger, loadBank: LoadBank):
    lastRow = logger.getLastRow()
    voltage = loadBank.measure_voltage()
    current = loadBank.measure_current()
    if lastRow is not None:
        dischargingCoulombs = round(getCoulombs(current, logger.getTimeIncrement()) + lastRow["Discharging Coulombs [Ah]"], 7) # Charging Coulombs Col
        chargingCoulombs = lastRow["Charging Coulombs [Ah]"]
        soc = round(lastRow["Estimated SOC"] - (dischargingCoulombs-lastRow["Discharging Coulombs [Ah]"]) / totalCellCapacity * 100, 5)
    else:
        dischargingCoulombs = 0
        chargingCoulombs = 0
        soc = round(initialSOC - dischargingCoulombs / totalCellCapacity * 100, 5)

    row = [voltage, current, chargingCoulombs, dischargingCoulombs, soc]

    return row


def main()->None:
    paramError = checkTestVariables()
    if paramError:
        raise Exception(f"Test Variables are not set correctly. {paramError}")
    loadBank = LoadBank()
    # powerSupply = PowerSupply()
    # powerSupply.set_voltage(socVoltageEstimationMap[100], powerSupplyChannel)
    soc = initialSOC # change to match either top of charge or bottom of charge

    loggingCols = \
    [
        "Voltage [V]",
        "Current [A]",
        "Charging Coulombs [Ah]",
        "Discharging Coulombs [Ah]",
        "Estimated SOC"
    ]

    # Replace with your own data path
    logger = Logger(dataPath, loggingCols)

    # For now we are only doing discharging
    # chargeCharacterization(loadBank, powerSupply, logger, soc)
    try:
        dischargeCharacterization(loadBank, logger, soc)
    except Exception as e:
        print(e)
        loadBank.disable_load()
        # powerSupply.disable_output(powerSupplyChannel)
        raise
    
def checkTestVariables()->Exception:
    if firstPortionCutoffSoc not in socVoltageEstimationMap.keys():
        return Exception("First Portion Cutoff SOC is not in the SOC Voltage Estimation Map")
    if not os.exists(dataPath):
        return Exception("Data Path does not exist")
    return None
     
def dischargeCharacterization(loadBank: LoadBank, logger: Logger, soc: float):
    activeTimeSeconds = 120
    chargingRate      = 1.4   # 0.5C Rate in amps 
    startCellVoltage = loadBank.measure_voltage()     # Initial measurement is taken and set as the max cell voltage 
                                                    # (cell voltage may be above 4.2 slightly or you can start in middle of charge)
    restingTimeSeconds = minRestingTimeFirstPortion   # Resting time is set to resting time at top of charge
    deltaRestTime = (maxRestingTimeFirstPortion - minRestingTimeFirstPortion) / (socVoltageEstimationMap[firstPortionCutoffSoc] - startCellVoltage) # Slope to linearly interpolate rest time (BoC need more rest time than ToC)
    while(soc > firstPortionCutoffSoc and loadBank.measure_voltage() > socVoltageEstimationMap[0]):
        restingTimeSeconds = minRestingTimeFirstPortion + deltaRestTime * (loadBank.measure_voltage() - startCellVoltage)
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.enable_load()
        loadBank.set_current(chargingRate)
        

        time.sleep(activeTimeSeconds)
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.disable_load()

        time.sleep(restingTimeSeconds)
        soc = row[-1]

    loadBank.disable_load()
    row = getDischargingRow(logger, loadBank)
    logger.storeRow(row)

    print("Reached BoC Portion of Test, Resting for 30 Mins")

    time.sleep(1800) # give half an hour of resting time before BoC portion of the test
    activeTimeSeconds = 960 # Multiply the active time for the bottom of charge by sixteen and have one-sixteenth of charging rate
    chargingRate = 0.175 # 0.175A is the 0.0625C rate for the cell
    restingTimeSeconds = 10800 # For BoC set resting time for three hours

    print("Starting BoC Portion of Test")
    while(loadBank.measure_voltage() > socVoltageEstimationMap[0]):
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.enable_load()
        loadBank.set_current(chargingRate)
        

        time.sleep(activeTimeSeconds)
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.disable_load()

        time.sleep(restingTimeSeconds)
        soc = row[-1]

    loadBank.disable_load()
    row = getDischargingRow(logger, loadBank)
    logger.storeRow(row)

    # ensures we hit our settling voltage
    for i in range(0, 9):
        time.sleep(300)
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)




def chargeCharacterization(loadBank: LoadBank, powerSupply: PowerSupply, logger: Logger, soc: float):
    # According to INR-18650-P28A datasheets, the cells capacity can range from standard of 2800mAh to 2600mAh
    # (or 2700mAh minimum depending on the datasheet consulted). So a cutoff voltage is set to avoid overcharge and discharge
    startCellVoltage = powerSupply.measure_voltage(powerSupplyChannel)
    restingTimeSeconds = maxRestingTimeFirstPortion
    deltaRestTime = (minRestingTimeFirstPortion-maxRestingTimeFirstPortion)/ (startCellVoltage - socVoltageEstimationMap[100])
    while(powerSupply.measure_voltage(powerSupplyChannel) < socVoltageEstimationMap[100]):
        restingTimeSeconds = restingTimeSeconds + deltaRestTime * (startCellVoltage - powerSupply.measure_voltage(powerSupplyChannel))
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.set_current(chargingRate, powerSupplyChannel)
        powerSupply.enable_output(powerSupplyChannel)

        time.sleep(activeTimeSeconds)
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.disable_output(powerSupplyChannel)


        time.sleep(restingTimeSeconds)
        soc = row[-1]

    powerSupply.disable_output(chargingRate)
    row = getChargingRow(logger, powerSupply)
    logger.storeRow(row)


if __name__ == "__main__":
    main()