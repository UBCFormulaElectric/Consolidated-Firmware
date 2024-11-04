import os
import time
from datetime import datetime
import csv
import threading
# from loadBank._loadBank import *
# from powerSupply._powerSupply import *

class Logger:
    def __init__(self, logFolder: str, columns, newFile= True) -> None:
        self.columns = ["Timestamp", "Elapsed Time [s]", "Time Increment [s]"] + columns
        self.logFileName = datetime.now().strftime("log_%Y-%m-%d_%H-%M-%S.csv")
        self.logFilePath = os.path.join(logFolder, self.logFileName)
        self.logStop = False
        if newFile:
            with open(self.logFilePath, mode= 'w', newline= '') as csvFile:
                writer = csv.writer(csvFile, delimiter= ',', quotechar= '|', quoting= csv.QUOTE_MINIMAL)
                writer.writerow(self.columns)
    
    # def startLogging(self):
    #     with open(self.logFilePath, mode= 'w', newline= '') as csvFile:
    #         writer = csv.writer(csvFile, delimiter= ',', quotechar= '|', quoting= csv.QUOTE_MINIMAL)
    #         writer.writerow(self.columns)
        # thread = threading.Thread(target= self.logLoop)
        # thread.start()
    
    # def logLoop(self):
    #     self.startTime = time.time()
    #     self.logStop = False
    #     while(not self.logStop):
    #         with open(self.logFilePath, mode= 'a', newline= '') as csvFile:
    #             writer = csv.writer(csvFile, delimiter= ',', quotechar= '|', quoting= csv.QUOTE_MINIMAL)
    #             row = [str(func()) for func in self.columnFuncself.columnFuncInputs]
    #             self.lastRow = row
                
    #             print(row)
    #             writer.writerow(row)
    #             csvFile.flush()
    #         time.sleep(1)

    # def stopLogging(self):
    #     self.logStop = True
    #     return


    def storeRow(self, row):
        if self.startTime is None:
            self.startTime = time.time()
        self.lastRowTime = time.time()
        with open(self.logFilePath, mode= 'a', newline= '') as csvFile:
                writer = csv.writer(csvFile, delimiter= ',', quotechar= '|', quoting= csv.QUOTE_MINIMAL)
                
                writer.writerow([self.getTimestamp(), self.getElapsedTime()] + row)
                
    def getLastRow(self):
        return self.lastRow
    
    def getTimestamp(self):
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
    
    def getElapsedTime(self):
        if self.startTime is None:
            return "Logging has not started yet."
        # Calculate elapsed time in milliseconds
        elapsed_time = (time.time() - self.startTime) 
        return round(elapsed_time, 3)   
    
    def getTimeIncrement(self):
        if self.startTime is None:
            return "Logging has not started yet."
        elif self.getLastRow() is None:
            return None
        else: 
        # Calculate elapsed time in milliseconds
            timeIncrement = (time.time() - self.lastRowTime) 
        return round(timeIncrement, 3)   
    


# TESTING #  
def get1():
    return 1

def get2():
    return 2



    
def main():
    file = r'C:\Users\lkevi\OneDrive\Desktop\Coding'
    funcDict = {"One": get1, "Two": get2}

    logger = Logger(file, funcDict)

    logger.startLogging()

if __name__ == "__main__":
    main()