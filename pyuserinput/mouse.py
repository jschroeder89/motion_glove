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
    x_pos_prev = int(y_dim/2)
    y_pos_prev = int(x_dim/2)
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
        self.x_pos = int(self.x_pos + ((self.pos_data[0]) * (0.5 * 0.03 * 0.03) * self.sensitivity))
        self.y_pos = int(self.y_pos + ((self.pos_data[1]*-1) * (0.5 * 0.03 * 0.03) * self.sensitivity))

    def move_mouse(self):
        self.m.move(int(self.y_pos), int(self.x_pos))

    def move_mouse_smooth(self):
        x = int(self.x_pos - self.x_pos_prev)
        y = int(self.y_pos - self.y_pos_prev)
        m = max(abs(x), abs(y))
        if m < 10:
            return
        for i in range(1, m):
            self.x_pos = int(self.x_pos_prev + i/m*x)
            self.y_pos = int(self.y_pos_prev + i/m*y)
            if self.x_pos > self.y_dim:
                self.x_pos = 0
            if self.x_pos < 0:
                self.x_pos = self.y_dim
            if self.y_pos > self.x_dim:
                self.y_pos = 0
            if self.y_pos < 0:
                self.y_pos = self.x_dim
            self.move_mouse()

        """ if x > 0:
            for i in range(self.x_pos, self.x_pos_prev):
                xlist.append(i)
            xlist.reverse()
        else:
            for i in range(self.x_pos_prev, self.x_pos):
                xlist.append(i)
        if y > 0:
            for i in range(self.y_pos, self.y_pos_prev):
                ylist.append(i)
            ylist.reverse()
        else:
            for i in range(self.y_pos_prev, self.y_pos):
                ylist.append(i) """


        self.x_pos_prev = self.x_pos
        self.y_pos_prev = self.y_pos

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
                #self.move_mouse()
                self.move_mouse_smooth()
                #print(self.x_pos, self.y_pos)

mouse = Mouse()
mouse.main_loop()
