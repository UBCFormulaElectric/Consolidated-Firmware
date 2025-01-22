from validationtools.loadBank._loadBank import *
from validationtools.powerSupply._powerSupply import *
from validationtools.logger.logger import *


# Accoding to datasheet, cell has a peak charging voltage of 4.2V and a cutoff voltage of 2.5V
maxChargingVoltage= 4.2
minDischargingVoltage = 2.5
totalCellCapacity = 2.800 # Ah From datasheet on google drive
chargingRate      = 2.8   # 1C Rate in amps 
restingTimeSeconds = 30 
activeTimeSeconds = 30


def getCoulombs( current, timeStepSeconds)->float:
    coulombsAh = current * timeStepSeconds /3600 # seconds to hrs conversion
    return coulombsAh

def getChargingRow(logger, powerSupply):
    lastRow = logger.getLastRow
    voltage = powerSupply.measure_voltage
    current = powerSupply.measure_current
    if lastRow is not None:
        chargingCoulombs = getCoulombs(current, logger.getTimeIncrement()) + lastRow["Charging Coulombs [Ah]"] # Charging Coulombs Col
        dischargingCoulombs = lastRow["Discharging Coulombs [Ah]"]
    else:
        chargingCoulombs = 0
        dischargingCoulombs = None
    soc = chargingCoulombs / totalCellCapacity * 100

    row = [voltage, current, chargingCoulombs, dischargingCoulombs, soc]

    return row

def getDischargingRow(logger, loadBank):
    lastRow = logger.getLastRow()
    voltage = loadBank.measure_voltage()
    current = loadBank.measure_current()
    if lastRow is not None:
        dischargingCoulombs = getCoulombs(current, logger.getTimeIncrement()) + lastRow["Discharging Coulombs [Ah]"] # Charging Coulombs Col
        chargingCoulombs = lastRow["Charging Coulombs [Ah]"]
    else:
        dischargingCoulombs = 0
        chargingCoulombs = 0
    soc = (1- dischargingCoulombs / totalCellCapacity) * 100

    row = [voltage, current, chargingCoulombs, dischargingCoulombs, soc]

    return row


def main()->None:
    loadBank = LoadBank()
    powerSupply = PowerSupply()
    soc = 100

    loggingCols = \
    [
        "Voltage [V]",
        "Current [A]",
        "Charging Coulombs [Ah]",
        "Discharging Coulombs [Ah]",
        "SOC"
    ]

    logger = Logger(r"C:\Users\lkevi\OneDrive\Desktop\Coding\UBCFE\Data", loggingCols)

    # According to INR-18650-P28A datasheets, the cells capacity can range from standard of 2800mAh to 2600mAh
    # (or 2700mAh minimum depending on the datasheet consulted). So a cutoff voltage is set to avoid overcharge and discharge
    while(soc < 100 and loadBank.measure_voltage() < maxChargingVoltage):
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.enable_output()
        powerSupply.set_current(chargingRate)

        time.sleep(activeTimeSeconds)
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.enable_output()


        time.sleep(restingTimeSeconds)
        soc = row[-1]

    loadBank.set_current(chargingRate)

    while(soc > 0 and loadBank.measure_voltage() > minDischargingVoltage):
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
    
    
     




if __name__ == "__main__":
    main()
    # l = LoadBank()

    # l.set_current(0.500) # set it to draw 500mA
    # l.enable_load()

    # time.sleep(5) # sleep for 5 seconds before measurment

    # V_load = l.measure_voltage()
    # print(V_load)

    # l.disable_load()