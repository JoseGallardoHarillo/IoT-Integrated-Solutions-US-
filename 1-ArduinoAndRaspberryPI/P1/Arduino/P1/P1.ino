
//En mi caso poseo una placa con 2 relés, 
//para usar cada uno asigno los siguientes pines
int rele = 2;
int rele2 = 3;

//Variable que almacenará la información
//recibida mediante puerto serie
int tecla;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  pinMode(rele2, OUTPUT);

  //Ponemos el estado de los relés en alto para que se 
  //inicialicen apagados en caso de que en el uso anterior
  //se haya dejado algún relé encendido 
  digitalWrite(rele, HIGH);
  digitalWrite(rele2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()>0){ //Si recibe algún byte desde puerto serie
    tecla=Serial.read(); //Lectura del puerto serie
    
    if(tecla=='1'){ //Si se ha pulsado la tecla 1
      digitalWrite(rele, LOW); //Enciende relé 1
      Serial.println("rele 1 encendido"); 
      }
     
     if(tecla=='0'){ //Si se ha pulsado la tecla 0
      digitalWrite(rele, HIGH); //Apaga relé 1
      Serial.println("rele 1 apagado"); 
      }

      if(tecla=='3'){ //Si se ha pulsado la tecla 3
      digitalWrite(rele2, LOW); //Enciende relé 2
      Serial.println("rele 2 encendido"); 
      }

      if(tecla=='2'){ //Si se ha pulsado la tecla 2
      digitalWrite(rele2, HIGH); //Apaga relé 2
      Serial.println("rele 2 apagado");
      }
  }
}
