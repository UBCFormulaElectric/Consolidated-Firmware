# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:\Users\wesle\Desktop\CAN_logger.ui'
#
# Created by: PyQt5 UI code generator 5.13.0
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets
import pandas as pd
import plotly.graph_objects as go


class Ui_MainWindow(object):
    fileName = None
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(321, 182)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(30, 40, 101, 31))
        self.pushButton.setObjectName("pushButton")
        self.pushButton_2 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_2.setGeometry(QtCore.QRect(30, 100, 261, 31))
        self.pushButton_2.setObjectName("pushButton_2")
        self.lineEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit.setGeometry(QtCore.QRect(172, 50, 121, 21))
        self.lineEdit.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.lineEdit.setObjectName("lineEdit")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(170, 30, 121, 21))
        self.label.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.label.setFrameShape(QtWidgets.QFrame.NoFrame)
        self.label.setTextFormat(QtCore.Qt.AutoText)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName("label")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 321, 21))
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

        self.pushButton.clicked.connect(self.getCANLog)
        self.pushButton_2.clicked.connect(self.startPlot)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.pushButton.setText(_translate("MainWindow", "Browse log file"))
        self.pushButton_2.setText(_translate("MainWindow", "Create Graph"))
        self.label.setText(_translate("MainWindow", "CAN ID to Graph (HEX) "))
        self.menuCAN_graphing_tool.setTitle(_translate("MainWindow", "CAN plotting tool"))

    def getCANLog(self):
        self.fileName, _ = QtWidgets.QFileDialog.getOpenFileName(None, "Select CAN log .txt file", "",)


    def startPlot(self):
        hexID = self.lineEdit.text()
        self.lineEdit.clear()
        print(self.fileName)
        print(type(self.fileName))
        data = pd.read_csv(self.fileName, sep=';', header=16, quoting=3, error_bad_lines=False)
        oneid = data.loc[data['ID'] == hexID]
        oneid.Data = oneid.Data.apply(lambda x: int(x, 16))
        oneid.Timestamp = pd.to_datetime(oneid.Timestamp, format='%Y%m%d%H%M%S%f')
        fig = go.Figure([go.Scatter(x=oneid['Timestamp'], y=oneid['Data'])])
        fig.show()
        print(data)
        print(data.columns)
        print(oneid)
        print(oneid.Timestamp)


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
