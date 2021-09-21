/*GABRIEL ALEXANDER FONG PENAGOS 19722
 *¨LABORATORIO 06
 *17/09/2021
 */
 #define ROJO RED_LED //PIN 30
 #define VERDE GREEN_LED //PIN 39
 #define AZUL BLUE_LED //PIN 30

 int PB1 = PUSH1;
 int PB2 = PUSH2;
 int FLAG1 = 0;
 int FLAG2 = 0;
 int CONT1 = 0;
 int CONT2 = 0;
 int INIT = 0;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);                 // Inicializar C. serial a 9600 bits per second
    pinMode(PB1, INPUT_PULLUP);      // COLOCAMOS LOS PUSH EN PULL UP
    pinMode(PB2, INPUT_PULLUP);      
     
    pinMode(ROJO, OUTPUT);               //LED ROJO COMO OUTPUT 
    pinMode(VERDE,OUTPUT);               //LED VERDE COMO OUTPUT 
    pinMode(AZUL, OUTPUT);               //LED AZUL COMO OUTPUT 
    
 // UTILIZAMOS LAS FILAS DEL PINOUT DE LA TIVA PARA UNA CONEXION FACIL DE LOS LEDS 
    pinMode(PB_3, OUTPUT);  
    pinMode(PC_4, OUTPUT);  
    pinMode(PC_5, OUTPUT);  
    pinMode(PC_6, OUTPUT);  
    pinMode(PC_7, OUTPUT);  
    pinMode(PD_6, OUTPUT);  
    pinMode(PD_7, OUTPUT);  
  
    pinMode(PA_2, OUTPUT);  
    
    pinMode(PB_5, OUTPUT);
    pinMode(PB_0, OUTPUT);
    pinMode(PB_1, OUTPUT);  
    pinMode(PE_4, OUTPUT);  
    pinMode(PE_5, OUTPUT);  
    pinMode(PB_4, OUTPUT);  
    pinMode(PA_5, OUTPUT);  
    pinMode(PA_6, OUTPUT);  
            

 
}

