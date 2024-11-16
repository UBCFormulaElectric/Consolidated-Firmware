from validationtools.loadBank._loadBank import *
from validationtools.powerSupply._powerSupply import *
from validationtools.logger.logger import *

totalCellCapacity = 100000 # mAh Filler Value
chargingRate      = 0.2      # 1C Rate in amps Filler Value


def getCoulombs( current, timeStepSeconds)->float:
    coulombs = current * timeStepSeconds
    return coulombs

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
    # powerSupply = PowerSupply()
    soc = 90

    loggingCols = \
    [
        "Voltage [V]",
        "Current [A]",
        "Charging Coulombs [Ah]",
        "Discharging Coulombs [Ah]",
        "SOC"
    ]

    logger = Logger(r"C:\Users\lkevi\OneDrive\Desktop\Coding\UBCFE\Data", loggingCols)

    # while(soc < 100):
    #     powerSupply.set_voltage(1)
    #     powerSupply.set_current(chargingRate)
    #     row = getChargingRow(logger, powerSupply)
    #     logger.storeRow(row)

    #     time.sleep(10)
    #     powerSupply.set_current(0)

    #     row = getChargingRow(logger, powerSupply)
    #     logger.storeRow(row)
    #     time.sleep(10)

    #     soc = row[-1]

    # loadBank.set_voltage(3.6)
    loadBank.set_current(chargingRate)
    loadBank.enable_load()

    while(soc > 0):
        # loadBank.set_voltage(3.6)
        loadBank.set_current(chargingRate)
        
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)

        time.sleep(10)
        loadBank.set_current(0)

        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        time.sleep(10)

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