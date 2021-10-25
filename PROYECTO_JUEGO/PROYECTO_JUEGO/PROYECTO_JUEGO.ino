
 
#include <SD.h>
#include <SPI.h>

#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "lcd_registers.h"
#include "font.h"
#include "bitmaps.h"


#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7}; 

File ARCHIVO;
int PB1 = PUSH1;
int PB2 = PUSH2;
char SELECTOR;
int SEL_JUG = 0;
int SJUG = 0;
int FLAG1 = 0;
int FLAG2 = 0;
int START = 0;
uint16_t x = 0;
uint8_t y = 0;
uint16_t x2 = 0;
uint8_t y2 = 0;
uint16_t xcarro1 = 100;
uint8_t ycarro1 = 180;
uint16_t xcarro2 = 250;
uint8_t ycarro2 = 105;
uint16_t xcarro3 = 300;
uint16_t xcamion1 = 300;
uint8_t ycamion1 =195;
uint16_t xcamion2 = 300;
uint8_t ycamion2 =55;
uint16_t xcamion3 = 150;
uint8_t ycamion3 =55;
uint16_t xcar1 = 75;
uint8_t ycar1 = 165;
uint16_t xcar2 = 100 ;
uint8_t ycar2 = 90;
uint16_t xcar3 = 200;
uint8_t ycar3 = 90;
uint16_t xcar4 = 300;
uint8_t ycar4 = 90;
uint16_t xcono1 = 255;
uint8_t ycono1 = 150;
uint16_t xcono2 = 45;
uint8_t ycono2 = 150;
uint16_t xcono3 = 150;
uint8_t ycono3 = 135;
uint16_t xcono4 = 135;
uint8_t ycono4 = 135;
uint16_t xcono5 = 165;
uint8_t ycono5 = 135;
uint16_t xflorven1 = 165;
uint8_t yflorven1 = 120;
uint16_t xflorven2 = 150;
uint8_t yflorven2 = 120;
uint16_t xflorven3 = 0;
uint8_t yflorven3 = 210;
uint16_t xflorven4 = 15;
uint8_t yflorven4 = 210;
uint16_t xflorven5 = 30;
uint8_t yflorven5 = 210;
uint16_t xflorven6 = 300;
uint8_t yflorven6 = 210;
uint16_t xflorven7 = 285;
uint8_t yflorven7 = 210;
uint16_t xflorven8 = 270;
uint8_t yflorven8 = 210;

char J1;
char J2;
char PUSHJ1;

// PROTOTIPOS
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
void AJUMP_UPV(uint16_t x, uint8_t);
void AJUMP_UPN(uint16_t x, uint8_t);
void CARRO(uint16_t &x, uint8_t &y);
void CAR(uint16_t &x, uint8_t &y);
void CAMION1(uint16_t &x, uint8_t &y);
void VALSD(File f);

