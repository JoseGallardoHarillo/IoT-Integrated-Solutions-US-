
import serial #Importamos serial para realizar la comunicación serie con Arduino desde visual studio

Arduino = serial.Serial('COM4', 9600) #Puerto serie

while True:
    
    val = input("Introduce tecla por favor: ")
    
    if len(val) >= 1: #impedir problemas con los imputs vacíos
        Arduino.write(val.encode()) #El valor introducido en val (en nuestro contexto una tecla), 
                                #es transmitido por el puerto serie Arduino