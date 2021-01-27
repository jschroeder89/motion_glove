#!/usr/bin/python
# chmod +x plotter.py first
# usage -> ./plotter.py 'target i.e. bmi160, bno055' 'sensor i.e. acc, gyro'
import json
import serial
import time
import sys
import numpy as np
import matplotlib.pyplot as plt



class Plotter(object):
    """
    Plotter class for plotting the accelerator and/or the gyroscope 
    data from the BMI160 or BNO055.  
    """
    click_times = 0
    x_pos = 0
    y_pos = 0
    def __init__(self, args):
        """
        Read argument list and search for keywords
        """
        if 'bmi160' in args or 'BMI160' in args:
            self.sensor = 'BMI160'

        if 'bno055' in args or 'BNO055' in args:
            self.sensor = 'BNO055'

        if ('acc' or 'ACC') in args:
            self.acc = True
        else:
            self.acc = False

        if ('gyro' or 'GYRO') in args:
            self.gyro = True
        else:
            self.gyro = False

        if ('pos' or 'POS') in args:
            self.acc_lin = True
        else:
            self.acc_lin = False


    def plot_loop(self):
        """
        plot live data
        """
        plt.style.use('ggplot')
        ser = serial.Serial('/dev/ttyUSB0', 115200)
        size = 100
        x_vec = np.linspace(0,1,size+1)[0:-1]
        line0, line1, line2, line3, line4, line5 = ([] for i in range(6))
        ser_data_acc_x = np.zeros(100)
        ser_data_acc_y = np.zeros(100)
        ser_data_acc_z = np.zeros(100)
        ser_data_gyro_x = np.zeros(100)
        ser_data_gyro_y = np.zeros(100)
        ser_data_gyro_z = np.zeros(100)

        x = ser.readline().decode('utf8').strip()
        x = ser.readline().decode('utf8').strip()
        j = json.loads(x)
        data = self.get_serial_data(ser)
        print(data)
        if j[0] == self.sensor and self.acc == True:
            while True:
                data = self.get_serial_data(ser)
                if isinstance(data, np.ndarray):
                    self.format_acc_data(data[3:len(data)])
                    ser_data_acc_x[-1] = self.acc_data[0]
                    ser_data_acc_y[-1] = self.acc_data[1]
                    ser_data_acc_z[-1] = self.acc_data[2]
                    line1, line2, line3 = self.plot_acc_data(
                        x_vec, ser_data_acc_x, ser_data_acc_y, ser_data_acc_z,
                        line1, line2, line3)

                    ser_data_acc_x = np.append(ser_data_acc_x[1:], 0.0)
                    ser_data_acc_y = np.append(ser_data_acc_y[1:], 0.0)
                    ser_data_acc_z = np.append(ser_data_acc_z[1:], 0.0)
                else:
                    self.click_times += 1
                    print('Click-Counter: ' + str(self.click_times))

        elif j[0] == self.sensor and self.gyro == True:
            while True:
                data = self.get_serial_data(ser)
                self.format_gyro_data(data[:3])
                ser_data_gyro_x[-1] = self.gyro_data[0]
                ser_data_gyro_y[-1] = self.gyro_data[1]
                ser_data_gyro_z[-1] = self.gyro_data[2]

                line3, line4, line5 = self.plot_gyro_data(
                x_vec, ser_data_gyro_x, ser_data_gyro_y, ser_data_gyro_z,
                line3, line4, line5)

                ser_data_gyro_x = np.append(ser_data_gyro_x[1:], 0.0)
                ser_data_gyro_y = np.append(ser_data_gyro_y[1:], 0.0)
                ser_data_gyro_z = np.append(ser_data_gyro_z[1:], 0.0)

        elif j[0] == self.sensor and self.acc_lin == True:
            while True:
                data = self.get_serial_data(ser)
                if isinstance(data, np.ndarray):
                    self.format_acc_lin_data(data)
                    self.get_position()
                    #print(self.x_pos, self.y_pos)
                    ser_data_acc_x[-1] = self.x_pos
                    ser_data_acc_y[-1] = self.y_pos
                    line1, line2 = self.plot_acc_lin_data(
                        x_vec, ser_data_acc_x, ser_data_acc_y,
                        line1, line2)

                    ser_data_acc_x = np.append(ser_data_acc_x[1:], 0.0)
                    ser_data_acc_y = np.append(ser_data_acc_y[1:], 0.0)
                else:
                    self.click_times += 1
                    print('Click-Counter: ' + str(self.click_times))

    def get_serial_data(self, ser):
        """ 
        get json string from serial bus, strip junk 
        """
        click = False
        x = ser.readline().decode('utf8').strip()
        x = ser.readline().decode('utf8').strip()
        j = json.loads(x)
        if j[0] == 'INDEX':
            data = True
        else:
            data = np.array(j[1])
        return data

    def format_acc_lin_data(self, data):
        """
        format acc_lin_data
        """
        self.acc_lin_data = data/100
        for i in range(len(data)):
            if self.acc_lin_data[i] > 655/2:
                self.acc_lin_data[i] = self.acc_lin_data[i] - 655

    def format_acc_data(self, data):
        """
        format raw accelerator data into +-2g range
        """
        self.acc_data = data/16384
        for i in range(len(data)):
            if self.acc_data[i] > 3.99994/2:
                self.acc_data[i] = self.acc_data[i] - 3.99994

    def format_gyro_data(self, data):
        """
        format raw gyro data
        """
        self.gyro_data = data/262.2
        for i in range(len(data)):
            if self.gyro_data[i] > 250/2:
                self.gyro_data[i] = self.gyro_data[i] - 250

    def plot_gyro_data(self, x, s_x, s_y, s_z, line1, line2, line3, pause_time=0.001):
        """
        docstring
        """
        if line1 == []:
            plt.ion()
            fig = plt.figure(figsize=(13,6))
            line1, = plt.plot(x, s_x, '-', alpha=0.8)
            plt.ylabel('Angular velocity in rad/s')
            plt.xlabel('Timestep ' + str(pause_time))
            plt.title('Gyroscope data')

            line2, = plt.plot(x, s_y, '-', alpha=0.8)

            line3, = plt.plot(x, s_z, '-', alpha=0.8)
            plt.legend((line1, line2, line3), ('x', 'y', 'z'))
            plt.ylim(-150, 150)
            plt.show()

        line1.set_ydata(s_x)
        line2.set_ydata(s_y)
        line3.set_ydata(s_z)

        plt.pause(pause_time)

        return line1, line2, line3

    def plot_acc_data(self, x, s_x, s_y, s_z, line1, line2, line3, pause_time=0.001):
        if line1 == []:
            # plt.vlines(x, ymin = 0, ymax = max(s), colors = 'purple')
            plt.ion()
            fig = plt.figure(figsize=(13,6))
            # ax = fig.add_subplot(111)
            line1, = plt.plot(x, s_x, '-', alpha=0.8)
            plt.ylabel('Accel in g')
            plt.title('Accelerator data')
            # ay = fig.add_subplot(111)
            line2, = plt.plot(x, s_y, '-', alpha=0.8)

            # az = fig.add_subplot(111)
            line3, = plt.plot(x, s_z, '-', alpha=0.8)
            plt.legend((line1, line2, line3), ('x', 'y', 'z'))
            plt.ylim(-2.5, 2.5)
            plt.show()

        line1.set_ydata(s_x)
        line2.set_ydata(s_y)
        line3.set_ydata(s_z)

        plt.pause(pause_time)

        return line1, line2, line3

    def plot_acc_lin_data(self, x, s_x, s_y, line1, line2, pause_time=0.00001):
        if line1 == []:
            plt.ion()
            fig = plt.figure(figsize=(13,6))
            #ax = fig.add_subplot(111)
            #line1, = ax.plot(x, s_x, '-', alpha=0.8)
            line1, = plt.plot(x, s_x, '-')
            plt.ylabel('Linear accel in m/s*s')
            plt.title('Accelerator data')
            #ay = fig.add_subplot(111)
            #line2, = ax.plot(x, s_y, '-', alpha=0.8)
            line2, = plt.plot(x, s_y, '-')
            plt.legend((line1, line2), ('x', 'y'))
            plt.ylim(-1, 1)
            plt.show()

        line1.set_ydata(s_x)
        line2.set_ydata(s_y)

        plt.pause(pause_time)

        return line1, line2

    def get_position(self):
        self.x_pos = self.x_pos - ((self.acc_lin_data[0]) * (0.5 * 0.03 * 0.03))
        self.y_pos = self.y_pos + ((self.acc_lin_data[1]) * (0.5 * 0.03 * 0.03))


p = Plotter(sys.argv)
p.plot_loop()