void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  Serial3.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Start");
  pinMode(PB1, INPUT_PULLUP);      // COLOCAMOS LOS PUSH EN PULL UP
  pinMode(PB2, INPUT_PULLUP);      // COLOCAMOS LOS PUSH EN PULL UP
  LCD_Init();
  LCD_Clear(0x00);

  //FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
  FillRect(60, 60, 210, 120, 0x0400);

  //LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  String nombre = "GABRIEL FONG";
  LCD_Print(nombre, 70, 100, 2, 0xffff, 0x0000);
  delay(1000);
  String apellido = "DIEGO MENDEZ";
  LCD_Print(apellido, 70, 120, 2, 0xffff, 0x0000);
  delay(1000);

  SPI.setModule(0);//UTILIZAMOS LA CONFIGURACION 0 PARA LA SD
  pinMode(12, OUTPUT);                //SC COMO OUTPUT
  
  if (!SD.begin(12)) {
    Serial.println("initialization failed!");
    return;
}
  Serial.println("initialization done.");
  ARCHIVO = SD.open("FROG.txt",FILE_READ);
  VALSD(ARCHIVO);  
  ARCHIVO.close();
  String jugador1 = "1 JUGADOR";
  LCD_Print(jugador1, 80, 190, 2, 0xff00, 0x0000);
  String jugador2 = "2 JUGADORES";
  LCD_Print(jugador2, 80, 210, 2, 0xff00, 0x0000);
}
void loop() {
  //ANTIREBOTE DEL BOTON 1
        LecJ1();
        char SELECTOR = J1;
        String flecha("->");
        switch (SELECTOR){
          case '3':
          
           LCD_Print(flecha, 52, 190, 2, 0xff00, 0x0000);
           for (int i = 0; i <15; i++){
              H_line(53, 210 + i, 26, 0x0000);}
              SEL_JUG = 1;
           break;
           
           case '4':
           LCD_Print(flecha, 52, 210, 2, 0xff00, 0x0000);
           for (int i = 0; i <15; i++){
           H_line(53, 190 + i, 26, 0x0000);}
           SEL_JUG = 2;
           break;

           default:
            break;
           
        }
        FLAG1 = 0;
        Serial.println(SEL_JUG);


if (PUSHJ1 == '9')     //Pregunta si el PUSH1 está presionado
      {
        FLAG2 = 1;     //La variable cambia de valor
      }
if (PUSHJ1 == '8' && FLAG2 == 1){
      FLAG1 = 0;
      switch (SEL_JUG){
          case 1: //1 JUGADOR
           START = 1;
           SJUG = 1;
           break;
           
           case 2://2JUGADOR
           START = 1;
           SJUG = 2;
           break; 
        }
        FLAG2 = 0;  
        FillRect(0, 0, 320, 240, 0x0000);
        LCD_Bitmap(xcono1, ycono1, 15, 15, CONO);
        LCD_Bitmap(xcono2, ycono2, 15, 15, CONO); 
        LCD_Bitmap(xcono3, ycono3, 15, 15, CONO); 
        LCD_Bitmap(xcono4, ycono4, 15, 15, CONO);
        LCD_Bitmap(xcono5, ycono5, 15, 15, CONO);
        LCD_Bitmap(xflorven1, yflorven1, 15, 15, FLORES_VEN);
        LCD_Bitmap(xflorven2, yflorven2, 15, 15, FLORES_VEN);
        LCD_Bitmap(xflorven3, yflorven3, 15, 15, FLORES_VEN); 
        LCD_Bitmap(xflorven4, yflorven4, 15, 15, FLORES_VEN);
        LCD_Bitmap(xflorven5, yflorven5, 15, 15, FLORES_VEN);
        LCD_Bitmap(xflorven6, yflorven6, 15, 15, FLORES_VEN);
        LCD_Bitmap(xflorven7, yflorven7, 15, 15, FLORES_VEN); 
        LCD_Bitmap(xflorven8, yflorven8, 15, 15, FLORES_VEN);  
           
        x = 90;
        y = 210;
        x2 = 180;
        y2 = 195;
        AJUMP_DON(&x,&y); 
        YJUMP_DON(&x2,&y2);   
        if (SJUG ==2){
          x2 = 180;
          y2 = 210;
          YJUMP_DON(&x2,&y2); 
        }
      }

while (START == 1){
  
  LecJ1();
  switch(J1){
      default:
      break;

      case '1':
      AJUMP_DN(&x,&y);
      break;

      case '2':
      AJUMP_IN(&x,&y);
      break;

      case '4':
      AJUMP_DON(&x,&y);
      Serial.write("down");
      break;

      case '3':
      AJUMP_UPN(&x,&y);
      break;  
  }
  if (SJUG ==2){
  LecJ2();
  switch(J2){
      default:
      break;

      case '1':
      YJUMP_DN(&x2,&y2);
      break;

      case '2':
      YJUMP_IN(&x2,&y2);
      break;

      case '4':
      YJUMP_DON(&x2,&y2);
      Serial.write("down");
      break;

      case '3':
      YJUMP_UPN(&x2,&y2);
      break;  
    }
  }
  CARRO(&xcarro2,&ycarro2);
  CARRO(&xcarro1,&ycarro1);
  CAR(&xcar1,&ycar1);
  CAR(&xcar2,&ycar2);
  CAR(&xcar3,&ycar3);
  CAR(&xcar4,&ycar4);
  CARRO(&xcarro3,&ycarro2);
  CAMION1(&xcamion1,&ycamion1);
  CAMION1(&xcamion2,&ycamion2);
  CAMION1(&xcamion3,&ycamion3);
  
 if (((xcarro1-10)<= x && x<= xcarro1+15) && ((ycarro1-5)<=y && y<= (ycarro1+5))||
  ((xcamion1-10)<= x && x<= xcamion1+35) && ((ycamion1 -8)<=y && y <= (5+ycamion1))||
  ((xcamion2-10)<= x && x<= xcamion2+35) && ((ycamion2 -8)<=y && y <= (5+ycamion2))||
  ((xcamion3-10)<= x && x<= xcamion3+35) && ((ycamion3 -8)<=y && y <= (5+ycamion3))||
  (((xcarro2-10)<= x && x<= xcarro2+15)||((xcarro3-10)<= x && x<= xcarro3+15)) && ((ycarro2-5)<=y && y<= (ycarro2+5))||
  ((xcar1-15)<= x && x<= (xcar1+10)) && ((ycar1-7)<=y && y<= (ycar1+10))||
  ((xcar2-15)<= x && x<= (xcar2+10)) && ((ycar2-7)<=y && y<= (ycar2+10))||
  ((xcar3-15)<= x && x<= (xcar3+10)) && ((ycar3-7)<=y && y<= (ycar3+10))||
  ((xcar4-15)<= x && x<= (xcar4+10)) && ((ycar4-7)<=y && y<= (ycar4+10))||
  (xflorven1 == x && y ==yflorven1)|| (xflorven2 == x && y ==yflorven2)|| (xflorven3 == x && y ==yflorven3)|| (xflorven4 == x && y ==yflorven4)|| (xflorven5 == x && y ==yflorven5)|| (xflorven6 == x && y ==yflorven6)|| (xflorven7 == x && y ==yflorven7)|| (xflorven8 == x && y ==yflorven8)
  ){
    START =0;
   
    LCD_Bitmap(x, y, 15, 15, MUERTE);
    for(int i = 0; i<10;i++){
    LCD_Bitmap(100+i*5, 15,80 , 7, GAME_OVER);
    delay(1000);
    FillRect(100+i*5, 15, 80, 7, 0x0000);
    }
    delay(2000);
    FillRect(0, 0, 320, 240, 0x0000);
    ARCHIVO = SD.open("FROG.txt",FILE_READ);
    VALSD(ARCHIVO);  
    ARCHIVO.close();
    String jugador1 = "1 JUGADOR";
    LCD_Print(jugador1, 80, 190, 2, 0xff00, 0x0000);
    String jugador2 = "2 JUGADORES";
    LCD_Print(jugador2, 80, 210, 2, 0xff00, 0x0000);
  }
}
        
}
//FUNCION DE LECTURA DE SERIAL

