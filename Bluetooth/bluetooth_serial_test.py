#! /usr/bin/python

import socket
bd_addr = "43:34:1B:00:1F:AC"
port = 3
s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
s.connect((bd_addr,port))
while 1: 
    tosend = input()
    if tosend != 'q':
        s.send(bytes(tosend,'UTF-8'))
    else: 
        break

s.close()
