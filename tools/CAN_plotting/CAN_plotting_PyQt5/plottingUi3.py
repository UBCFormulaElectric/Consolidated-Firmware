from PyQt5 import QtCore, QtGui, QtWidgets
import pandas as pd
import plotly.graph_objects as go
import cantools as ct

class Ui_MainWindow(object):
    fileName = None
    data = None
    oneid = None
    fig = go.Figure()
    dbcFileName = None
    dbcData = None
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(541, 503)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.createGraphButton = QtWidgets.QPushButton(self.centralwidget)
        self.createGraphButton.setGeometry(QtCore.QRect(9, 427, 521, 31))
        self.createGraphButton.setObjectName("createGraphButton")
        self.browseLogFileButton = QtWidgets.QPushButton(self.centralwidget)
        self.browseLogFileButton.setGeometry(QtCore.QRect(110, 10, 91, 31))
        self.browseLogFileButton.setObjectName("browseLogFileButton")
        self.addCANSignalbutton = QtWidgets.QPushButton(self.centralwidget)
        self.addCANSignalbutton.setGeometry(QtCore.QRect(170, 380, 91, 31))
        self.addCANSignalbutton.setObjectName("addCANSignalbutton")
        self.signalSelectList = QtWidgets.QListWidget(self.centralwidget)
        self.signalSelectList.setGeometry(QtCore.QRect(10, 140, 251, 231))
        self.signalSelectList.setObjectName("signalSelectList")
        self.addedSignalList = QtWidgets.QListWidget(self.centralwidget)
        self.addedSignalList.setGeometry(QtCore.QRect(280, 140, 251, 231))
        self.addedSignalList.setObjectName("addedSignalList")
        self.deleteSignalButton = QtWidgets.QPushButton(self.centralwidget)
        self.deleteSignalButton.setGeometry(QtCore.QRect(280, 380, 91, 31))
        self.deleteSignalButton.setObjectName("deleteSignalButton")
        self.addedSignalLable = QtWidgets.QLabel(self.centralwidget)
        self.addedSignalLable.setGeometry(QtCore.QRect(280, 100, 251, 31))
        self.addedSignalLable.setAlignment(QtCore.Qt.AlignCenter)
        self.addedSignalLable.setObjectName("addedSignalLable")
        self.lineEditID = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEditID.setGeometry(QtCore.QRect(150, 110, 111, 21))
        self.lineEditID.setObjectName("lineEditID")
        self.IDSearchButton = QtWidgets.QPushButton(self.centralwidget)
        self.IDSearchButton.setGeometry(QtCore.QRect(10, 110, 131, 21))
        self.IDSearchButton.setObjectName("IDSearchButton")
        self.signalNameLable = QtWidgets.QLabel(self.centralwidget)
        self.signalNameLable.setGeometry(QtCore.QRect(10, 50, 251, 31))
        self.signalNameLable.setAlignment(QtCore.Qt.AlignCenter)
        self.signalNameLable.setObjectName("signalNameLable")
        self.browseDBCFileButton = QtWidgets.QPushButton(self.centralwidget)
        self.browseDBCFileButton.setGeometry(QtCore.QRect(10, 10, 91, 31))
        self.browseDBCFileButton.setObjectName("browseDBCFileButton")
        self.statusLabel = QtWidgets.QLabel(self.centralwidget)
        self.statusLabel.setGeometry(QtCore.QRect(280, 10, 251, 31))
        self.statusLabel.setObjectName("statusLabel")
        self.IDSearchButton_2 = QtWidgets.QPushButton(self.centralwidget)
        self.IDSearchButton_2.setGeometry(QtCore.QRect(10, 81, 251, 20))
        self.IDSearchButton_2.setObjectName("IDSearchButton_2")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 541, 21))
        self.menubar.setObjectName("menubar")
        self.menuCAN_graphing_tool = QtWidgets.QMenu(self.menubar)
        self.menuCAN_graphing_tool.setObjectName("menuCAN_graphing_tool")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.menubar.addAction(self.menuCAN_graphing_tool.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        self.browseLogFileButton.clicked.connect(self.getCANLog)
        self.createGraphButton.clicked.connect(self.startPlot)
        self.IDSearchButton.clicked.connect(self.searchSignals)
        self.addCANSignalbutton.clicked.connect(self.addSignalToList)
        self.deleteSignalButton.clicked.connect(self.deleteSignal)
        self.IDSearchButton_2.clicked.connect(self.showAllSignals)
        self.browseDBCFileButton.clicked.connect(self.getdbcFile)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.createGraphButton.setText(_translate("MainWindow", "Create Graph"))
        self.browseLogFileButton.setText(_translate("MainWindow", "Browse Log File"))
        self.addCANSignalbutton.setText(_translate("MainWindow", "Add CAN Signal"))
        self.deleteSignalButton.setText(_translate("MainWindow", "Delete Signal"))
        self.addedSignalLable.setText(_translate("MainWindow", "Added Signals"))
        self.IDSearchButton.setText(_translate("MainWindow", "Search CAN ID"))
        self.signalNameLable.setText(_translate("MainWindow", "Signal Name"))
        self.browseDBCFileButton.setText(_translate("MainWindow", "Browse DBC File"))
        self.statusLabel.setText(_translate("MainWindow", "STATUS :"))
        self.IDSearchButton_2.setText(_translate("MainWindow", "Show All Signals"))
        self.menuCAN_graphing_tool.setTitle(_translate("MainWindow", "CAN graphing tool"))

    def getCANLog(self):
        self.fileName, _ = QtWidgets.QFileDialog.getOpenFileName(None, "Select CAN log .txt file", "",)
        if not self.dbcFileName:
            return

        if not self.fileName:
            return

        self.parseRawLog()

    def getdbcFile(self):
        self.dbcFileName, _ = QtWidgets.QFileDialog.getOpenFileName(None, "Select .dbc file", "", )
        if not self.dbcFileName:
            return

        if not self.fileName:
            return

        self.parseRawLog()

    def parseRawLog(self):
        self.data = pd.read_csv(self.fileName, sep=';', header=16, quoting=3, error_bad_lines=False, encoding="utf-8")
        #self.data.Timestamp = pd.to_datetime(self.data.Timestamp, format='%Y%m%d%H%M%S%f')
        dbc = ct.database.load_file(self.dbcFileName)
        print(dbc)
        data['decoded'] = self.data.apply(lambda x: dbc.decode_message(x['ID'], x['Data']), axis=1)
        hmmm = self.data.apply(lambda x: dbc.decode_message(x['ID'], x['Data']), axis=1)
        print(hmmm)

    def startPlot(self):
        self.fig.data = []
        print(self.addedSignalList.count())
        for x in range(self.addedSignalList.count()):
            signal = self.addedSignalList.item(x)
            print(signal.text())
            onetrace = self.data.loc[lambda data: data['Name'] == signal.text()]
            self.fig.add_trace(go.Scatter(x=onetrace['Timestamp'], y=onetrace['Value(DEC)'],name=signal.text(),
                                          mode='lines+markers'))

        self.fig.show()

    def showAllSignals(self):
        self.signalSelectList.clear()

        for x in self.data.Name.unique():
            self.signalSelectList.addItem(x)

    def searchSignals(self):
        hexID = self.lineEditID.text()
        self.lineEditID.clear()
        self.signalSelectList.clear()
        if not hexID:
            return

        self.oneid = self.data.loc[self.data['ID(HEX)'] == int(hexID)]
        for x in self.oneid.Name.unique():
            self.signalSelectList.addItem(x)

    def addSignalToList(self):
        item = self.signalSelectList.currentItem()
        if not item:
            return

        self.addedSignalList.addItem(item.text())

    def deleteSignal(self):
        print(self.addedSignalList.currentRow())
        self.addedSignalList.takeItem(self.addedSignalList.currentRow())

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
