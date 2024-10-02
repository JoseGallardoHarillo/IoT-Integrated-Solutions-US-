
#ioinclude <LiquidCrystal_I2C.h>

  //Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
  LiquidCrystal_I2C lcd(0x3F,16,2);  

  int mensaje; //Mensaje que se deberá imprimir por el LCD 

  //Pin para el sensor
  #define sensorPin A0

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

  // Lectura del sensor:
  int lectura = analogRead(sensorPin);

  // Convertir a voltios:
  float voltios = (lectura * 5.0)/1024.0;                     

  // De voltios a celcius:
  
  float temperatura = (voltios - 0.5) * 100;


  lcd.clear();
  lcd.print(temperatura);
  lcd.print((char) 223); // grados
  lcd.print("C");

  delay(1000);

}
