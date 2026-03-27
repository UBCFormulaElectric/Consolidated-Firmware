from loadBank._loadBank import *
from powerSupply._powerSupply import *
from logger.logger import *
from typing import Callable
import os
import time

#########################################################################
#   This script is currently written for Molicel P30B 18650 cells.      #
#   If this it is to be used for a different cell, ADJUST THE TEST      #
#   VARIABLES BELOW ACCORDING TO THE CELL'S DATASHEET.                  #
#########################################################################

NOMINAL_CELL_VOLTAGE = 3.5
DEFAULT_POWER_SUPPLY_CHANNEL = 2

totalCellCapacity = 3.000 # Ah From datasheet on google drive

minRestingTimeTOC = 300 # Change back to 5 mins for 100% soc
minRestingTimeBOC = 900 # Ideally we have something like 10-15 mins for the 10-12% soc

socBOC = 0
socTOC = 100
#################################################################
# Two important variables that can vary often from test to test #
powerSupplyChannel = DEFAULT_POWER_SUPPLY_CHANNEL               #
initialSOC = socTOC                                             #
#################################################################

activeStatusCheckIntervalSeconds = 5
firstPortionCutoffSoc = 12
# This is used for charging portion of test ONLY since we want to
# mitigate high current prematurely causing overvoltage of cell.
secondPortionCutoffSoc = 88
cRateAmps = totalCellCapacity
base1CActiveTime = 60 # Discharge time for 1C rate, this is the smallest unit of the test step

# Accoding to datasheet, cell has a peak charging voltage of 4.2V and a cutoff voltage of 2.5V

socVoltageEstimationMap = {socTOC: 4.2, socBOC: 2.5}
dataPath = r"C:\Users\Formula Electric\Documents\consolitdated_firm\Consolidated-Firmware\validationtools\validationtools\CellCharacterizationData"


def getCoulombs(current, timeStepSeconds) -> float:
    coulombsAh = current * timeStepSeconds / 3600 # seconds to hrs conversion
    return coulombsAh


def sleepWithStatusChecks(
    totalSleepSeconds: float,
    statusCheck: Callable[[], None],
    checkIntervalSeconds: float = activeStatusCheckIntervalSeconds,
) -> None:
    """
    Sleeps for totalSleepSeconds while running statusCheck every checkIntervalSeconds.
    statusCheck can raise to abort the active step immediately.
    """
    if totalSleepSeconds <= 0:
        return
    if checkIntervalSeconds <= 0:
        raise Exception("checkIntervalSeconds must be greater than 0")

    endTime = time.time() + totalSleepSeconds
    while True:
        remaining = endTime - time.time()
        if remaining <= 0:
            break

        time.sleep(min(checkIntervalSeconds, remaining))
        statusCheck()


def checkDischargeStatus(loadBank: LoadBank) -> None:
    voltage = loadBank.measure_voltage()
    current = loadBank.measure_current()
    print(f"Active discharge check -> V: {round(voltage, 4)} V, I: {round(current, 4)} A")


def checkChargeStatus(powerSupply: PowerSupply) -> None:
    voltage = powerSupply.measure_voltage(powerSupplyChannel)
    current = powerSupply.measure_current(powerSupplyChannel)
    print(f"Active charge check -> V: {round(voltage, 4)} V, I: {round(current, 4)} A")


def getChargingRow(logger: Logger, powerSupply: PowerSupply):
    lastRow = logger.getLastRow()
    voltage = powerSupply.measure_voltage(powerSupplyChannel)
    current = powerSupply.measure_current(powerSupplyChannel)
    if lastRow is not None:
        chargingCoulombs = round(getCoulombs(current, logger.getTimeIncrement()) + lastRow["Charging Coulombs [Ah]"], 7)
        dischargingCoulombs = lastRow["Discharging Coulombs [Ah]"]
        soc = round(lastRow["Estimated SOC"] + (chargingCoulombs - lastRow["Charging Coulombs [Ah]"]) / totalCellCapacity * 100, 5)
    else:
        chargingCoulombs = 0
        dischargingCoulombs = 0
        soc = round(initialSOC + chargingCoulombs / totalCellCapacity * 100, 5)

    row = [voltage, current, chargingCoulombs, dischargingCoulombs, soc]

    return row


