import json
import serial
import time
import time
import numpy as np

ser = serial.Serial('/dev/ttyUSB0', 115200)


#encode/decode utf8 .split
while True:
    x = ser.readline().decode('utf8').strip()
    break
while True:
    x = ser.readline().decode('utf8').strip()
    y = ser.readline().decode('utf8').strip()
    
    j1 = json.loads(x)
    j2 = json.loads(y)   
    
    """if j1[0] == "BNO055_0x28" and j2[0] == "BMI160":
        BNO055_arr = np.array(j1[1])
        BMI160_arr = np.array(j2[1])
    else:
        BNO055_arr = np.array(j2[1])
        BMI160_arr = np.array(j1[1])    
    """
