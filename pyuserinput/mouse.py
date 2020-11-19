import json
import serial
import time
import time
import numpy as np

ser = serial.Serial('/dev/ttyUSB0', 115200)
x_Pos = 0
y_Pos = 0
sampleRate = 0 #TBD

def getPosition(acc_data):
    x_Pos = x_Pos + acc_data[0] * (0.5 * sampleRate * sampleRate)
    y_Pos = y_Pos + acc_data[1] * (0.5 * sampleRate * sampleRate)
    print("x_Pos: ", x_Pos, "y_Pos: ", y_Pos)
    return acc_data

#encode/decode utf8 .split
while True:
    x = ser.readline().decode('utf8').strip()
    break
while True:
    x = ser.readline().decode('utf8').strip()
    j = json.loads(x)
    if j[0] == "BNO055_0x28":
        BNO055_arr = getPosition(np.array(j[1]))
    
    
