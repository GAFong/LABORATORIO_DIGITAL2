//GABRIEL ALEXANDER FONG PENAGOS
//19722
#include <SD.h>
#include <SPI.h>

 #define ROJO RED_LED //PIN 30
 #define VERDE GREEN_LED //PIN 39
 #define AZUL BLUE_LED //PIN 30

char SELECT = 0;
int SEL = 0;
int PB1 = PUSH1;
int PB2 = PUSH2;
int FLAG1 = 0;
String inputString = "";
File myFile;

void setup() {
  SPI.setModule(0);//UTILIZAMOS LA CONFIGURACION 0 PARA LA SD
  Serial.begin(9600);
  pinMode(PB1, INPUT_PULLUP);      // COLOCAMOS LOS PUSH EN PULL UP
  pinMode(PB2, INPUT_PULLUP); 
  pinMode(12, OUTPUT);                //SC COMO OUTPUT
  pinMode(ROJO, OUTPUT);               //LED ROJO COMO OUTPUT 
  pinMode(VERDE,OUTPUT);               //LED VERDE COMO OUTPUT 
  pinMode(AZUL, OUTPUT);               //LED AZUL COMO OUTPUT 

  if (!SD.begin(12)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
    

}

void loop() {
  //ANTIREBOTE 1 PARA EL COMIENZO
      if (digitalRead(PB1) == LOW)     //Pregunta si el PUSH1 está presionado
      {
        FLAG1 = 1;     //La variable cambia de valor
      }
      if (digitalRead(PB1) == HIGH && FLAG1 == 1)
      {
       Serial.print("\n SELECCIONE LA IMAGEN A MOSTRAR\n");
       Serial.print("(1)SPIDER \n(2)TESLA \n(3)TWITTER\n");
       inputString = "";
       FLAG1 = 0;
       
     }
     while (Serial.available()){
       SELECT = (char)Serial.read();
      //inputString += SELECT;
      SEL = int(SELECT);
      switch (SEL){
      case 48:
        digitalWrite(ROJO, LOW);
        digitalWrite(VERDE, LOW);
        digitalWrite(AZUL, LOW);
      break;
      
      case 49:
        digitalWrite(ROJO, HIGH);
        digitalWrite(VERDE, LOW);
        digitalWrite(AZUL, LOW);
        myFile = SD.open("spider.txt");
        
        if (myFile) {
          Serial.println("spider.txt:");
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        }
      break;
      
      case 50:
        digitalWrite(ROJO, LOW);
        digitalWrite(VERDE, HIGH);
        digitalWrite(AZUL, LOW);
        myFile = SD.open("tesla.txt");
        
        if (myFile) {
          Serial.println("tesla.txt:");
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        }
      break;

      case 51:
        digitalWrite(ROJO, LOW);
        digitalWrite(VERDE, LOW);
        digitalWrite(AZUL, HIGH);
        myFile = SD.open("TWITTER.txt");
        
        if (myFile) {
          Serial.println("TWITTER.txt:");
      
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
            Serial.write(myFile.read());
          }
          // close the file:
          myFile.close();
        }
      break;
     }
     }
    
     
  // put your main code here, to run repeatedly: 
  
}
