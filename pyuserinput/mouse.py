import serial
import json
import time
import os 
import time
import numpy as np

ser = serial.Serial('/dev/ttyUSB0', 115200)


#encode/decode utf8 .split
while True:
    x = ser.readline().decode('utf8').strip()
    break;
while True:
    x = ser.readline().decode('utf8').strip()
    j = json.loads(x)
    if j[0] == "BNO055_0x28":
        arr = np.array(j[1])
        print(arr)
    
