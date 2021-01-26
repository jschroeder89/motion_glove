#!/usr/bin/python
# chmod +x mouse.py first
# usage -> ./mouse.py  
import json
import serial
import numpy as np
from pymouse import PyMouse


class Mouse(object):
    """
    mouse controlls via bno055 
    """    
    m = PyMouse()
    x_dim, y_dim = m.screen_size()
    y_pos = int(x_dim/2)
    x_pos = int(y_dim/2)
    sensitivity = 15000
    def __init__(self):
        """
        set initial position
        """
        self.m.move(int(self.y_pos), int(self.x_pos))

    def get_serial_data(self, ser):
        """ 
        get json string from serial bus, strip junk 
        """
        ser_data = ser.readline().decode('utf8').strip()
        ser_data = ser.readline().decode('utf8').strip()
        j = json.loads(ser_data)
        if j[0] == ('INDEX' or 'MIDDLE'):
            data = j[0]
        else:
            data = np.array(j[1])
        return data

    def format_data(self, data):
        """
        format acc_lin_data
        """
        self.pos_data = data/100
        for i in range(len(data)):
            if self.pos_data[i] > 655/2:
                self.pos_data[i] = self.pos_data[i] - 655

    def get_position(self):
        self.x_pos = self.x_pos + ((self.pos_data[0]) * (0.5 * 0.03 * 0.03) * self.sensitivity)
        self.y_pos = self.y_pos + ((self.pos_data[1]) * (0.5 * 0.03 * 0.03) * self.sensitivity)

    def move_mouse_to(self):
        self.m.move(int(self.y_pos), int(self.x_pos))

    def main_loop(self):
        """
        main loop setting the mouse coordinates 
        """
        ser = serial.Serial('/dev/ttyUSB0', 115200)
        while True:
            data = self.get_serial_data(ser)   

            if isinstance(data, np.ndarray):
                self.format_data(data)
                self.get_position()
                self.move_mouse_to()
                print(self.x_pos, self.y_pos)

mouse = Mouse()   
mouse.main_loop() 