def getDischargingRow(logger: Logger, loadBank: LoadBank):
    lastRow = logger.getLastRow()
    voltage = loadBank.measure_voltage()
    current = loadBank.measure_current()
    if lastRow is not None:
        dischargingCoulombs = round(getCoulombs(current, logger.getTimeIncrement()) + lastRow["Discharging Coulombs [Ah]"], 7)
        chargingCoulombs = lastRow["Charging Coulombs [Ah]"]
        soc = round(lastRow["Estimated SOC"] - (dischargingCoulombs - lastRow["Discharging Coulombs [Ah]"]) / totalCellCapacity * 100, 5)
    else:
        dischargingCoulombs = 0
        chargingCoulombs = 0
        soc = round(initialSOC - dischargingCoulombs / totalCellCapacity * 100, 5)

    row = [voltage, current, chargingCoulombs, dischargingCoulombs, soc]

    return row


def main() -> None:
    #########################################################################
    # UNCOMMENT THE MODE YOU WANT TO RUN, ADJUST TEST VARIABLES ACCORDINGLY #
    mode = "discharge"                                                      #
    # mode = "charge"                                                       #
    #########################################################################

    psuChannel = DEFAULT_POWER_SUPPLY_CHANNEL
    global initialSOC

    paramError = checkTestVariables(mode)
    if paramError:
        raise Exception(f"Test Variables are not set correctly. {paramError}")
    loadBank = LoadBank()
    powerSupply = PowerSupply()
    initialSOC = socTOC if mode == "discharge" else socBOC
    powerSupplyChannel = psuChannel
    soc = initialSOC # change to match either top of charge or bottom of charge

    loggingCols = [
        "Voltage [V]",
        "Current [A]",
        "Charging Coulombs [Ah]",
        "Discharging Coulombs [Ah]",
        "Estimated SOC"
    ]

    # Replace with your own data path
    logger = Logger(dataPath, loggingCols)

    if mode == "discharge":
        try:
            dischargeCharacterization(loadBank, logger, soc)
        except Exception as e:
            print(e)
            loadBank.disable_load()
            raise
        finally:
            loadBank.disable_load()
    elif mode == "charge":
        try:
            chargeCharacterization(powerSupply, logger, soc)
        except Exception as e:
            print(e)
            powerSupply.disable_output(powerSupplyChannel)
            raise
        finally:
            powerSupply.disable_output(powerSupplyChannel)
    else:
        raise Exception("Mode variable is not set to either charge or discharge")


def checkTestVariables(mode: str) -> Exception:
    if not os.path.exists(dataPath):
        return Exception("Data Path does not exist")
    if mode not in ["charge", "discharge"]:
        return Exception("Mode variable is not set to either charge or discharge")
    if powerSupplyChannel not in [1, 2]:
        return Exception("PSU Channel variable is not set to either 1 or 2")
    return None


def dischargeCharacterization(loadBank: LoadBank, logger: Logger, soc: float):
    dischargingRate = cRateAmps / 2   # 0.5C Rate in amps
    activeTimeSeconds = base1CActiveTime * cRateAmps / dischargingRate
    restingTimeSeconds = minRestingTimeTOC   # Resting time is set to resting time at top of charge
    deltaRestTime = (minRestingTimeTOC - minRestingTimeBOC) / (socTOC - firstPortionCutoffSoc) # Slope to linearly interpolate rest time (BoC need more rest time than ToC)
    while(soc > firstPortionCutoffSoc and loadBank.measure_voltage() > socVoltageEstimationMap[socBOC]):
        restingTimeSeconds = minRestingTimeTOC + (- deltaRestTime * (socTOC - soc)) # On a plot of rest time vs soc, we are tracing from the right to left, so we make the delta negative
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.enable_load()
        loadBank.set_current(dischargingRate)

        sleepWithStatusChecks(activeTimeSeconds, lambda: checkDischargeStatus(loadBank))
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
    dischargingRate = cRateAmps / 16 # 0.0625C rate for the cell
    activeTimeSeconds = base1CActiveTime * cRateAmps / dischargingRate  # Multiply the active time for the bottom of charge by sixteen and have one-sixteenth of charging rate
    restingTimeSeconds = 7200 # For BoC set resting time for three hours, in the interest of limited time, we limit to two hours

    print("Starting BoC Portion of Test")
    while(loadBank.measure_voltage() > socVoltageEstimationMap[socBOC]):
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.enable_load()
        loadBank.set_current(dischargingRate)

        sleepWithStatusChecks(activeTimeSeconds, lambda: checkDischargeStatus(loadBank))
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)
        loadBank.disable_load()

        time.sleep(restingTimeSeconds)
        soc = row[-1]

    loadBank.disable_load()
    row = getDischargingRow(logger, loadBank)
    logger.storeRow(row)

    # Gives an idea of if a settling voltage for the cell was reached
    for i in range(0, 9):
        time.sleep(300)
        row = getDischargingRow(logger, loadBank)
        logger.storeRow(row)


