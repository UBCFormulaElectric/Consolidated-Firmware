from loadBank._loadBank import *
from powerSupply._powerSupply import *
from logger.logger import *

def main()->None:
    loadbank = LoadBank()
    powersupply = PowerSupply()
    loggingDict = \
    {
        "Voltage [V]" : powersupply.measure_voltage,
        "Current [A]" : powersupply.measure_current
    }

    logger = Logger() 




if __name__ == "main":
    main()