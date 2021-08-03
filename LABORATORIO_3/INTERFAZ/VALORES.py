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
        self.ENVIAR.valueChanged.connect(self.serial_cont)
        threading.Thread(daemon=True, target=posiciones).start()

    def serial_cont(self):
        global ser
        ser.write(bytes.fromhex('61'))  #letra a
        ser.write(bytes.fromhex(hex(ord(str(self.TEXT_CONTADOR.value())))[2:]))
        
    def escribir(self,pot1_1,pot1_2,pot1_3,pot2_1,pot2_2,pot2_3):
        V1 = pot1_1 + (0.1*pot1_2) + (0.01*pot1_3)
        V2 = pot2_1 + (0.1*pot2_2) + (0.01*pot2_3)
        self.POT1.setText('POT1  {}v'.format(V1))
        self.POT2.setText('POT2  {}v'.format(V2))


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
                pot1_1 = int(valoradc[0][2])
                pot1_2 = int(valoradc[1][0])
                pot1_3 = int(valoradc[2][0])
                pot2_1 = int(valoradc[3][0])
                pot2_2 = int(valoradc[4][0])
                pot2_3 = int(valoradc[5][0])
                ventana.escribir(pot1_1,pot1_2,pot1_3,pot2_1,pot2_2,pot2_3)
                print(pot1_1,'\t',pot2_1)
            except :
                pass
            ventana.actual()
    except:
        print("Error: No esta comunicando")

app = QtWidgets.QApplication([])
ventana = interfaz()
ventana.show()
app.exec_()
