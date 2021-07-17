/* 
 * File:   main.c
 * Author: GABRIEL ALEXANDE FONG PENAGOS
 * LABORATORIO 1
 * LIBRERIRAS E INTERRUPCIONES
 * Created on July 16, 2021, 12:25 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ADC_LIB.h"
#include "DISPLAY_LIB.h"

/*--------------------------CONFIGURACIONES ----------------------------------*/

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

//------------------------------VARIABLES---------------------------------------
int CONTADOR = 0;
int flag1 = 0;
int flag2 = 0;
int VALOR_ADC = 0;
int HEX1 = 0;
int HEX2 = 0;
int CANAL = 2;
int CANAL2 = 2;
int select = 1;

//------------------------------TABLA-------------------------------------------
int digitos [10] = {
0B00111111, 0B00000110, 0B01011011, 0B01001111, 0B01100110, 0B01101101,
0B01111101, 0B00000111, 0B01111111, 0B01100111};
/*
int TABLA_HEX[16] = { 
    0B00111111,    
    0B00000110,    
    0B01011011,    
    0B01001111,    
    0B01100110,    
    0B01101101,   
    0B01111101,    
    0B00000111,    
    0B01111111,    
    0B01100111,    
    0B01110111,    
    0B01111100,    
    0B00111001,    
    0B01011110,    
    0B01111001,    
    0B01110001,    
    };*/
//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);

//---------------------------INTERRUPCION--------------------------------------
void __interrupt()isr(void){
    di();                   //PUSH
    if  (T0IF == 1){
        TMR0 = _tmr0_value; //VALOR
        DISPLAY_HEX(&select,HEX1, HEX2);
        INTCONbits.T0IF = 0;            //LIMPIO LA BANDERA DE T0IF
    }
    if (RBIF == 1){
        if (RB0 == 0){              //REVISAMOS QUE RB0 FUE PRESIONADO
            flag1 = 1;
        }
        else{                       //SI NO ESTA PRESIONADO Y LA BANDERA LEVANTADA
            if (flag1 == 1) {
                CONTADOR++;            //INCREMENTAMOS LA VARIABLE
                flag1 = 0;              //LIMPIAMOS LA BANDERA
            }
        }
           
        if (RB1 == 0){              //REVISAMOS QUE RB1 FUE PRESIONADO
            flag2 = 1;
        }
        else {                      //SI NO ESTA PRESIONADO Y LA BANDERA LEVANTADA
            if(flag2 ==1){
                CONTADOR--;             //DECREMNTAMOS LA VARIABLE
                flag2 = 0;              //LIMPIAMOS LA BANDERA
            }
        }
     INTCONbits.RBIF = 0;           //LIMPIAMOS LA BANDERA DEL TMR0
    }
    if (ADIF == 1){
        VALOR_ADC = ADC_READ();
        PIR1bits.ADIF = 0;              //LIMPIAMOS LA BANDERA DEL ADC
    }
    ei();                           //POP
}
void main (void){
    setup();                        //FUNCION DE SETUP
    ADCON0bits.GO = 1;              //COMIENZA EL CICLO DEL ADC
    while(1){
        PORTC = CONTADOR;
        //void ADC_CHANNELS(2,CANAL2);
        __delay_us(100);
        ADCON0bits.GO = 1;
        BIN_HEX(VALOR_ADC, &HEX1, &HEX2);
       if (VALOR_ADC == CONTADOR){
            PORTE = 2;
        }
        else {
            PORTE = 0;
        }
    }
}

//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0B00000100;          //RA2 ANALOGICO 
    ANSELH = 0X00;              //PINES COMO DIGITALES
    
    TRISA = 0B00000100;          //PORTA COMO OUTPUT
    TRISC = 0X00;               //PORTC COMO OUTPUT
    TRISD = 0X00;               //PORTD COMO OUTPUT
    TRISE = 0X00;               //PORTE COMO OUTPUT
    TRISB = 0B00000011;         //RB0 RB1 COMO ENTRADA
    
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
    
    //CONFIGURACION DEL IOC
    WPUB0 = 1 ;                 //HABILITAMOS  LOS WEAK PULL UP RB0 RB1
    WPUB1 = 1 ;
    IOCB0 = 1 ;
    IOCB1 = 1 ;
    
    ADC_INIT(CANAL);
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    INTCONbits.T0IE = 1;        //HABILITAMOS BANDERA DEL TMR0
    INTCONbits.T0IF = 0;        //LIMPIAMOS BANDERA DEL TMR0
    INTCONbits.RBIE = 1;        //HABILITAMOS LAS INTERRUPCIONES IOC
    INTCONbits.RBIF = 0;        //LIMPIAMOS LA BANDER DE IOC
    
}
