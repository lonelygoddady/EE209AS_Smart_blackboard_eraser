import sys

import Project_GUI_v1
from PyQt4 import QtCore, QtGui

a1, a2, a3, a4 = 0, 0, 0, 0

class myApp(QtGui.QMainWindow, Project_GUI_v1.Ui_MainWindow):
    def __init__(self, parent=None):
        super(myApp, self).__init__(parent)
        self.setupUi(self)
        self.area1_pb.clicked.connect(self.area1)
        self.area2_pb.clicked.connect(self.area2)
        self.area3_pb.clicked.connect(self.area3)
        self.area4_pb.clicked.connect(self.area4)
        self.Save.clicked.connect(self.save_content)
        self.Cancel.clicked.connect(self.cancel_select)
        self.Update.clicked.connect(self.update_content)
        self.Clear.clicked.connect(self.clear)
    

    def area1(self):
        global a1
            
        if a1 != 1:
            self.textBrowser.append("Area 1 (top left) selected!\n")
            a1 = 1
        else:
            self.textBrowser.append("Area 1 (top left) already selected!\n")
                

    def area2(self):
        global a2
            
        if a2 != 1:
            self.textBrowser.append("Area 2 (top left) selected!\n")        
            a2 = 1
        else:
            self.textBrowser.append("Area 2 (top left) already selected!\n")

    def area3(self):
        global a3
            
        if a3 != 1:
            self.textBrowser.append("Area 3 (top left) selected!\n")        
            a3 = 1
        else:
            self.textBrowser.append("Area 3 (top left) already selected!\n")


    def area4(self):
        global a4
            
        if a4 != 1:
            self.textBrowser.append("Area 4 (bot right) selected!\n")       
            a4 = 1
        else:
            self.textBrowser.append("Area 4 (bot right) already selected!\n")


    def save_content(self):
        self.textBrowser.setText("Content Saving...\n");
        ##code to save content

    def cancel_select(self):
        global a1,a2,a3,a4
        self.textBrowser.setText("Cancel selected area.\n")
        a1, a2, a3, a4 = 0, 0, 0, 0
        ##code to cancel selected area

    def update_content(self):
        self.textBrowser.setText("Update blackboard content.\n")
        ##code to update content 

    def clear(self):
        self.textBrowser.setText("Perform board erasing...\n");
        ##code to update cleaning instruction
        a1, a2, a3, a4 = 0, 0, 0, 0


def main():
    app = QtGui.QApplication(sys.argv)
    thisApp = myApp()
    thisApp.show()
    #sys.exit(app.exec_())
    app.exec_()

if __name__ == "__main__":
    main()
