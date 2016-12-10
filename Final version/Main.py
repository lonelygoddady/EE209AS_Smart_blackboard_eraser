import sys
import threading
import serial
import time
import Project_GUI_v2
from PyQt4 import QtGui, QtCore

import cv2
import numpy as np
from src.transform import four_point_transform
from src import imutils
from skimage.filters import threshold_adaptive
import argparse

x1, y1, x2, y2 = 0, 0, 0, 0
press_num = 0
sig = 0
#global GlobalscreenCnt = np.array([(0,0),(0,0),(0,0),(0,0)], dtype = 'float32')

class myApp(QtGui.QMainWindow, Project_GUI_v2.Ui_MainWindow):

    def __init__(self, parent=None):
        global pix
        super(myApp, self).__init__(parent)
        self.setupUi(self)
        self.label_area.installEventFilter(self)
        self.Save.clicked.connect(self.save_content)
        self.Cancel.clicked.connect(self.cancel_select)
        self.Update.clicked.connect(self.update_content)
        self.Calibrate.clicked.connect(self.calibrate_content)
        self.Clear.clicked.connect(self.clear)
  
    def calibrate_content(self,event):
        #calibrate board projected content
        self.textBrowser.setText("Board content calibrating...\n")
        calibrate_image()
        self.textBrowser.setText("Update blackboard content.\n")
        window_s = self.label_area.size() 
        pix = QtGui.QPixmap('Display.png')
        scale_pix = pix.scaled(self.label_area.size(),QtCore.Qt.IgnoreAspectRatio,QtCore.Qt.SmoothTransformation)       
        self.label_area.setPixmap(scale_pix)
        self.label_area.setFixedSize(window_s)
        

    def Data_trans(self,event): 
        global x1, y1, x2, y2
        ser = serial.Serial('/dev/ttyACM0', 9600)
        s = str(x1) + ',' + str(y1) + ',' + str(x2) + ',' + str(y2) + ','
        self.textBrowser.setText(ser.name)
        if ser.is_open == False:
            ser.open()
            self.textBrowser.append("Gate is open\n")
        else: 
            ser.write(s)
            self.textBrowser.append(s+'\n')
            ser.close()

    def paintEvent(self,pixm):
        global press_num, pix, size
        if (press_num == 2):
            pix = QtGui.QPixmap("Display.png")
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

    def cancel_select(self,event):
        global x1, x2, y1, y2, press_num
        self.textBrowser.setText("Cancel selected area.\n")
        x1, x2, y1, y2, press_num, sig = 0, 0, 0, 0, 0, 0
        self.update_content()
        ##code to cancel selected area

    def update_content(self):
        global pix
        capture_image()
        self.textBrowser.setText("Update blackboard content.\n")
        window_s = self.label_area.size() 
        pix = QtGui.QPixmap('Display.png')
        scale_pix = pix.scaled(self.label_area.size(),QtCore.Qt.IgnoreAspectRatio,QtCore.Qt.SmoothTransformation)       
        self.label_area.setPixmap(scale_pix)
        self.label_area.setFixedSize(window_s)
        ##code to update content 

    def clear(self):
        global press_num, x1, y1, x2, y2
        if (press_num == 2):
            self.textBrowser.setText("Perform board erasing...\n");
            #send out two points coordinate data
            self.Data_trans(self)
            self.cancel_select(self) 
        else:
            self.textBrowser.setText("Need to specify erasing area!\n")
        ##code to update cleaning instruction




        
def main():
    app = QtGui.QApplication(sys.argv)
    thisApp = myApp()
    thisApp.show()
    #sys.exit(app.exec_())
    app.exec_()

def capture_image():
    cap = cv2.VideoCapture(0)
    cap.set(3,1920)
    cap.set(4,1080)
    ret,image = cap.read()
    if ret:
        try:
            GlobalscreenCnt
        except NameError:
            print("Warning: Did not find the screenCnt, recalibrating...")
            cap.release()
            calibrate_image()
        else:
            ratio = image.shape[0] / 500.0
            warped = four_point_transform(image, GlobalscreenCnt.reshape(4, 2) * ratio)
            cv2.imwrite("Display.png", cv2.resize(warped, (500, 500)))
            cap.release()
        

def calibrate_image():
    cap = cv2.VideoCapture(0)
    cap.set(3,1920)
    cap.set(4,1080)
    ret,image = cap.read()
    if ret:
        #print("Calibrating...")
        ratio = image.shape[0] / 500.0
        original = image.copy()
        image = imutils.resize(image, height = 500)

        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (5, 5), 0)
        edged = cv2.Canny(gray, 75, 200)

        (cnts, _) = cv2.findContours(edged, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        cnts = sorted(cnts, key = cv2.contourArea, reverse = True)[:5]

        for c in cnts:
            peri = cv2.arcLength(c, True)
            approx = cv2.approxPolyDP(c, 0.02 * peri, True)
            if len(approx) == 4:
                    screenCnt = approx
                    break

        try:
            screenCnt
        except NameError:
            cv2.imwrite("Display.png", edged)

        else:
            global GlobalscreenCnt
            GlobalscreenCnt = screenCnt
            warped = four_point_transform(original, screenCnt.reshape(4, 2) * ratio)
            cv2.imwrite("Display.png", cv2.resize(warped, (500, 500)))
            del screenCnt
            
    cap.release()



if __name__ == "__main__":
    main()
