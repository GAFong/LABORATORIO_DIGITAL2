#GABRIEL ALEXANDER FONG PENAGOS

from INTERFAZ_LAB5 import *
from PyQt5 import QtWidgets
import threading
import serial
import time
import sys
from Adafruit_IO import Client, RequestError, Feed

ser = serial.Serial(baudrate=9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS, timeout=0)



ADAFRUIT_IO_KEY = "aio_sEHr11owgK9KsfwDMQJ87uN27xnf"
ADAFRUIT_IO_USERNAME = "GAFONG"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

class interfaz (QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__ (self):
        super().__init__()
        self.setupUi(self)
        # FUNCIONES DE BOTONES, SLIDERS, ETC.
        self.SENSOR1.valueChanged.connect(self.serial_sensor1)
        self.SENSOR2.valueChanged.connect(self.serial_sensor2)
        threading.Thread(daemon=True, target=posiciones).start()

    def serial_sensor1(self):
        global ser
        s1 = self.SENSOR1.value()
        print(s1)
        #SENSOR1 Feed
        analog_feed = aio.feeds('sensor1-lab5')
        aio.send_data(analog_feed.key, 1)
        digital_data = aio.receive(analog_feed.key)
        print(f'digital signal: {analog_data.value}')

    def serial_sensor2(self):
        global ser
        
        #SENSOR2 Feed
        analog_feed = aio.feeds('sensor2-lab5')
        aio.send_data(analog_feed.key, self.SENSOR2.value())
        analog_data = aio.receive(analog_feed.key)
        print(f'analog signal: {analog_data.value}')

    def actual(self):
        self.update()

def posiciones():
    global ventana, ser
    try:
        while (1):
            ser.flushInput()
            time.sleep(.3)
            #angulo = ser.readline()
            #ventana.posicion_garra(angulo)
            ser.readline()

            ventana.actual()
    except:
        print("Error: No esta comunicando")



app = QtWidgets.QApplication([])
ventana = interfaz()
ventana.show()
app.exec_()
