from loadBank._loadBank import *
from powerSupply._powerSupply import *
from logger.logger import *

# Accoding to datasheet, cell has a peak charging voltage of 4.2V and a cutoff voltage of 2.5V
maxChargingVoltage= 4.2
minDischargingVoltage = 2.5
totalCellCapacity = 2.800 # Ah From datasheet on google drive
chargingRate      = 2.8   # 1C Rate in amps 
restingTimeSeconds = 30 
activeTimeSeconds = 10
# Second channel of the power supply allows for lower voltages, higher current
powerSupplyChannel = 2
initialSOC = 55


def getCoulombs( current, timeStepSeconds)->float:
    coulombsAh = current * timeStepSeconds /3600 # seconds to hrs conversion
    return coulombsAh

def getChargingRow(logger: Logger, powerSupply: PowerSupply):
    lastRow = logger.getLastRow()
    voltage = powerSupply.measure_voltage(powerSupplyChannel)
    current = powerSupply.measure_current(powerSupplyChannel)
    if lastRow is not None:
        chargingCoulombs = getCoulombs(current, logger.getTimeIncrement()) + lastRow["Charging Coulombs [Ah]"] # Charging Coulombs Col
        dischargingCoulombs = lastRow["Discharging Coulombs [Ah]"]
        soc = lastRow["SOC"] + chargingCoulombs / totalCellCapacity * 100
    else:
        chargingCoulombs = 0
        dischargingCoulombs = None
        soc = initialSOC + chargingCoulombs / totalCellCapacity * 100

    row = [voltage, current, chargingCoulombs, dischargingCoulombs, soc]

    return row

def getDischargingRow(logger: Logger, loadBank: LoadBank):
    lastRow = logger.getLastRow()
    voltage = loadBank.measure_voltage()
    current = loadBank.measure_current()
    if lastRow is not None:
        dischargingCoulombs = getCoulombs(current, logger.getTimeIncrement()) + lastRow["Discharging Coulombs [Ah]"] # Charging Coulombs Col
        chargingCoulombs = lastRow["Charging Coulombs [Ah]"]
        soc = lastRow["SOC"] - dischargingCoulombs / totalCellCapacity * 100
    else:
        dischargingCoulombs = 0
        chargingCoulombs = 0
        soc = initialSOC - dischargingCoulombs / totalCellCapacity * 100

    row = [voltage, current, chargingCoulombs, dischargingCoulombs, soc]

    return row


def main()->None:
    loadBank = LoadBank()
    powerSupply = PowerSupply()
    soc = initialSOC #change to match either top of charge or bottom of charge
    powerSupply.set_voltage(maxChargingVoltage, powerSupplyChannel)

    loggingCols = \
    [
        "Voltage [V]",
        "Current [A]",
        "Charging Coulombs [Ah]",
        "Discharging Coulombs [Ah]",
        "SOC"
    ]

    # Replace with your own data path
    logger = Logger(r"C:\Users\lkevi\OneDrive\Desktop\Coding\UBCFE\Data", loggingCols)

    # According to INR-18650-P28A datasheets, the cells capacity can range from standard of 2800mAh to 2600mAh
    # (or 2700mAh minimum depending on the datasheet consulted). So a cutoff voltage is set to avoid overcharge and discharge
    # For now we are on;y doing discharging
    # chargeCharacterization(loadBank, powerSupply, logger)

    dischargeCharacterization(loadBank, powerSupply, logger)
    
    
     
def dischargeCharacterization(loadBank: LoadBank, powerSupply: PowerSupply, logger: Logger):
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

def chargeCharacterization(loadBank: LoadBank, powerSupply: PowerSupply, logger: Logger):
        # According to INR-18650-P28A datasheets, the cells capacity can range from standard of 2800mAh to 2600mAh
    # (or 2700mAh minimum depending on the datasheet consulted). So a cutoff voltage is set to avoid overcharge and discharge
    while(soc < 100 and loadBank.measure_voltage() < maxChargingVoltage):
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


if __name__ == "__main__":
    main()
    # l = LoadBank()

    # l.set_current(0.500) # set it to draw 500mA
    # l.enable_load()

    # time.sleep(5) # sleep for 5 seconds before measurment

    # V_load = l.measure_voltage()
    # print(V_load)

    # l.disable_load()