def chargeCharacterization(powerSupply: PowerSupply, logger: Logger, soc: float):
    # According to INR-18650-P30B datasheets, the cells capacity can range from standard of 2900 to 3000mAh
    # So a cutoff voltage is set to avoid overcharge and discharge

    chargingRate = cRateAmps / 16  # 0.0625C rate for the cell
    activeTimeSeconds = base1CActiveTime * cRateAmps / chargingRate
    restingTimeSeconds = 7200
    powerSupply.set_voltage(socVoltageEstimationMap[100], powerSupplyChannel) # Set voltage to 4.2V max according to datasheet to avoid overvoltage from high current at low soc
    while(soc < firstPortionCutoffSoc and powerSupply.measure_voltage(powerSupplyChannel) < socVoltageEstimationMap[100]):
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.set_current(chargingRate, powerSupplyChannel)
        powerSupply.enable_output(powerSupplyChannel)

        sleepWithStatusChecks(activeTimeSeconds, lambda: checkChargeStatus(powerSupply))
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.disable_output(powerSupplyChannel)

        time.sleep(restingTimeSeconds)
        soc = row[-1]

    chargingRate = cRateAmps / 2   # 0.5C Rate in amps
    activeTimeSeconds = base1CActiveTime * cRateAmps / chargingRate
    restingTimeSeconds = minRestingTimeBOC
    deltaRestTime = (minRestingTimeTOC - minRestingTimeBOC) / (socTOC - firstPortionCutoffSoc) # Slope to linearly interpolate rest time (BoC need more rest time than ToC)
    while(powerSupply.measure_voltage(powerSupplyChannel) < socVoltageEstimationMap[100] and soc < secondPortionCutoffSoc):
        restingTimeSeconds = minRestingTimeBOC + deltaRestTime * (soc - firstPortionCutoffSoc) # On a plot of rest time vs soc, we are tracing from the left to right, so we make the delta positive
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.set_current(chargingRate, powerSupplyChannel)
        powerSupply.enable_output(powerSupplyChannel)

        sleepWithStatusChecks(activeTimeSeconds, lambda: checkChargeStatus(powerSupply))
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.disable_output(powerSupplyChannel)
        time.sleep(restingTimeSeconds)
        soc = row[-1]

    chargingRate = cRateAmps / 16  # 0.0625C rate for the cell, trying to get close to constant voltage charging
    activeTimeSeconds = base1CActiveTime * cRateAmps / chargingRate
    restingTimeSeconds = 1200 # Since we are at TOC the cell settles faster, we just want to give the cell extra time to recover not go over 4.2V cutoff
    while(powerSupply.measure_voltage(powerSupplyChannel) < socVoltageEstimationMap[100]):
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.set_current(chargingRate, powerSupplyChannel)
        powerSupply.enable_output(powerSupplyChannel)

        sleepWithStatusChecks(activeTimeSeconds, lambda: checkChargeStatus(powerSupply))
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)
        powerSupply.disable_output(powerSupplyChannel)
        time.sleep(restingTimeSeconds)

    powerSupply.disable_output(powerSupplyChannel)
    row = getChargingRow(logger, powerSupply)
    logger.storeRow(row)

    # Gives an idea of if a settling voltage for the cell was reached
    for i in range(0, 9):
        time.sleep(300)
        row = getChargingRow(logger, powerSupply)
        logger.storeRow(row)


if __name__ == "__main__":
    main()