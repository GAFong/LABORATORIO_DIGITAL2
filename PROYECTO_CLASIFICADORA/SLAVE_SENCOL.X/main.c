/* 
 * File:   main.c
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 * SLAVE SENSOR DE COLOR
 * Created on August 23, 2021, 2:52 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "I2C.h"
#include "LCD.h"
/*
 * 
 */
// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#define _XTAL_FREQ 8000000
#define S0 0    //SELECTORES DEL SENSOR
#define S1 1
#define S2 2
#define S3 3
#define TMR1H_value	0x00;
#define TMR1L_value 0x00;

//------------------------------VARIABLES---------------------------------------
uint8_t ROJO;
uint8_t AZUL;
uint8_t VERDE;
uint8_t POS1;
uint8_t POS2;
uint8_t POS3;
uint8_t COLORH;
uint8_t COLORL;
uint16_t frecu;
float  frecuencia=0;
uint8_t flag_frec = 0;
char texto[20];

//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);
void READ_ROJO(void);
void VAL (uint16_t var);
void floattostr_(float numero_, unsigned char *cadena_,char decimales_);

void __interrupt()isr(void){
    di(); 
    if (PIR2bits.CCP2IF == 1){
        PORTA = 0X01;
        TMR1ON = 0;         //APAGAMOS EL TIMER1
        frecu=(CCPR2H << 8 ) | CCPR2L;
        flag_frec=1;  //habilita bandera de RPM
        TMR1L = 0X00;
        TMR1H = 0X00;
        PIR2bits.CCP2IF = 0;
        TMR1ON = 1;         //APAGAMOS EL TIMER1
       
    }
     
    ei();                           //POP
}

void main (void){
    setup();
    Lcd_Init();                     //INICIALIZAMOS LA LCD
    Lcd_Clear();  //Limpiar LCD
    Lcd_Set_Cursor(1,1); //cursor fila uno primera posicion 
    Lcd_Write_String("FRECUENCIA");
    TMR1ON = 1;
    while(1){
        void READ_ROJO(void);
        
        //__delay_ms(100);
        if (flag_frec==1){
            if (frecu!=0){
                frecuencia=8000000/(frecu*4.0); //convierte a Hercios
                frecu=0;
         }
            //else frecuencia=0;
            flag_frec=0;
        }      
        floattostr_(frecuencia,texto,2);
        Lcd_Set_Cursor(2,5);
        Lcd_Write_String(texto);
        PORTD++;

       // frecuencia=0;  //coloca en cero por si se quita la señal
        //__delay_ms(100);

    }
        
    }


//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0B0000000;          //PINES COMO DIGITALES
    ANSELH = 0X00;              //PINES COMO DIGITALES
    
    TRISA = 0B0000000;          //PORTA COMO OUTPUT
    TRISD = 0X00;               //PORTD COMO OUTPUT
    TRISE = 0X00;               //PORTE COMO OUTPUT
    TRISB = 0B00000000;         //PORTB COMO OUTPUT DE SELECTORES
    TRISC = 0X02;
    
    PORTA = 0X00;                //LIMPIAMOS EL PUERTOA
    PORTB = 0X00;                //LIMPIAMOS EL PUERTOB
    PORTC = 0X00;                //LIMPIAMOS EL PUERTOC
    PORTD = 0X00;                //LIMPIAMOS EL PUERTOD
    PORTE = 0X00;                //LIMPIAMOS EL PUERTOE
    
    //CONFIGURACION DEL OSCIALDOR
    OSCCONbits.IRCF2 = 1;        //OSCILADOR  DE 8 MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //RELOJ INTERNO 
    
     // EUSART_INIT(tx,rx);
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    PIE2bits.CCP2IE = 1;        //HABILITAMOS LAS INTERRUPCIONES DEL CCP
    PIR2bits.CCP2IF = 0;        //LIMPIAMOS LAS BANDERA DEL CCP
    
    
    //CONFIGUGACION DEL SENSOR TCS230
    PORTEbits.RE0 = 1;
    PORTEbits.RE1 = 1;
    
    //CONFIGURACION DEL TMR1
    T1CON = 0x00;            //CONFIGURACIONES DEL TMR1 (DESHABILITAMOS EL TMR1 ON)
    TMR1H = TMR1H_value;     //VALORES PARA 100 MS DEL TMR1
    TMR1L = TMR1L_value;
    
    //CONFIGURACION COMPARADOR
     CCP1CON = 0B00000101;       //CADA FALNCO DE SUBIDA
     
}
void VAL(uint16_t variable){        // Función para obtener valor decimal
    uint16_t valor;
    valor = variable;                  
    POS1 = (valor/100) ;                // VALOR DE CENTENAS
    valor = (valor - (POS1*100));
    POS2 = (valor/10);              // VALOR DE DECENAS
    valor = (valor - (POS2*10));
    POS3 = (valor);                // UNIDADES
    
    POS1 = POS1 + 48;          // PASARLO A VALORES ASCCII
    POS2 = POS2 + 48;
    POS3 = POS3 + 48;
    
}

void READ_ROJO(void){
    //CCP1CON = 0B00000101;
    PORTEbits.RE2 = 0;      //PARA LEER COLOR ROJO
    //PORTBbits.RB3 = 0;
    //PORTD = COLORH;
    //PORTA = COLORL;
}

 void floattostr_(float numero_, unsigned char *cadena_,char decimales_){
//variables temporales
int largo_entera,largo_n,cont_for,tempo_int;
double tempo_float;
//largo de la trama a armar en decimales
largo_n = decimales_+1;
largo_entera = 0;
// si es negativa coloca el -
if ( numero_ < 0)
  {
  *cadena_++ = '-';
  numero_ = -numero_;
  }
//si es menor que no multiplica por 10
if (numero_ > 0.0) while (numero_ < 1.0)
  {
  numero_ =numero_* 10.0;
  largo_entera--;
  }

  //realiza division varias veces hasta que sea menor que 10
while (numero_ >= 10.0)
  {
  numero_ = numero_/10.0;
  largo_entera++;   //sube el largo de la trama por ser deciaml
 }
 largo_n = largo_n+largo_entera;   //el largo es la decimal mas la parte entera

//round. numero_ is between 1 and 10 and largo_n will be printed to
// right of decimal point so rounding is ...
for (tempo_float = cont_for = 1; cont_for < largo_n; cont_for++)
  tempo_float = tempo_float/10.0;
numero_ += tempo_float/2.0;
if (numero_ >= 10.0) {numero_ = 1.0; largo_entera++;}
//si tiene decimales
if (largo_entera<0)
  {
   *cadena_++ = '0'; *cadena_++ = '.';
   if (largo_n < 0) largo_entera = largo_entera-largo_n;
   for (cont_for = -1; cont_for > largo_entera; cont_for--)  *cadena_++ = '0';
  }
for (cont_for=0; cont_for < largo_n; cont_for++)
  {
  tempo_int = numero_;
  *cadena_++ = tempo_int + 48;  //convierte a ascci
  if (cont_for ==  largo_entera ) *cadena_++ = '.';
  numero_ -= (tempo_float=tempo_int);
  numero_ = numero_*10.0;
  }
*cadena_ =0;    //anexa final de linea
}
