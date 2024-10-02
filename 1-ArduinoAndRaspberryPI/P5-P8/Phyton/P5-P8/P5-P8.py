
import serial #Importamos serial para realizar la comunicación serie con Arduino desde visual studio

Arduino = serial.Serial('COM4', 9600) #Puerto serie

while True:
    
    print (Arduino.readline().decode()) #Imprime lo que recibe el puerto serie del PC

