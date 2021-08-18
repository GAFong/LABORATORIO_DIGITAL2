#GABRIEL ALEXANDER FONG PENAGOS

from INTERFAZ_LAB5 import *
from PyQt5 import QtWidgets
import threading
import serial
import time
import sys
from Adafruit_IO import Client, RequestError, Feed

ser = serial.Serial(port='COM3',baudrate=9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS, timeout=0)



ADAFRUIT_IO_KEY = "aio_BBkL78xxoSLB4sszOqFktc3aoiFB"
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

        analog_feed = aio.feeds('sensor1-lab5')
        aio.send_data(analog_feed.key, self.SENSOR1.value())
        analog_data = aio.receive(analog_feed.key)
        print(f'digital signal: {analog_data.value}')

    def serial_sensor2(self):
        global ser

        analog_feed = aio.feeds('sensor2-lab5')
        aio.send_data(analog_feed.key, self.SENSOR2.value())
        analog_data = aio.receive(analog_feed.key)
        print(f'analog signal: {analog_data.value}')


    def escribir(self,cont1,cont2,cont3):
        vcont = cont3+(cont2*10)+(cont1*100)
        analog_feed = aio.feeds('contador-lab5')
        aio.send_data(analog_feed.key, vcont)


    def actual(self):

        self.update()

def posiciones():
    global ventana, ser
    try:
        while (1):
            ser.flushInput()
            time.sleep(.3)
            #angulo = ser.readline()
            analog_feed = aio.feeds('valor')
            analog_data = aio.receive(analog_feed.key)
            #print((str(analog_data.value))[2:0])
            valor = analog_data.value
            print(valor)
            v1 = hex(ord(str(analog_data.value)[2:]))[2:4]
            v2 = hex(ord(str(analog_data.value)[1:2]))[2:4]
            v3 = hex(ord(str(analog_data.value)[0:1]))[2:4]
            print(v1)
            print(v2)
            print(v3)
            ser.write(bytes.fromhex(v3))
            ser.write(bytes.fromhex(v2))
            ser.write(bytes.fromhex(v1))
            ser.readline()
            try:

                contador = str(ser.readline()).split(',')
                #print(valorservos)
                cont1 = int(contador[0][2])
                cont2 = int(contador[1][0])
                cont3 = int(contador[2][0])
                ventana.escribir(cont1,cont2,cont3)
            except :
                pass
            ventana.actual()

    except:
        print("Error: No esta comunicando")



app = QtWidgets.QApplication([])
ventana = interfaz()
ventana.show()
app.exec_()
