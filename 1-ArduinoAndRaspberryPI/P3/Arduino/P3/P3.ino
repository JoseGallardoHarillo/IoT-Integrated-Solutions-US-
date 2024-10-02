
#include <LiquidCrystal_I2C.h>

  //Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
  LiquidCrystal_I2C lcd(0x3F,16,2);  

  String mensaje; //Mensaje que se deberá imprimir por el LCD 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()>0){ //Si recibe algún byte desde puerto serie
    lcd.clear(); //Recoloca el LCD de paso a la columna 0

    mensaje = Serial.readStringUntil('\n'); //Leemos el mensaje escrito vía serie

    String fila = mensaje.substring(0,16); //fila 1
    
    lcd.print(letra);

    lcd.setCursor(0,1);

    fila = mensaje.substring(16,32); //fila 2
    lcd.print(letra);
  }
}
