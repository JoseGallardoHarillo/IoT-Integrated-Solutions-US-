#include <LowPower.h>

int pinInter = 2; //El pin 2 posee interrupción, 
//por lo que le vamos a asignar la entrada digital 
//del botón este pin para que salte la interrupción cuando se pulse

int pinLDR = A0; //Pin asignado al terminal analógico del LDR

//Variables a rellenar
float lecturaLDR; 
float ldrVoltage;
float resistorVoltage;
float valorNominalLDR;

//Valores constantes
#define MAX_ADC_READING 1023
#define ADC_REF_VOLTAGE 5.0
#define REF_RESISTANCE 10000 //Nuestra resistencia

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(pinInter,INPUT);
  pinMode(pinLDR,INPUT);
  attachInterrupt(digitalPinToInterrupt(pinInter), muestreo, FALLING); //Activación de la interrupción en el pin 2, 
  //saltará cuando se envíe el primer byte de una transmisión serie dirección PC-Arduino
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //Ponemos el Arduino en Power Down

  LowPower.powerDown(SLEEP_FOREVER , ADC_ON , BOD_OFF); //El Arduino estará dormido para menor consumo, 
  //con el conversor ASC activado y con el procesador a disposición de resetearlo en caso de que la 
  //potencia que le llegue sea muy baja
  
}


void muestreo(){
    
    detachInterrupt(digitalPinToInterrupt(pinInter)); //Desactiva la interrupción
    //para impedir conflictos
    
    lecturaLDR = analogRead(pinLDR); //Lectura del LDR
    
    resistorVoltage = (float) lecturaLDR/MAX_ADC_READING * ADC_REF_VOLTAGE; //Pasamos la lectura a un valor entre 0 y 5V
    
    ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
    
    valorNominalLDR = ldrVoltage/ resistorVoltage * REF_RESISTANCE;

    Serial.println("----------------------------------");
    Serial.print("Voltaje de resistencia: ");
    Serial.println(resistorVoltage);
  
    Serial.print("Voltaje en ldr: ");
    Serial.println(ldrVoltage);
  
    Serial.print("Valor nominal LDR: ");
    Serial.println(valorNominalLDR);
    Serial.println("----------------------------------");
    Serial.flush(); 
    
    attachInterrupt(digitalPinToInterrupt(pinInter), muestreo, FALLING); //Activa la interrupción 
    //de nuevo para que pueda volver a ser usada
  
}
