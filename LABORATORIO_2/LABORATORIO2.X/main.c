/* 
 * File:   main.c
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 *LABORATORIO 2
 * Created on July 24, 2021, 8:27 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ADC_LIB.h"

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
//------------------------------VARIABLES---------------------------------------
unsigned int VALOR_ADC = 0;
unsigned int POT0 = 0;
unsigned int POT1 = 0;
//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);

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
    
    ei();                           //POP
}

void main (void){
    setup();                        //FUNCION DE SETUP
    ADCON0bits.GO = 1;              //COMIENZA EL CICLO DEL ADC
    while(1){
        ADC_CHANNELS(CANAL0,VALOR_ADC,&PORTC,&PORTD);   //POT0

        ADC_CHANNELS(CANAL1,VALOR_ADC,&PORTC,&PORTD);  //POT1
  
    }
}
//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0B00000011;          //RA2 ANALOGICO 
    ANSELH = 0X00;              //PINES COMO DIGITALES
    
    TRISA = 0B00000011;          //PORTA COMO OUTPUT
    TRISC = 0X00;               //PORTC COMO OUTPUT
    TRISD = 0X00;               //PORTD COMO OUTPUT
    TRISE = 0X00;               //PORTE COMO OUTPUT
    TRISB = 0B00000000;         //RB0 RB1 COMO ENTRADA
    
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
    OPTION_REG = 0B01010100;    //RBPU HABILITADO, PSA (0) PRESCALER 1:32
    TMR0 = _tmr0_value;         //TMR0 A 5 ms
    
    ADC_INIT(CANAL0);
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    INTCONbits.T0IE = 1;        //HABILITAMOS BANDERA DEL TMR0
    INTCONbits.T0IF = 0;        //LIMPIAMOS BANDERA DEL TMR0
}

