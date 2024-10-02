#include "IRremote.h"
#include <Servo.h>

#define sensorPin A0 //Para temperatura

//Pines
int receiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11
int servo = 9;
int rele = 3;
int serie = 2;
int led = 4;

//Para mensaje serie
int tecla;

//Lectura del sensor de temperatura
int lectura;
float voltios;
float temperatura;

//Grados del servo
int gradosServo = 0;


//Objetos
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
Servo myservo;  // objeto servo



void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
  myservo.attach(9);  // Pin 9 asignado al servo
  pinMode(rele, OUTPUT);
  pinMode(led,OUTPUT);
  digitalWrite(rele,HIGH);
  digitalWrite(rele,HIGH);

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */



// takes action based on IR code received
void translateIR(){ 

// describing Remote IR codes 

  switch(results.value)
  {
  //Encender/apagar led
  case 0xFFA25D: Serial.print("POWER: ");
  digitalWrite(led,!digitalRead(led));
  
  if(digitalRead(led) == HIGH){
    Serial.println("Led encendido");
  }

  else{
    Serial.println("Led apagado");
  }
  break;

  
  //Activo/desactivo el relé
  case 0xFFE21D: 
    Serial.print("FUNC/STOP:"); 
    digitalWrite(rele,!digitalRead(rele));

    if(digitalRead(rele) == LOW){
    Serial.println("Relé encendido");
  }

  else{
    Serial.println("Relé apagado");
  }
    
    break;
    
  case 0xFF629D: Serial.println("VOL+"); 
  break;
  
  case 0xFF22DD: Serial.println("FAST BACK"); 
  break;
  case 0xFF02FD: Serial.print("PAUSE: ");   

  // Lectura del sensor:
  lectura = analogRead(sensorPin);

  // Convertir a voltios:
  voltios = (lectura * 5.0)/1024.0;                     

  // De voltios a celcius:
  
  temperatura = (voltios - 0.5) * 100;

  Serial.print(temperatura);
  Serial.println("ºC");
  break;
  case 0xFFC23D: Serial.println("FAST FORWARD"); 
  break;
  case 0xFFE01F: Serial.println("DOWN"); 
  break;
  case 0xFFA857: Serial.println("VOL-");   
  break;
  case 0xFF906F: Serial.println("UP");    
  break;
  //Mando el valor que quiero mandar al servo
  case 0xFF9867: Serial.println("EQ"); 
  break;
  case 0xFFB04F: Serial.println("ST/REPT");
  break;
  
  //0º
  case 0xFF6897: Serial.print("0: ");   
  gradosServo = 0;
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  lcd.setCursor(0,1);
  Serial.println(gradosServo);
  break; 

  //90º
  case 0xFF30CF: Serial.println("1: ");
  gradosServo = 90;
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break; 

  //135º
  case 0xFF18E7: Serial.println("2: "); 
  gradosServo = 135;   
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break;

  //180º
  case 0xFF7A85: Serial.println("3: ");  
  gradosServo = 180;  
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break;

  //grados + 5
  case 0xFF10EF: Serial.println("4: ");
  gradosServo += 5;
  if(gradosServo > 180){
    gradosServo -= 180;
  }
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break;

  //grados + 10
  case 0xFF38C7: Serial.println("5: ");    
  gradosServo += 10;
  if(gradosServo > 180){
    gradosServo -= 180;
  }
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break;

  //grados + 20
  case 0xFF5AA5: Serial.println("6: ");
  gradosServo += 20;
  if(gradosServo > 180){
    gradosServo -= 180;
  }
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break;

  //grados + 30
  case 0xFF42BD: Serial.println("7: "); 
  gradosServo += 30;
  if(gradosServo > 180){
    gradosServo -= 180;
  }   
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break;

  //grados + 40

  case 0xFF4AB5: Serial.println("8: "); 
  gradosServo += 40;
  if(gradosServo > 180){
    gradosServo -= 180;
  }
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break;

  //grados + 50
  case 0xFF52AD: Serial.println("9: ");  
  gradosServo += 50;
  if(gradosServo > 180){
    gradosServo -= 180;
  }
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  Serial.println(gradosServo);
  break;

  //grados + 60
  case 0xFFFFFFFF: Serial.println(" REPEAT");

  gradosServo += 60;
  if(gradosServo > 180){
    gradosServo -= 180;
  }
  myservo.write(gradosServo); //Indicamos al servo a qué grados se tiene que poner
  break; 
   
  default: 
    Serial.println("Botón desconocido");

  }// End Case

  delay(500); // Do not get immediate repeat


} //END translateIR
