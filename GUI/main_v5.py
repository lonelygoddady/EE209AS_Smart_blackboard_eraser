import sys
import Project_GUI_v2
from PyQt4 import QtGui, QtCore

x1, y1, x2, y2 = 0, 0, 0, 0
press_num = 0
sig = 0

class myApp(QtGui.QMainWindow, Project_GUI_v2.Ui_MainWindow):

    def __init__(self, parent=None):
        global pix
        super(myApp, self).__init__(parent)
        self.setupUi(self)
        self.label_area.installEventFilter(self)
        self.Save.clicked.connect(self.save_content)
        self.Cancel.clicked.connect(self.cancel_select)
        self.Update.clicked.connect(self.update_content)
        self.Clear.clicked.connect(self.clear)

    def paintEvent(self,pixm):
        global press_num, pix, size
        if (press_num == 2):
            pix = QtGui.QPixmap("sample.png")
            window_s = self.label_area.size()
            scale_pix = pix.scaled(size,QtCore.Qt.IgnoreAspectRatio,QtCore.Qt.SmoothTransformation) 
            painter = QtGui.QPainter(scale_pix)
            painter.setBrush(QtCore.Qt.NoBrush)
            painter.setPen(QtCore.Qt.red)
            qrect = QtCore.QRect(QtCore.QPoint(x1,y1),QtCore.QPoint(x2,y2))
            painter.drawRect(qrect) 
            painter.end()
            self.label_area.setPixmap(scale_pix)
            self.label_area.setFixedSize(window_s)

    def eventFilter(self, source, event): 
        global x1, x2, y1, y2, press_num, size 
        if(event.type() == QtCore.QEvent.MouseButtonPress and source is self.label_area): 
             if (press_num <= 2):
                press_num += 1
                if (press_num == 1):
                    x1 = event.pos().x()
                    y1 = event.pos().y()
                    self.textBrowser.setText(str(x1)+","+str(y1)+"\n")
                elif (press_num == 2):
                    x2 = event.pos().x()
                    y2 = event.pos().y()
                    self.textBrowser.append(str(x2)+","+str(y2)) 
                    size = self.label_area.size()
                    self.paintEvent(self)
                    self.update()
             else:
                self.textBrowser.append("Area already selected!\n")
        self.update()
        return QtGui.QWidget.eventFilter(self, source, event)

    def save_content(self):
        self.textBrowser.setText("Content Saving...\n");
        #code to save content

    def cancel_select(self):
        global x1, x2, y1, y2, press_num
        self.textBrowser.setText("Cancel selected area.\n")
        x1, x2, y1, y2, press_num, sig = 0, 0, 0, 0, 0, 0
        self.update_content()
        ##code to cancel selected area

    def update_content(self):
        global pix 
        self.textBrowser.setText("Update blackboard content.\n")
        window_s = self.label_area.size() 
        pix = QtGui.QPixmap('sample.png')
        scale_pix = pix.scaled(self.label_area.size(),QtCore.Qt.IgnoreAspectRatio,QtCore.Qt.SmoothTransformation)       
        self.label_area.setPixmap(scale_pix)
        self.label_area.setFixedSize(window_s)
        ##code to update content 

    def clear(self):
        global x1, x2, y1, y2, press_num, sig 
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