void LecJ1(void){
 Serial3.write('V'); 
  delay(5);
  if (Serial3.available()) {
      J1 = Serial3.read(); 
  }
  //Serial.print(XJ1);         // print the character
  delay(10); 
  
  Serial3.write('P'); 
   delay(5);
  if (Serial3.available()) {
      PUSHJ1 = Serial3.read(); 
  }
  //Serial.print(PUSHJ1);         // print the character
  delay(10); 
}
void LecJ2(void){
Serial5.write('V'); 
  delay(5);
  if (Serial5.available()) {
      J2 = Serial5.read(); 
  }
  //Serial.print(X);         // print the character
  delay(5); 
  
  Serial5.write('P'); 
   delay(5);
  if (Serial5.available()) {
      PUSHJ1 = Serial5.read(); 
  }
  Serial.print(PUSHJ1);         // print the character
  delay(5);
}
//RANA AZUL SALTANDO HACIA ARRIBA

void AJUMP_DON(uint16_t *x, uint8_t *y){
  if( *y > 210 || (*y ==(y2-15)  && *x == x2) || (*y ==(ycono1-15)  && *x == xcono1)|| (*y ==(ycono2-15)  && *x == xcono2)|| (*y ==(ycono3-15)  && *x == xcono3)|| (*y ==(ycono4-15)  && *x == xcono4)|| (*y ==(ycono5-15)  && *x == xcono5)){
  }
  else{
    for (int i = 0; i<3; i++){
      LCD_Bitmap(*x, *y, 15, 15, RA_DOWN_N);
      *y= *y +1;
      delay(5);
    }
    for (int i = 0; i<3; i++){
     LCD_Bitmap(*x, *y, 15, 15, RA_JUMP_DO_N);
     *y= *y +1;
     delay(5); 
    }
    for (int i = 0; i<6; i++){
     LCD_Bitmap(*x, *y, 15, 15, RA_FLY_DO_N);
     *y= *y +1;
     delay(5); 
    }
    for (int i = 0; i<3; i++){
      LCD_Bitmap(*x, *y, 15, 15, RA_DOWN_N);
      *y= *y +1;
      delay(5);
    }
 }
}
void AJUMP_UPN(uint16_t *x, uint8_t *y){
 if( (*y ==(y2+15)  && *x == x2)||(*y ==(ycono1+15)  && *x == xcono1)|| (*y ==(ycono2+15)  && *x == xcono2)|| (*y ==(ycono3+15)  && *x == xcono3)|| (*y ==(ycono4+15)  && *x == xcono4)|| (*y ==(ycono5+15)  && *x == xcono5)){
  }
  else {
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_UP_N);
    *y= *y -1;
    delay(5);
  }
  for (int i = 0; i<3; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_JUMP_UP_N);
   *y= *y -1;
   delay(5); 
  }
  for (int i = 0; i<6; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_FLY_UP_N);
   *y= *y -1;
   delay(5); 
  }
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_UP_N);
    *y= *y -1;
    delay(5);
  }
}
}
void AJUMP_DN(uint16_t *x, uint8_t *y){
  
  if(*x > 285|| (*x ==(x2-15)  && *y == y2)||(*x ==(xcono1-15)  && *y == ycono1)|| (*x ==(xcono2-15)  && *y == ycono2)|| (*x ==(xcono3-15)  && *y == ycono3)|| (*x ==(xcono4-15)  && *y == ycono4)|| (*x ==(xcono5-15)  && *y == ycono5)){
  }
  else{
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_D_N);
    *x= *x +1;
    delay(5);
  }
  for (int i = 0; i<3; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_JUMP_D_N);
   *x= *x +1;
   delay(5); 
  }
  for (int i = 0; i<6; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_FLY_D_N);
   *x= *x +1;
   delay(5); 
  }
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_D_N);
    *x= *x +1;
    
    delay(5);
  }
  }
}
void AJUMP_IN(uint16_t *x, uint8_t *y){
  if (*x < 15|| (*x ==(x2+15)  && *y == y2)||(*x ==(xcono1+15)  && *y == ycono1)|| (*x ==(xcono2+15)  && *y == ycono2)|| (*x ==(xcono3+15)  && *y == ycono3)|| (*x ==(xcono4+15)  && *y == ycono4)|| (*x ==(xcono5+15)  && *y == ycono5)){
  }
  else {
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_I_N);
    *x= *x -1;
    delay(5);
  }
  for (int i = 0; i<3; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_JUMP_I_N);
   *x = *x -1;
   delay(5); 
  }
  for (int i = 0; i<6; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_FLY_I_N);
   *x = *x -1;
   delay(5); 
  }
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_I_N);
    *x = *x -1;
    delay(5);
  }
  }
}
//RANA AMARILLA
void YJUMP_DON(uint16_t *x, uint8_t *y){
  if( *y > 210 || (*y ==(ycono1-15)  && *x == xcono1)|| (*y ==(ycono2-15)  && *x == xcono2)|| (*y ==(ycono3-15)  && *x == xcono3)|| (*y ==(ycono4-15)  && *x == xcono4)|| (*y ==(ycono5-15)  && *x == xcono5)){
  }
  else{
    for (int i = 0; i<3; i++){
      LCD_Bitmap(*x, *y, 15, 15, RA_DOWN_V);
      *y= *y +1;
      delay(5);
    }
    for (int i = 0; i<3; i++){
     LCD_Bitmap(*x, *y, 15, 15, RA_JUMP_DO_V);
     *y= *y +1;
     delay(5); 
    }
    for (int i = 0; i<6; i++){
     LCD_Bitmap(*x, *y, 15, 15, RA_FLY_DO_V);
     *y= *y +1;
     delay(5); 
    }
    for (int i = 0; i<3; i++){
      LCD_Bitmap(*x, *y, 15, 15, RA_DOWN_V);
      *y= *y +1;
      delay(5);
    }
 }
}
void YJUMP_UPN(uint16_t *x, uint8_t *y){
 if( (*y ==(ycono1+15)  && *x == xcono1)|| (*y ==(ycono2+15)  && *x == xcono2)|| (*y ==(ycono3+15)  && *x == xcono3)|| (*y ==(ycono4+15)  && *x == xcono4)|| (*y ==(ycono5+15)  && *x == xcono5)){
  }
  else {
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_UP_V);
    *y= *y -1;
    delay(5);
  }
  for (int i = 0; i<3; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_JUMP_UP_V);
   *y= *y -1;
   delay(5); 
  }
  for (int i = 0; i<6; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_FLY_UP_V);
   *y= *y -1;
   delay(5); 
  }
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_UP_V);
    *y= *y -1;
    delay(5);
  }
}
}
void YJUMP_DN(uint16_t *x, uint8_t *y){
  
  if(*x > 285|| (*x ==(xcono1-15)  && *y == ycono1)|| (*x ==(xcono2-15)  && *y == ycono2)|| (*x ==(xcono3-15)  && *y == ycono3)|| (*x ==(xcono4-15)  && *y == ycono4)|| (*x ==(xcono5-15)  && *y == ycono5)){
  }
  else{
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_D_V);
    *x= *x +1;
    delay(5);
  }
  for (int i = 0; i<3; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_JUMP_D_V);
   *x= *x +1;
   delay(5); 
  }
  for (int i = 0; i<6; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_FLY_D_V);
   *x= *x +1;
   delay(5); 
  }
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_D_V);
    *x= *x +1;
    
    delay(5);
  }
  }
}
void YJUMP_IN(uint16_t *x, uint8_t *y){
  if (*x < 15|| (*x ==(xcono1+15)  && *y == ycono1)|| (*x ==(xcono2+15)  && *y == ycono2)|| (*x ==(xcono3+15)  && *y == ycono3)|| (*x ==(xcono4+15)  && *y == ycono4)|| (*x ==(xcono5+15)  && *y == ycono5)){
  }
  else {
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_I_V);
    *x= *x -1;
    delay(5);
  }
  for (int i = 0; i<3; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_JUMP_I_V);
   *x = *x -1;
   delay(5); 
  }
  for (int i = 0; i<6; i++){
   LCD_Bitmap(*x, *y, 15, 15, RA_FLY_I_V);
   *x = *x -1;
   delay(5); 
  }
  for (int i = 0; i<3; i++){
    LCD_Bitmap(*x, *y, 15, 15, RA_I_V);
    *x = *x -1;
    delay(5);
  }
  }
}
void CARRO(uint16_t *x, uint8_t *y){
    LCD_Bitmap(*x, *y, 15, 15, CARRO1);
    V_line(*x + 15, *y, 15, 0x0000);
    V_line(*x + 14, *y, 15, 0x0000);
    V_line(*x + 16, *y, 15, 0x0000);
    *x = *x-2;
    //Serial.println(*x);
    if (*x >= 65530){
      *x = 300;
      for (int i = 0; i<15; i++){
      V_line(i, *y, 15, 0x0000);}
    }  
      
    delay(5);
}
void CAR(uint16_t *x, uint8_t *y){
    LCD_Bitmap(*x, *y, 15, 15, CARRO2);
    V_line(*x - 1, *y, 15, 0x0000);
    V_line(*x - 2, *y, 15, 0x0000);
    V_line(*x - 3, *y, 15, 0x0000);
    V_line(*x - 4, *y, 15, 0x0000);
    V_line(*x - 5, *y, 15, 0x0000);
    V_line(*x - 6, *y, 15, 0x0000);
    *x = *x+6;
    //Serial.println(*x);
    if (*x >= 320){
      *x = 0;
      for (int i = 0; i<20; i++){
      V_line(300+i, *y, 15, 0x0000);}
    }  
      
    delay(5);
}
void CAMION1(uint16_t *x, uint8_t *y){
    LCD_Bitmap(*x, *y, 30, 10, CAMION);
    V_line(*x + 34, *y, 15, 0x0000);
    V_line(*x + 33, *y, 15, 0x0000);
    V_line(*x + 35, *y, 15, 0x0000);
    V_line(*x + 36, *y, 15, 0x0000);
    *x = *x-4;
    
    if (*x >= 65530){
      *x = 300;
      for (int i = 0; i<33; i++){
      V_line(i, *y, 15, 0x0000);}
    }  
      
    delay(5);
}

