import os
import time
from datetime import datetime
import csv



class Logger:
    def __init__(self, logFolder: str, columns) -> None:
        self.columns = ["Index", "Timestamp", "Elapsed Time [s]", "Time Increment [s]"] + columns
        self.logFileName = datetime.now().strftime("log_%Y-%m-%d_%H-%M-%S.csv")
        self.logFilePath = os.path.join(logFolder, self.logFileName)
        self.startTime = None
        self.lastRow = None
        self.lastRowTime = None

        with open(self.logFilePath, mode= 'w', newline= '') as csvFile:
            writer = csv.writer(csvFile, delimiter= ',', quotechar= '|', quoting= csv.QUOTE_MINIMAL)
            writer.writerow(self.columns)

    def getIndex(self)->int:
        if self.lastRow is None:
            return 0
        else:
            return self.lastRow["Index"] + 1


    def storeRow(self, row)->None:
        if self.startTime is None:
            self.startTime = time.time()
        
        with open(self.logFilePath, mode= 'a', newline= '') as csvFile:
                writer = csv.writer(csvFile, delimiter= ',', quotechar= '|', quoting= csv.QUOTE_MINIMAL)
                row = [self.getIndex(), self.getTimestamp(), self.getElapsedTime(), self.getTimeIncrement()] + row
                writer.writerow(row)

                self.lastRow = dict(zip(self.columns, row))

                max_key_length = max(len(key) for key in self.lastRow.keys())
                for key, value in self.lastRow.items():
                    print(f"{key.ljust(max_key_length)} : {value}")

        self.lastRowTime = time.time()
                
    def getLastRow(self)->dict:
        """
        Returns a map with the column names as keys and the last column values as values.

        INPUTS:  None
        OUTPUTS: Map with the column names as keys and the last column values as values.
        """
        return self.lastRow
    
    def getTimestamp(self)->datetime:
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
    
    def getElapsedTime(self)->float:
        if self.startTime is None:
            return "Logging has not started yet."
        # Calculate elapsed time in milliseconds
        elapsed_time = (time.time() - self.startTime) 
        return round(elapsed_time, 3)   
    
    def getTimeIncrement(self)->float:
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
    file = r'Your\file\path'
    funcList = ["One", "Two", "Three Float"]

    logger = Logger(file, funcList)
    while(1):

        logger.storeRow([1, 2, 3.4])
        time.sleep(0.5)

if __name__ == "__main__":
    main()