#include <Servo.h>

Servo myservo;  // objeto servo

//Variable que almacenará la información
//recibida mediante puerto serie
int tecla;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // Pin 9 asignado al servo
}

void loop() {

  if (Serial.available()>0){//Si recibe algún byte desde puerto serie
    
    tecla = Serial.parseInt(); //Leemos y parseamos a entero el valor recibido para usarlo en el servo
    myservo.write(tecla); //Indicamos al servo a qué grados se tiene que poner
    
    Serial.println(tecla);
  }
 
}
