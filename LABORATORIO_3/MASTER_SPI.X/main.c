/* 
 * File:   main.c
 * Author: GABIREL ALEXANDER FONG PENAGOS
 *MASTER, SPI, EUSART
 * CODIGO INTERFAZ
 * python -m PyQt5.uic.pyuic -x SPI_INT.ui -o SPI_INT.py 
 * Created on July 29, 2021, 3:18 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "EUSART_LIB.h"
#include "SPI.h"

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
#define ENTER 10
#define COMA 0x2C

//------------------------------VARIABLES---------------------------------------
uint16_t POT0 = 0;
uint16_t POT1 = 0;
uint8_t SEL_POT = 0;
uint16_t valor;
uint8_t POS1;
uint8_t POS2;
uint8_t POS3;
uint8_t POS_TX = 0;
//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);
void VAL (uint16_t var);
uint8_t ENVIO(void);
//---------------------------INTERRUPCION--------------------------------------
void __interrupt()isr(void){
    di();                   //PUSH
     
    
    
    ei();                           //POP
}

void main (void){
    setup();   
    while(1){
        PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       spiWrite(1);
       POT0 = spiRead();
       PORTD = POT0;
       __delay_ms(1);
       POT0= POT0*1.961;
       VAL(POT0);
       //EUSART_ENVIAR(49);
      // __delay_ms(10);
      // TXREG = 49;
      // __delay_ms(50);
      
       EUSART_ENVIAR(POS1);
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
       spiWrite(2);
       POT1 = spiRead();
       __delay_ms(1);
       PORTB = POT1;
       POT1= POT1*1.961;
       VAL(POT1);
       EUSART_ENVIAR(POS1);
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
       
      // __delay_ms(1);
      // PORTCbits.RC2 = 1;       //Slave Deselect
         
    }

}

//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0B00000000;          //RA0 RA1 ANALOGICO, RA5 ESCLAVO 
    ANSELH = 0X00;              //PINES COMO DIGITALES
    
    TRISA = 0B00000000;          //PORTA COMO OUTPUT, RA5 
    TRISC2 = 0;
    TRISC7 = 1;                 //TX
    PORTCbits.RC2 = 1;          //ESCLAVO DESHABILITADO 
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
    
    EUSART_INIT(tx,rx);
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
     
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

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

uint8_t ENVIO (void){
    
    switch (POS_TX){
        case 0:
            POS_TX = 1;
            return POS1;   //MANDAMOS EL VALOR MAPEADO A TXREG DEL POT0
            break;
        case 1:
            POS_TX = 2;
            return POS2;    //MANDAMOS UNA COMA
            break;
        case 2:
            POS_TX = 3;
            return POS3;   //MANDAMOS EL VALOR MAPEADO A TXREG DEL POT1
            break;
       
        case 3:
            POS_TX = 0;     
            return 13;    //MANDAMOS ENTER
            break;    
    }
}