void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c); 
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
    for (int j = 0; j < height; j++){
        k = (j*(ancho) + index*width -1 - offset)*2;
        k = k+width*2;
       for (int i = 0; i < width; i++){
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k+1]);
        k = k - 2;
       } 
    }
  }
  else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
    }
  }
  digitalWrite(LCD_CS, HIGH);
}

void VALSD(File f){
  LCD_CMD(0x02c); 
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  
  SetWindows(0,0,319,239);
  
  uint8_t VALOR11; 
  uint8_t VALOR12; 
  uint8_t VALOR21;
  uint8_t VALOR22;
  f.seek(0); //COLOCAMOS AL INICIO DEL TXT
  while(f.available()){
    VALOR11  = ASCII_HEX(f.read());
    VALOR12 = ASCII_HEX(f.read());
    VALOR21 = ASCII_HEX(f.read());
    VALOR22 = ASCII_HEX(f.read());
    unsigned char color1 = (VALOR11*16) + VALOR12; //SUMAMOS LOS VALORES EN SUS POSICIONES
    unsigned char color2 = (VALOR21*16) + VALOR22;
    LCD_DATA(color1); 
    LCD_DATA(color2); 
  }
   digitalWrite(LCD_CS, HIGH);
  
  }
  int ASCII_HEX(char VAL){ 
  if (VAL >= '0' && VAL <= '9') //NUMEROS DE HEX
    return VAL - '0' ;
  
  if (VAL >= 'a' && VAL <= 'f')//LAS LETRAS DEL HEX
    return VAL - 'a' + 10 ;
}
