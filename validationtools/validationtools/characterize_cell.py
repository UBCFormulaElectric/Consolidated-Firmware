from loadBank._loadBank import *
from powerSupply._powerSupply import *
from logger.logger import *

# Accoding to datasheet, cell has a peak charging voltage of 4.2V and a cutoff voltage of 2.5V
maxVoltage= 4.2
minVoltage = 2.5
totalCellCapacity = 2.800 # Ah From datasheet on google drive
chargingRate      = 2.8   # 1C Rate in amps 
restingTimeSecondsMaxSoc = 180
restingTimeSecondsMinSoc = 420
activeTimeSeconds = 70
# Second channel of the power supply allows for lower voltages, higher current
powerSupplyChannel = 2
initialSOC = 100


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
    loadBank = LoadBank()
    # powerSupply = PowerSupply()
    # powerSupply.set_voltage(maxVoltage, powerSupplyChannel)
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
    logger = Logger(r"C:\Users\lkevi\OneDrive\Desktop\Coding\UBCFE\Data", loggingCols)

    # For now we are only doing discharging
    # chargeCharacterization(loadBank, powerSupply, logger, soc)
    try:
        dischargeCharacterization(loadBank, logger, soc)
    except Exception as e:
        print(e)
        loadBank.disable_load()
        # powerSupply.disable_output(powerSupplyChannel)
        raise
    
    
     
def dischargeCharacterization(loadBank: LoadBank, logger: Logger, soc: float):
    startCellVoltage = loadBank.measure_voltage()     # Initial measurement is taken and set as the max cell voltage 
                                                    # (cell voltage may be above 4.2 slightly or you can start in middle of charge)
    restingTimeSeconds = restingTimeSecondsMaxSoc   # Resting time is set to resting time at top of charge
    deltaRestTime = (restingTimeSecondsMinSoc - restingTimeSecondsMaxSoc) / (minVoltage - startCellVoltage) # Slope to linearly interpolate rest time (BoC need more rest time than ToC)
    while(loadBank.measure_voltage() > minVoltage):
        restingTimeSeconds = restingTimeSeconds + deltaRestTime * (loadBank.measure_voltage() - startCellVoltage)
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

def chargeCharacterization(loadBank: LoadBank, powerSupply: PowerSupply, logger: Logger, soc: float):
    # According to INR-18650-P28A datasheets, the cells capacity can range from standard of 2800mAh to 2600mAh
    # (or 2700mAh minimum depending on the datasheet consulted). So a cutoff voltage is set to avoid overcharge and discharge
    startCellVoltage = powerSupply.measure_voltage(powerSupplyChannel)
    restingTimeSeconds = restingTimeSecondsMinSoc
    deltaRestTime = (restingTimeSecondsMaxSoc-restingTimeSecondsMinSoc)/ (startCellVoltage - maxVoltage)
    while(powerSupply.measure_voltage(powerSupplyChannel) < maxVoltage):
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
    # l = LoadBank()

    # l.set_current(0.500) # set it to draw 500mA
    # l.enable_load()

    # time.sleep(5) # sleep for 5 seconds before measurment

    # V_load = l.measure_voltage()
    # print(V_load)

    # l.disable_load()