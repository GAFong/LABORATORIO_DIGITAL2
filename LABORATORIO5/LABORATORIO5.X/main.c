/* 
 * File:   main.c
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 *PIC CON ADAFRUIT
 * Created on August 14, 2021, 1:51 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
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


#define _XTAL_FREQ 4000000
#define tx 0
#define rx 1
#define ENTER 13
#define COMA 0x2C
//------------------------------VARIABLES---------------------------------------
uint8_t CONTADOR;
uint8_t flag1 = 0;
uint8_t flag2 = 0;
uint8_t POS1;
uint8_t POS2;
uint8_t POS3;
uint8_t POS_TX = 0;
uint8_t POS_RX = 0;
uint8_t SEL = 0;
uint8_t DATO1 = 0;
uint8_t DATO2 = 0;
uint8_t DATO3 = 0;
//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);
void VAL (uint16_t var);

void __interrupt()isr(void){
    di();                   //PUSH
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
    if (PIR1bits.RCIF){
        if (RCREG >= 97 && RCREG <= 104 ){
                SEL = RCREG;  }         //AGREGAMOS EL VALOR PARA SELECCIONAR DEL EUSART
        else  {
            switch (POS_RX){
                case 0:
                    DATO1 = (RCREG-48)*100;     //CENTENAS
                    POS_RX = 1;
                    break;
                case 1:
                    DATO2 = (RCREG-48)*10;      //DECENAS
                    POS_RX = 2;
                    break;
                case 2:
                    DATO3 = (RCREG-48);         //UNIDADES
                    POS_RX = 0;
                    break; 
        }
        }
     }
    ei();                           //POP
}
void main (void){
    setup(); 
    while(1){
       
        VAL(CONTADOR);
        EUSART_ENVIAR(POS1);     //ENVIAMOS POSICION POR POSICION Y SEPARADOS POR COMAS
       __delay_us(200);
       EUSART_ENVIAR(COMA);
       __delay_us(200);
       EUSART_ENVIAR(POS2);
       __delay_us(200);
       EUSART_ENVIAR(COMA);
       __delay_us(200);
       EUSART_ENVIAR(POS3);
       __delay_us(200);
       EUSART_ENVIAR(COMA);
       __delay_us(200);
       EUSART_ENVIAR(ENTER);
        __delay_us(200);
       PORTD = DATO1 + DATO2 +DATO3;
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
    TRISB = 0X03;                //PORTB COMO OUTPUT
    
    PORTA = 0X00;                //LIMPIAMOS EL PUERTOA
    PORTB = 0X00;                //LIMPIAMOS EL PUERTOB
    PORTC = 0X00;                //LIMPIAMOS EL PUERTOC
    PORTD = 0X00;                //LIMPIAMOS EL PUERTOD
    PORTE = 0X00;                //LIMPIAMOS EL PUERTOE
    
    //CONFIGURACION DEL OSCIALDOR
    OSCCONbits.IRCF2 = 1;        //OSCILADOR  DE 8 MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;         //RELOJ INTERNO 
    
    OPTION_REGbits.nRBPU = 0;   //RBPU HABILITADO, PULL UPS
    //CONFIGURACION DEL IOC
    WPUB0 = 1 ;                 //HABILITAMOS  LOS WEAK PULL UP RB0 RB1
    WPUB1 = 1 ;
    IOCB0 = 1 ;
    IOCB1 = 1 ;
    EUSART_INIT(tx,rx);
    INTCONbits.GIE = 1;         //DESHABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //DESHABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    INTCONbits.RBIE = 1;        //HABILITAMOS LAS INTERRUPCIONES IOC
    INTCONbits.RBIF = 0;        //LIMPIAMOS LA BANDER DE IOC
   
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