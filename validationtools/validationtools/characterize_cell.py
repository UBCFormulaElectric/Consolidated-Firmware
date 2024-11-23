from validationtools.loadBank._loadBank import *
from validationtools.powerSupply._powerSupply import *
from validationtools.logger.logger import *

totalCellCapacity = 2.600 # Ah From datasheet on google drive
chargingRate      = 2.6      # 1C Rate in amps 


def getCoulombs( current, timeStepSeconds)->float:
    coulombsAh = current * timeStepSeconds /3600 # seconds to hrs conversion
    return coulombsAh

def getCellSOC(accumulatedCoulombs):
    return accumulatedCoulombs / totalCellCapacity * 100 # In percent

def getChargingRow(logger, powerSupply):
    lastRow = logger.getLastRow
    voltage = powerSupply.measure_voltage
    current = powerSupply.measure_current
    if lastRow is not None:
        chargingCoulombs = getCoulombs(current, logger.getTimeIncrement()) + lastRow[5] # Charging Coulombs Col
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

    while(soc < 100):
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.enable_output()
        powerSupply.set_current(chargingRate)

        time.sleep(30)
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.enable_output()


        time.sleep(30)
        soc = row[-1]

    loadBank.set_current(chargingRate)

    while(soc > 0):
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.enable_load()
        loadBank.set_current(chargingRate)
        

        time.sleep(30)
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.disable_load()

        time.sleep(30)
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