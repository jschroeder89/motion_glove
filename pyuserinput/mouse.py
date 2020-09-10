import serial
import json
import time

ser = serial.Serial('/dev/ttyUSB0', 115200)
data = []

#encode/decode utf8 .split
while True:
    x = ser.readline().decode('utf8').strip()
    break;
while True:
    x = ser.readline().decode('utf8').strip()
    j = json.loads(x)
    print(j)


    
