
//Solo cuando usemos Arduino Nano
#define MY_RF24_CE_PIN 10
#define MY_RF24_CS_PIN 9


#define MY_RF24_CHANNEL 36 //Canal
#define MY_NODE_ID 67 //Mi rango de ids esta entre 66 y 70, sirve para
//identificar a los nodos que incorporemos a nuestro sistema.
#define MY_DEBUG //Para imprimir por puerto serie
#define MY_RADIO_RF24 //Usamos un módulo nrf24


#include <MySensors.h>
#include <Bounce2.h>


//id's
#define child_id1 1 //id del switch 1
#define child_id2 2 //id del switch 2
#define child_id3 3 //id del relé

//pines
#define switch1 3
#define switch2 4 
#define rele 5


//Variables para conservar el estado del relé para los switches
int antiguoValor1 = -1;
int antiguoValor2 = -1;
bool estado = false;

//Dos debouncers para impedir rebote en los switches/botones
Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();


//canal de comunicación entre x id y la raspberry pi
MyMessage mensaje1(child_id1, V_STATUS);
MyMessage mensaje2(child_id2, V_STATUS);
MyMessage mensaje3(child_id3, V_STATUS);

//Presentación de las ids para que se muestren por Domoticz
void presentation(){
 present(child_id1, S_BINARY); //S_BINARY  sirve para comportamientos binarios,
 //ideal para botones y relés
 present(child_id2, S_BINARY);
 present(child_id3, S_BINARY);
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9800);

  //Configuración de pines
  pinMode(switch1, INPUT);
  pinMode(switch2, INPUT);
  pinMode(rele, OUTPUT);

  //Inicialización
  digitalWrite(switch1,HIGH);
  digitalWrite(switch2,HIGH);
  digitalWrite(rele, 0);

  debouncer1.attach(switch1);
  debouncer1.interval(10);
  debouncer2.attach(switch2);
  debouncer2.interval(10); 
}

void loop() {
  // put your main code here, to run repeatedly:

  estado = false;
  debouncer1.update();
  debouncer2.update();

  //Valores de switches
  int valor1 = debouncer1.read();
  int valor2 = debouncer2.read();

  //Usando switches

  //Comprobación switch 1
  if(valor1 != antiguoValor1){ //Si el estado del switch 1 cambia 
    //entonces se informa a Domoticz 
    send(mensaje1.set(valor1 == HIGH? 1:0)); //Encendido/Apagado (Esto solo sirve
    //para que se encienda o se apague la bombillita del estado del switch en Domoticz)
    antiguoValor1 = valor1;
    estado = true;
  }

  //Comprobación switch 2
  if(valor2 != antiguoValor2){ //Si el estado del switch 2 cambia entonces se informa a Domoticz 
    send(mensaje2.set(valor2 == HIGH? 1:0)); //Encendido/Apagado (Esto solo sirve
    //para que se encienda o se apague la bombillita del estado del switch 2 en Domoticz)
    antiguoValor2 = valor2;
    estado = true;
  }

  if(estado){ //Si el estado ha cambiado, se envía un mensaje para cambiar el relé
    int led = digitalRead(rele);
    send(mensaje3.set(led == HIGH? 1:0)); //Encendido/Apagado en Domoticz
    digitalWrite(rele, led == HIGH? 0:1);  //Para cambiar el estado del relé con los switches, 
    //cuando se interactua con el relé directamente el valor de encendido/apagado es al revez
  }

}



//Usando domoticz
void receive (const MyMessage &message){
  //identificar a qué sensor se refieren

  if(message.getSensor() ==  child_id3 && message.type == V_STATUS){ 
    //Si el mensaje va relacionado con el relé
    
    estado = message.getBool();
    digitalWrite(rele, estado? 0:1);//Encendido/apagado de relé desde domoticz
    //cuando se interactua con el relé directamente el valor de encendido/apagado es al revez
    
    //Impresión puerto serie cuando se activa desde domoticz si está debug activo
    #ifdef MY_DEBUG
    Serial.print("Cambios detectados en el sensor:");
    Serial.print(message.getSensor());
    Serial.print(", Nuevo estado: ");
    Serial.println(message.getBool());
    #endif
    
  }
}
