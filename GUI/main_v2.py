import sys
import Project_GUI_v2
from PyQt4 import QtGui, QtCore

x1, y1, x2, y2 = 0, 0, 0, 0
press_num = 0

class myApp(QtGui.QMainWindow, Project_GUI_v2.Ui_MainWindow):

    def __init__(self, parent=None):
        super(myApp, self).__init__(parent)
        self.setupUi(self)
        self.label_area.installEventFilter(self)
        self.Save.clicked.connect(self.save_content)
        self.Cancel.clicked.connect(self.cancel_select)
        self.Update.clicked.connect(self.update_content)
        self.Clear.clicked.connect(self.clear)

    def paintEvent(self, event):
        global x1, x2, y1, y2
        painter = QtGui.QPainter(self)
        painter.setBrush(QtCore.Qt.NoBrush)
        painter.setPen(QtCore.Qt.red)
        x1 = x1 + 5
        x2 = x2 + 5
        y1 = y1 + 5
        y2 = y2 + 5
        qrect = QtCore.QRect(QtCore.QPoint(x1,y1),QtCore.QPoint(x2,y2))
        painter.drawRect(qrect)

        painter.end()

    def eventFilter(self, source, event): 
        global x1, x2, y1, y2, press_num
        if(event.type() == QtCore.QEvent.MouseButtonPress and source is self.label_area):
            if (press_num < 2):
                press_num += 1
                if (press_num == 1):
                    x1 = event.pos().x()
                    y1 = event.pos().y()
                    self.textBrowser.setText(str(x1)+","+str(y1)+"\n")
                elif (press_num == 2):
                    x2 = event.pos().x()
                    y2 = event.pos().y()
                    self.textBrowser.append(str(x2)+","+str(y2))
                    self.paintEvent(self.label_area)
                    self.update()
            else:
                self.textBrowser.append("Area already selected!\n")
   
        return QtGui.QWidget.eventFilter(self, source, event)

    def save_content(self):
        self.textBrowser.setText("Content Saving...\n");
        ##code to save content

    def cancel_select(self):
        global x1, x2, y1, y2, press_num
        self.textBrowser.setText("Cancel selected area.\n")
        x1, x2, y1, y2, press_num = 0, 0, 0, 0, 0
        self.update()
        ##code to cancel selected area

    def update_content(self):
        self.textBrowser.setText("Update blackboard content.\n")
        ##code to update content 

    def clear(self):
        global x1, x2, y1, y2, press_num 
        if (press_num == 2):
            self.textBrowser.setText("Perform board erasing...\n");
            #send out two points coordinate data
            self.cancel_select() 
        else:
            self.textBrowser.setText("Need to specify erasing area!\n")
        ##code to update cleaning instruction

def main():
    app = QtGui.QApplication(sys.argv)
    thisApp = myApp()
    thisApp.show()
    #sys.exit(app.exec_())
    app.exec_()

if __name__ == "__main__":
    main()
