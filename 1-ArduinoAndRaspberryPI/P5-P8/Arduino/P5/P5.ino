int pinInter = 2; //El pin 2 posee interrupción, 
//por lo que le vamos a asignar la entrada digital 
//del botón este pin para que salte la interrupción cuando se pulse

int pinLDR = A0; //Pin asignado al terminal analógico del LDR

//Variables a rellenar
float lecturaLDR; 
float ldrVoltage;
float resistorVoltage;
float valorNominalLDR;

//Anti-Rebote

long startTime = 0;
const int timeWait = 150;

//Valores constantes
#define MAX_ADC_READING 1023
#define ADC_REF_VOLTAGE 5.0
#define REF_RESISTANCE 10000 //Nuestra resistencia

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(pinInter,INPUT_PULLUP); //Cuando no esta pulsado está a 1 (no pulsado)
  pinMode(pinLDR,INPUT);
  attachInterrupt(digitalPinToInterrupt(pinInter), muestreo, FALLING); //Activación de la 
  //interrupción en el pin 2, saltará cuando el pulsador está a 0 (pulsado)
}

void loop() {
  // put your main code here, to run repeatedly:
}


void muestreo(){

  if(millis()-startTime > timeWait){ //Si no se mantiene un tiempo de
    //espera entre pulsación y pulsación
    
    detachInterrupt(digitalPinToInterrupt(pinInter)); //Desactiva la interrupción 
    //para impedir conflictos
    
    lecturaLDR = analogRead(pinLDR); //Lectura de LDR
    
    resistorVoltage = (float) lecturaLDR/MAX_ADC_READING * ADC_REF_VOLTAGE; //Pasamos la lectura a un 
    //valor entre 0 y 5V
    
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
    
    startTime = millis(); //El tiempo pasado se captura para la condición de anti-rebote
    
    attachInterrupt(digitalPinToInterrupt(pinInter), muestreo, FALLING); //Activa la interrupción 
    //de nuevo para que pueda volver a ser usada
  }
  
}
