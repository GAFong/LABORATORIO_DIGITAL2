#GABRIEL ALEXANDER FONG PENAGOS

from SPI_INT import *
from PyQt5 import QtWidgets
import threading
import serial
import time
import sys

ser = serial.Serial(port='COM3',baudrate=9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS, timeout=0)

class interfaz (QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__ (self):
        super().__init__()
        self.setupUi(self)
        # FUNCIONES DE BOTONES, SLIDERS, ETC.

        threading.Thread(daemon=True, target=posiciones).start()

    def escribir(self,pot1,pot2):
        V1 = pot1
        V2 = pot2
        self.labelPOT1.setText('POT1 {}v'.format(V1))
        self.labelPOT2.setText('POT2   {}v'.format(V1))


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
            try:
                valoradc = str(ser.readline()).split(',')
                #print(valorservos)
                pot1 = int(valoradc[0][2])
                pot2 = int(valoradc[1][0])
                ventana.escribir(pot1,pot2)
                print(pot1,'\t',pot2)
            except :
                pass
            ventana.actual()
    except:
        print("Error: No esta comunicando")

app = QtWidgets.QApplication([])
ventana = interfaz()
ventana.show()
app.exec_()
