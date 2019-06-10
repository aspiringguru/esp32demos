#https://youtu.be/AKiTbDsckU0
#https://www.hackster.io/Jalal_Mansoori/python3-and-arduino-communication-c33192

import serial
import time

arduino = serial.Serial('COM6', 115200)
#check device manager > ports > Silicon Labs CP210x USB to UART Bridge for port #
#right click > properties > port settings  check bits per second is set to 115200
time.sleep(2)

print ("enter 1 to turn ON LED and 0 to turn OFF LED")

while 1:
    dataFromUserInput = input()
    if dataFromUserInput == '1':
        #arduino.write('1')
        arduino.write(str.encode('1'))
        print("LED is turned ON")
    elif dataFromUserInput == '0':
        #arduino.write('0')
        arduino.write(str.encode('0'))
        print("LED is turned OFF")
