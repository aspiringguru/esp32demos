#https://github.com/pybluez/pybluez

import bluetooth

nearby_devices = bluetooth.discover_devices(lookup_names=True)
print("found %d devices" % len(nearby_devices))

for addr, name in nearby_devices:
    print("  %s - %s" % (addr, name))
    #addr = MAC address, name = device assigned name


import os
import sys
import struct
bdaddr = bluetooth.read_local_bdaddr()
print(bdaddr)
#this will match with bluetooth mac from : cmd > ipconfig /all

#-------------------------------------------------------------------------------
#this is from http://blog.kevindoran.co/bluetooth-programming-with-python-3/
#needs server running on the ESP32 to respond?
import bluetooth

serverMACAddress = '30:AE:A4:21:D2:AA'
port = 5
s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
s.connect((serverMACAddress, port))
while 1:
    text = raw_input() # Note change to the old (Python 2) raw_input
    if text == "quit":
    break
    s.send(text)
sock.close()


#-------------------------------------------------------------------------------
import socket

serverMACAddress = '30:AE:A4:21:D2:AA'
port = 5
#check windows device manager > get port used by driver
s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
#does not work on windows? python does not have sockets on windows?
s.connect((serverMACAddress,port))
while 1:
    text = input()
    if text == "quit":
        break
    s.send(bytes(text, 'UTF-8'))
s.close()
