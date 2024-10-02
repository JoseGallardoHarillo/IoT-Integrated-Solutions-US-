
#define MY_RF24_CHANNEL 36 //Canal
#define MY_NODE_ID 67 //Mi rango de ids esta entre 66 y 70, sirve para
//identificar a los nodos que incorporemos a nuestro sistema.
#define MY_DEBUG //Para imprimir por puerto serie
#define MY_RADIO_RF24 //Usamos un módulo nrf24
 
#include <MySensors.h>
#include <DHT.h>
#include <Vcc.h>

#define DHT_DATA_PIN 3 //Pin DHT

// Se establece esta compensación si el sensor tiene una pequeña compensación 
// permanente con respecto a las temperaturas reales. 
//En grados celcius (como temperatura por defecto)
#define SENSOR_TEMP_OFFSET 0

// Tiempo en el que se mantiene dormido entre actualición (en milisegundos)
// Debe ser >1000ms para DHT22 y >2000ms para DHT11
static const uint64_t UPDATE_INTERVAL = 60000;

//Objeto DHT
DHT dht;

//Batería

const float VccExpected   = 3.0;
const float VccCorrection = 2.860/2.92;
Vcc vcc(VccCorrection);
float v;
int oldBatteryPcnt = 0;
int BatteryPcnt;


// Fuerza enviar una actualización de la temperatura despues de 10 lecturas de sensor
static const uint8_t FORCE_UPDATE_N_READS = 10;

//id's
#define CHILD_ID_HUM 1
#define CHILD_ID_TEMP 2
#define CHILD_ID_BATTERY 3

//Tempertura y humedad
float lastTemp;
float lastHum;

uint8_t nNoUpdatesTemp;
uint8_t nNoUpdatesHum;

bool metric = true;  //Para indicar si se va a usar la medida por defecto (Celcius) o en algún otro (p.e Fahrenheit)

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgBat(CHILD_ID_BATTERY, V_VOLTAGE);

//Presentación de las ids para que se muestren por Domoticz
void presentation(){

 sendSketchInfo("TemperatureAndHumidity", "1.1");
 present(CHILD_ID_HUM, S_HUM);
 present(CHILD_ID_TEMP, S_TEMP);
 present(CHILD_ID_BATTERY, S_MULTIMETER);


 metric = getControllerConfig().isMetric; //Configurado para Celcius
}


void setup() {
  // put your setup code here, to run once:

  dht.setup(DHT_DATA_PIN); // Inicializa el DHT para que el pin de dato dea el indicado
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod()) { //Si no se respeta el intervalo
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
  //Duerme el tiempo del mínimo periodo de muestreo para darle tiempo a activarse
  //(de otra manera, podría ocurrir un error de timeout en la primera lectura)
  sleep(dht.getMinimumSamplingPeriod());
}

void loop() {
  // put your main code here, to run repeatedly:

  //ZONA BATERIA
  
  //Para el porcentaje(Sólo se imprime en DEBUG)
  if (oldBatteryPcnt != BatteryPcnt)
  {
    BatteryPcnt = (int)vcc.Read_Perc(VccExpected);
    oldBatteryPcnt = BatteryPcnt;
  }

  float v = vcc.Read_Volts(); //Lectura del voltaje

  //Impresión puerto serie de voltaje si está debug activo
  #ifdef MY_DEBUG
   Serial.print("VCC: ");
   Serial.print(v);
   Serial.println(" V");
   Serial.print("Pcnt: ");
   Serial.print(BatteryPcnt);
   Serial.println("%");
  #endif
 
  send(msgBat.set(v,1)); //Envío de voltaje a Domoticz

  //ZONA TEMPERATURA
  
  // Lectura del DHT
  dht.readSensor(true);
  
  //Obtención de la temperatua
  float temperature = dht.getTemperature();
  
  if (isnan(temperature)) {
    Serial.println("Failed reading temperature from DHT!");
  } else if (temperature != lastTemp || nNoUpdatesTemp == FORCE_UPDATE_N_READS) {
    // Sólo envía temperatura si cambia desde la última medida o si no enviamos una actualización 10 veces
    lastTemp = temperature;

    //Aplica el compensador antes de convertirlo en algo diferente que grados Celcius
    temperature += SENSOR_TEMP_OFFSET;

    if (!metric) {
      temperature = dht.toFahrenheit(temperature);
    }
    
    //Resetea el contador de actualizaciones
    nNoUpdatesTemp = 0;
    send(msgTemp.set(temperature, 1));//Envío de temperatura a Domoticz

  //Impresión puerto serie de temperatura si está debug activo
    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature);
    #endif
  } else {
    // Incrementa el contador de iteraciones sin actualización si la temperatura se mantiene igual
    nNoUpdatesTemp++;
  }

  //ZONA HUMEDAD
  
  // Obtención de humedad
  float humidity = dht.getHumidity();
  
  if (isnan(humidity)) {
    Serial.println("Failed reading humidity from DHT");
  } else if (humidity != lastHum || nNoUpdatesHum == FORCE_UPDATE_N_READS) {
    // Sólo envía humedad si cambia desde la última medida o si no enviamos una actualización 10 veces
    lastHum = humidity;
    //Resetea el contador de actualizaciones
    nNoUpdatesHum = 0;
    send(msgHum.set(humidity, 1));//Envío de humedad a Domoticz

    //Impresión puerto serie de humedad si está debug activo
    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity);
    #endif
  } else {
    // Incrementa el contador de iteraciones sin actualización si la temperatura se mantiene igual
    nNoUpdatesHum++;
  }

  // Se duerme durante el intervalo establecido
  sleep(UPDATE_INTERVAL); 

}
