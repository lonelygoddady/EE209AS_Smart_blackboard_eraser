import serial 
import sys

ser = serial.Serial('/dev/ttyACM0')
print(ser.name)
