
// Enable and select radio type attached 
#define MY_RF24_CHANNEL 36
#define MY_NODE_ID 68 //Por ejemplo (66-70)
#define MY_DEBUG //PARA POLIMETRO CASO 2
#define MY_RADIO_RF24

//SIGNING
#define MY_SIGNING_SOFT
#define MY_SIGNING_SOFT_RANDOMSEED_PIN 7
#define MY_SIGNING_REQUEST_SIGNATURES
#define MY_DEBUG_VERBOSE_SIGNING

#include <SPI.h>
#include <MySensors.h>  
#include <DHT.h>
#include <Vcc.h>

// Set this to the pin you connected the DHT's data pin to
#define DHT_DATA_PIN 3

// Set this offset if the sensor has a permanent small offset to the real temperatures.
// In Celsius degrees (as measured by the device)
#define SENSOR_TEMP_OFFSET 0

// Sleep time between sensor updates (in milliseconds)
// Must be >1000ms for DHT22 and >2000ms for DHT11
static const uint64_t UPDATE_INTERVAL = 60000;

//Batería

const float VccExpected   = 3.0;
const float VccCorrection = 2.860/2.92;  // Measured Vcc by multimeter divided by reported Vcc
int oldBatteryPcnt = 0;
int BatteryPcnt;
float v;

Vcc vcc(VccCorrection);



// Force sending an update of the temperature after n sensor reads, so a controller showing the
// timestamp of the last update doesn't show something like 3 hours in the unlikely case, that
// the value didn't change since;
// i.e. the sensor would force sending an update every UPDATE_INTERVAL*FORCE_UPDATE_N_READS [ms]
static const uint8_t FORCE_UPDATE_N_READS = 10;

//id's
#define CHILD_ID_HUM 1
#define CHILD_ID_TEMP 2
#define CHILD_ID_BATTERY 3

float lastTemp;
float lastHum;
uint8_t nNoUpdatesTemp;
uint8_t nNoUpdatesHum;
bool metric = true;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgBat(CHILD_ID_BATTERY, V_VOLTAGE);
DHT dht;

void presentation(){
// Send the sketch version information to the gateway

 sendSketchInfo("TemperatureAndHumidity", "1.1");
 present(CHILD_ID_HUM, S_HUM);
 present(CHILD_ID_TEMP, S_TEMP);
 present(CHILD_ID_BATTERY, S_MULTIMETER);

 metric = getControllerConfig().isMetric;
}


void setup() {
  // put your setup code here, to run once:

  dht.setup(DHT_DATA_PIN); // set data pin of DHT sensor
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod()) {
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
  // Sleep for the time of the minimum sampling period to give the sensor time to power up
  // (otherwise, timeout errors might occure for the first reading)
  sleep(dht.getMinimumSamplingPeriod());
}

void loop() {
  // put your main code here, to run repeatedly:


  //ZONA BATERIA

  if (oldBatteryPcnt != BatteryPcnt)
  {
    BatteryPcnt = (int)vcc.Read_Perc(VccExpected);
    oldBatteryPcnt = BatteryPcnt;
  }

  
  float v = vcc.Read_Volts();

  
  #ifdef MY_DEBUG
   Serial.print("VCC: ");
   Serial.print(v);
   Serial.println(" V");
   Serial.print("Pcnt: ");
   Serial.print(BatteryPcnt);
   Serial.println("%");
  #endif
 
  send(msgBat.set(v,1));

  //ZONA TEMPERATURA
  
  // Force reading sensor, so it works also after sleep()
  dht.readSensor(true);
  
  // Get temperature from DHT library
  float temperature = dht.getTemperature();
  if (isnan(temperature)) {
    Serial.println("Failed reading temperature from DHT!");
  } else if (temperature != lastTemp || nNoUpdatesTemp == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp = temperature;

    // apply the offset before converting to something different than Celsius degrees
    temperature += SENSOR_TEMP_OFFSET;

    if (!metric) {
      temperature = dht.toFahrenheit(temperature);
    }
    // Reset no updates counter
    nNoUpdatesTemp = 0;
    send(msgTemp.set(temperature, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature);
    #endif
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp++;
  }

  //ZONA HUMEDAD
  
  // Get humidity from DHT library
  float humidity = dht.getHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed reading humidity from DHT");
  } else if (humidity != lastHum || nNoUpdatesHum == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum = humidity;
    // Reset no updates counter
    nNoUpdatesHum = 0;
    send(msgHum.set(humidity, 1));
    
    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity);
    #endif
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum++;
  }

  // Sleep for a while to save energy
  sleep(UPDATE_INTERVAL); 

}
