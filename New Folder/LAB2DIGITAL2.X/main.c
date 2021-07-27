/* 
 * File:   main.c
 * Author: DELL
 *
 * Created on July 26, 2021, 6:37 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "ADC_LIB.h"
#include "LCD.h"
#include "EUSART_LIB.h"


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


#define _tmr0_value 100 
#define _XTAL_FREQ 4000000

#define CANAL0 0
#define CANAL1 1
#define ENTER 13
#define PUNTO 46
#define v 118
#define tx 0
#define rx 1

//------------------------------VARIABLES---------------------------------------
unsigned int VALOR_ADC = 0;
uint16_t POT0 = 0;
uint16_t POT1 = 0;
uint8_t POS1;
uint8_t POS2;
uint8_t POS3;
uint8_t DATO;
uint8_t CONTADOR = 0;
uint8_t OPCION;
unsigned char S1[8] = "SENSOR1:";
unsigned char S2[8] = "SENSOR2:";
//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);
void VAL (uint16_t var);

//---------------------------INTERRUPCION--------------------------------------
void __interrupt()isr(void){
    di();                   //PUSH
     if (ADIF == 1){                    //INTERRUPCION DEL ADC
         
        VALOR_ADC = ADC_READ();
        PIR1bits.ADIF = 0;              //LIMPIAMOS LA BANDERA DEL ADC
    }
    if  (T0IF == 1){
        TMR0 = _tmr0_value; //VALOR
        INTCONbits.T0IF = 0;            //LIMPIO LA BANDERA DE T0IF
    }
    if (PIR1bits.RCIF){
        if (RCREG == 43 || RCREG == 45 ){
                OPCION = RCREG; 
            
        }
    }
    ei();                           //POP
}

void main (void){
    setup();                        //FUNCION DE SETUP
    Lcd_Init();                     //INICIALIZAMOS LA LCD
    Lcd_Clear();  //Limpiar LCD
    Lcd_Set_Cursor(1,1); //cursor fila uno primera posicion 
    Lcd_Write_String("S1:   S2:    S3:");
    ADCON0bits.GO = 1;              //COMIENZA EL CICLO DEL ADC
    while(1){
       
          ADC_CHANNELS(CANAL1,VALOR_ADC,&POT0);  //POT0
          POT0= POT0*1.961;
        VAL(POT0);
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(POS1);
        Lcd_Write_Char(PUNTO);
        Lcd_Write_Char(POS2);
        Lcd_Write_Char(POS3);
        
        for(DATO=0; DATO <= 9; DATO++){
             TXREG = S1[DATO];
             __delay_ms(10);
             }
       // EUSART_ENVIAR(ENTER);
        EUSART_ENVIAR(POS1);
        EUSART_ENVIAR(PUNTO);
        EUSART_ENVIAR(POS2);
        EUSART_ENVIAR(POS3);
        EUSART_ENVIAR(v);
        EUSART_ENVIAR(ENTER);
        
        Lcd_Write_String("v ");
        ADC_CHANNELS(CANAL0,VALOR_ADC,&POT1);   //POT1
        POT1= POT1*1.961;
        VAL(POT1);
        Lcd_Write_Char(POS1);
        Lcd_Write_Char(PUNTO);
        Lcd_Write_Char(POS2);
        Lcd_Write_Char(POS3);
        Lcd_Write_String("v   ");
        for(DATO=0; DATO <= 9; DATO++){
             TXREG = S2[DATO];
             __delay_ms(10);
             }
      //  EUSART_ENVIAR(ENTER);
        EUSART_ENVIAR(POS1);
        EUSART_ENVIAR(PUNTO);
        EUSART_ENVIAR(POS2);
        EUSART_ENVIAR(POS3);
        EUSART_ENVIAR(v);
        EUSART_ENVIAR(ENTER);
        
        switch(OPCION){
            case 43:
                CONTADOR++;
                if (CONTADOR == 100){
                    CONTADOR = 0;
                }
                OPCION = 0;
                break;
            case 45:
                if (CONTADOR == 0){
                    CONTADOR = 0X63;
                }
                else {
                CONTADOR--;}
                OPCION = 0;
                break;    
        }
        VAL(CONTADOR);
        Lcd_Write_Char(POS2);
        Lcd_Write_Char(POS3);
        
    }
}
//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0B00000011;          //RA2 ANALOGICO 
    ANSELH = 0X00;              //PINES COMO DIGITALES
    
    TRISA = 0B00000011;          //PORTA COMO OUTPUT
    TRISC = 0B10000000;          //PORTC COMO OUTPUT
    TRISD = 0X00;               //PORTD COMO OUTPUT
    TRISE = 0X00;               //PORTE COMO OUTPUT
    TRISB = 0B00000000;         //PORTB COMO OUTPUT
    
    PORTA = 0X00;                //LIMPIAMOS EL PUERTOA
    PORTB = 0X00;                //LIMPIAMOS EL PUERTOB
    PORTC = 0X00;                //LIMPIAMOS EL PUERTOC
    PORTD = 0X00;                //LIMPIAMOS EL PUERTOD
    PORTE = 0X00;                //LIMPIAMOS EL PUERTOE
    
    //CONFIGURACION DEL OSCIALDOR
    OSCCONbits.IRCF2 = 1;        //OSCILADOR  DE 4 MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;         //RELOJ INTERNO 
    
    //CONFIGURACION DEL TMR0
    OPTION_REG = 0B11010100;    //RBPU DESHABILITADO, PSA (0) PRESCALER 1:32
    TMR0 = _tmr0_value;         //TMR0 A 5 ms
    
    ADC_INIT(CANAL0);
    EUSART_INIT(tx,rx);
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    INTCONbits.T0IE = 1;        //HABILITAMOS BANDERA DEL TMR0
    INTCONbits.T0IF = 0;        //LIMPIAMOS BANDERA DEL TMR0
}

void VAL(uint16_t variable){        // Función para obtener valor decimal
    uint16_t valor;
    valor = variable;                  
    POS1 = (valor/100) ;                // VALOR DE CENTENAS
    valor = (valor - (POS1*100));
    POS2 = (valor/10);              // VALOR DE DECENAS
    valor = (valor - (POS2*10));
    POS3 = (valor);                // UNIDADES
    
    POS1 = POS1 + 48;          // PASARLO A VALORES ASCCI
    POS2 = POS2 + 48;
    POS3 = POS3 + 48;
    
}
