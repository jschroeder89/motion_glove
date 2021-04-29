#!/usr/bin/python
# chmod +x mouse.py first
# usage -> ./mouse.py
import json
import serial
import numpy as np
from pymouse import PyMouse
from pykeyboard import PyKeyboard
import math



class Mouse(object):
    """
    mouse controlls via bno055 
    """
    m = PyMouse()
    k = PyKeyboard()
    x_dim, y_dim = m.screen_size()
    y_pos = int(x_dim/2)
    x_pos = int(y_dim/2)
    x_pos_prev = int(y_dim/2)
    y_pos_prev = int(x_dim/2)
    q_w, q_x, q_y, q_z = 0, 0, 0, 0
    roll, pitch, heading = 0, 0, 0
    heading_offset = 0
    heading_upper_threshold = 8.0
    heading_lower_threshold = -8.0
    def __init__(self):
        """
        set initial position
        """
        # self.m.move(int(self.y_pos), int(self.x_pos))

    def get_serial_data(self, ser):
        """ 
        get json string from serial bus, strip junk 
        """
        data = []
        ser_data = ser.readline().decode('utf8').strip()
        ser_data = ser.readline().decode('utf8').strip()
        s = ser_data.split("][")
        index = s[0] + ']'
        main = '[' + s[1]
        index_data = json.loads(index)
        main_data = json.loads(main)
        data.append(np.array(index_data[1]))
        data.append(np.array(main_data[1]))
        return data

    def format_data_index(self, data):
        """
        format raw accelerator data into +-2g range
        """
        self.acc_data = data/16384
        for i in range(len(data)):
            if self.acc_data[i] > 3.99994/2:
                self.acc_data[i] = self.acc_data[i] - 3.99994

    def format_data_main(self, data):
        """
        format raw data
        """
        self.orientation_data = data/16
        #for i in range(len(self.orientation_data)):
            #if self.orientation_data[i] > 4000:
                #self.orientation_data[i] = self.orientation_data[i] - 4095
        self.heading = self.orientation_data[0]  
        self.pitch = self.orientation_data[1] 
        self.roll = self.orientation_data[2] 
        if self.pitch >= 3916:
            self.pitch = self.pitch - 4095 
        self.pitch = self.pitch * (-1)
        if self.roll >= 3916:
            self.roll = self.roll - 4095
        self.heading = self.heading - self.heading_offset
        if self.heading >= 180:
            self.heading = (360 - self.heading) * - 1
    
    def get_sensor_data(self, ser):
        data = self.get_serial_data(ser)
        self.format_data_index(data[0])
        self.format_data_main(data[1])
        
    def get_heading_offset(self, ser):
        data = self.get_serial_data(ser)
        self.format_data_main(data[1])
        self.heading_offset = self.heading
        
    def map_keys(self, ser):
        if self.heading >= self.heading_upper_threshold:
            self.k.press_key('d')
            while self.heading >= self.heading_upper_threshold:
                self.get_sensor_data(ser)
                if self.pitch >= 30:
                    self.k.press_key('w')
                    while self.pitch >= 30:
                        self.get_sensor_data(ser)
                    self.k.release_key('w')
            self.k.release_key('d')
        elif self.heading <= self.heading_lower_threshold:
            self.k.press_key('a')
            while self.heading <= self.heading_lower_threshold:
                self.get_sensor_data(ser)
                if self.pitch >= 30:
                    self.k.press_key('w')
                    while self.pitch >= 30:
                        self.get_sensor_data(ser)
                self.k.release_key('w')
            self.k.release_key('a')
        elif self.heading <= self.heading_upper_threshold and self.heading >= self.heading_lower_threshold:
            if self.pitch >= 30:
                self.k.press_key('w')
                while self.pitch >= 30:
                    self.get_sensor_data(ser)
            self.k.release_key('w')
        


    def get_next_position(self):
        self.x_pos = self.x_pos + (2.5 * self.pitch)
        self.y_pos = self.y_pos + (1.5 * self.heading)

    def move_mouse(self):
        self.m.move(int(self.y_pos), int(self.x_pos))

    def move_mouse_smooth(self):
        x = int(self.x_pos - self.x_pos_prev)
        y = int(self.y_pos - self.y_pos_prev)
        m = max(abs(x), abs(y))
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

        self.x_pos_prev = self.x_pos
        self.y_pos_prev = self.y_pos

    def main_loop(self):
        """
        main loop setting the mouse coordinates 
        """
        ser = serial.Serial('COM3', 115200) #Windows
        #ser = serial.Serial('/dev/ttyUSB0', 115200) #Linux
        self.get_heading_offset(ser)
        while True:
            self.get_sensor_data(ser)
            self.map_keys(ser)
            # self.get_next_position()
            # self.move_mouse_smooth()
            print(self.heading)
            
                
                
                
mouse = Mouse()
mouse.main_loop()
