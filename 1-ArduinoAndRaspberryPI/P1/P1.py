
import serial  #Importamos serial para realizar la comunicación serie con Arduino desde visual studio 

Arduino = serial.Serial('COM4', 9600) #Puerto serie


while True:

    val = input("Introduce tecla por favor: ")
    if len(val) >= 1: #impedir problemas con los inputs vacíos
        Arduino.write(val.encode()) #El valor introducido en val (en nuestro contexto una tecla), 
                                #es transmitido por el puerto serie Arduino

        print(Arduino.readline().decode()) #Imprime lo que recibe el puerto serie del PC