void loop() {
      //ANTIREBOTE 1 PARA EL COMIENZO
      if (digitalRead(PB1) == LOW)     //Pregunta si el PUSH1 está presionado
      {
        FLAG1 = 1;     //La variable cambia de valor
      }
      if (digitalRead(PB1) == HIGH && FLAG1 == 1)
      {
        if (INIT == 2){
          CONT1++;
        }
        if (INIT == 0){
          INIT = 1;
        }
        FLAG1 = 0;    //La variable vuelve a su valor original
     }
     //ANTIREBOTE 2 PARA EL COMIENZO
     if (digitalRead(PB2) == LOW)     //Pregunta si el PUSH1 está presionado
      {
        FLAG2 = 1;     //La variable cambia de valor
      }
      if (digitalRead(PB2) == HIGH && FLAG2 == 1)
      {
        if (INIT == 2){
          CONT2++;
        }
        if (INIT == 0){
          INIT = 1;
        }
        FLAG2 = 0;    //La variable vuelve a su valor original
        
     }

     if (INIT == 1){
      digitalWrite(ROJO, HIGH);
      digitalWrite(VERDE, LOW);
      digitalWrite(AZUL, LOW);
      delay(1000);
      digitalWrite(AZUL, HIGH);
      digitalWrite(ROJO, LOW);
      delay(1000);
      digitalWrite(VERDE, HIGH);
      digitalWrite(AZUL, LOW);
      delay(10);
      INIT = 2;
     }
     Serial.println(CONT1);

     if (INIT == 2){
      switch(CONT1){
        case 0:                 //TODO APAGADO
          digitalWrite(PB_5, LOW);
          digitalWrite(PB_0, LOW);
          digitalWrite(PB_1, LOW);
          digitalWrite(PE_4, LOW);
          digitalWrite(PE_5, LOW);
          digitalWrite(PB_4, LOW);
          digitalWrite(PA_5, LOW);
          digitalWrite(PA_6, LOW);
        break;
        
        case 1:               // CONT1 = 1
          digitalWrite(PB_5, HIGH);
        break;
        
        case 2:               // CONT1 = 2
          digitalWrite(PB_5, LOW);
          digitalWrite(PB_0, HIGH);
        break;

        case 3:               // CONT1 = 3
          digitalWrite(PB_0, LOW);
          digitalWrite(PB_1, HIGH);
        break;

        case 4:               // CONT1 = 4
          digitalWrite(PB_1, LOW);
          digitalWrite(PE_4, HIGH);
        break;

        case 5:               // CONT1 = 5
          digitalWrite(PE_4, LOW);
          digitalWrite(PE_5, HIGH);
        break;

        case 6:               // CONT1 = 6
          digitalWrite(PE_5, LOW);
          digitalWrite(PB_4, HIGH);
        break;

        case 7:               // CONT1 = 7
          digitalWrite(PB_4, LOW);
          digitalWrite(PA_5, HIGH);
        break;

        case 8:               // CONT1 = 8
          digitalWrite(PA_5, LOW);
          digitalWrite(PA_6, HIGH);
        break;

        case 9:               //GANADOR 1
          digitalWrite(PA_6, LOW);
          digitalWrite(VERDE, LOW);
          digitalWrite(ROJO, HIGH);
          delay(500);
          digitalWrite(ROJO, LOW);
          delay(500);
          digitalWrite(ROJO, HIGH);
          delay(500);
          digitalWrite(ROJO, LOW);
          delay(500);
          digitalWrite(ROJO, HIGH);
          delay(500);
          digitalWrite(ROJO, LOW);
          delay(500);
           digitalWrite(ROJO, HIGH);
          delay(500);
          digitalWrite(ROJO, LOW);
          delay(500);
          INIT = 0; 
          CONT1 = 0; 
          CONT2 = 0;
      }

      switch(CONT2){
        case 0:                 //TODO APAGADO
          digitalWrite(PB_3, LOW);
          digitalWrite(PC_4, LOW);
          digitalWrite(PC_5, LOW);
          digitalWrite(PC_6, LOW);
          digitalWrite(PC_7, LOW);
          digitalWrite(PD_6, LOW);
          digitalWrite(PD_7, LOW);
          digitalWrite(PA_2, LOW);
        break;
        
        case 1:               // CONT2 = 1
          digitalWrite(PB_3, HIGH);
        break;
        
        case 2:               // CONT2 = 2
          digitalWrite(PB_3, LOW);
          digitalWrite(PC_4, HIGH);
        break;

        case 3:               // CONT2 = 3
          digitalWrite(PC_4, LOW);
          digitalWrite(PC_5, HIGH);
        break;

        case 4:               // CONT2 = 4
          digitalWrite(PC_5, LOW);
          digitalWrite(PC_6, HIGH);
        break;

        case 5:               // CONT2 = 5
          digitalWrite(PC_6, LOW);
          digitalWrite(PC_7, HIGH);
        break;

        case 6:               // CONT2 = 6
          digitalWrite(PC_7, LOW);
          digitalWrite(PD_6, HIGH);
        break;

        case 7:               // CONT2 = 7
          digitalWrite(PD_6, LOW);
          digitalWrite(PD_7, HIGH);
        break;

        case 8:               // CONT2 = 8
          digitalWrite(PD_7, LOW);
          digitalWrite(PA_2, HIGH);
        break;

        case 9:               //GANADOR
          digitalWrite(PA_2, LOW);
          digitalWrite(VERDE, LOW);
          digitalWrite(AZUL, HIGH);
          delay(500);
          digitalWrite(AZUL, LOW);
          delay(500);
          digitalWrite(AZUL, HIGH);
          delay(500);
          digitalWrite(AZUL, LOW);
          delay(500);
          digitalWrite(AZUL, HIGH);
          delay(500);
          digitalWrite(AZUL, LOW);
          delay(500);
          digitalWrite(AZUL, HIGH);
          delay(500);
          digitalWrite(AZUL, LOW);
          delay(500);
          INIT = 0;  
          CONT1 = 0;
          CONT2 = 0;
      }
     }
}
