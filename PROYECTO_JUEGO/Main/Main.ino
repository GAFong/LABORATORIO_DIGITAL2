// Wire Slave Receiver
// Diego A. Mendez - 19673
// Gabriel Fong - 19722
// Esclavo I2C



int X;
int Y;
int PB;
int UP;
int DOWN;
char Valor;
char PUSH;
char outSerial;
char inSerial;

void setup() {
  pinMode(A0, INPUT);           //Pin de entrada analogo
  pinMode(A1, INPUT);
  pinMode(A2, INPUT_PULLUP);
  
  Serial.begin(9600);           // start serial for output
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
}

void loop() {

  X = analogRead(A0);
  Y = analogRead(A1);
  PB = digitalRead(A2);
  Valores();

  if (Serial.available()) {
  char inSerial = Serial.read();    // receive byte as an integer
   Envio(inSerial);
  Serial.write(outSerial); 
  }
 
  //Serial.println(inSerial);         // print the integer

  
  //Serial.print("Enviado");         // print the integer
  

//  Serial.print(Horizontal);   // send a X
//  Serial.print('\n');   // send a capital A
//  Serial.print(Vertical);   // send a capital A
//  Serial.print('\n');   // send a capital A
  Serial.print(Valor);   // send a capital A
  delay(100);
}


void Valores(void){
  if(X > 800){  //Joystick horizontal
    Valor= '1';  //Positivo
    }
  else if(X < 300){
    Valor = '2'; //Negativo
  }

  else if(Y > 800){  //Joystick Verical
    Valor= '3'; //Positivo
    }
  else if(Y < 300){ //Negativo
    Valor = '4';
  }
  else{
    Valor = '0'; //Cero
    }

   if(digitalRead(A2) == 0){  //Pushbutton
      PUSH='9'; //Presionado
   }
   else{
      PUSH ='8'; //Sin presionar
    }

}

void Envio(char t){
  switch(t){
      case 'V':
        outSerial = Valor;
        break;

      case 'P': 
        outSerial = PUSH;
        break;
    }
  }
