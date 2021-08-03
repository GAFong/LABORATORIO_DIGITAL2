/* 
 * File:   main.c
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 *ESCLAVO, ADC, SPI
 * Created on July 29, 2021, 10:56 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "ADC_LIB.h"
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
#define CANAL0 0
#define CANAL1 1

//------------------------------VARIABLES---------------------------------------
unsigned int VALOR_ADC = 0;
uint8_t POT0 = 0;
uint8_t POT1 = 0;
uint8_t SEL_POT = 0;


//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);

//---------------------------INTERRUPCION--------------------------------------
void __interrupt()isr(void){
    di();                   //PUSH
     if (ADIF == 1){                    //INTERRUPCION DEL ADC
         
        VALOR_ADC = ADC_READ();
        PIR1bits.ADIF = 0;              //LIMPIAMOS LA BANDERA DEL ADC
    }
    if(SSPIF == 1){
        SEL_POT = spiRead();
        switch (SEL_POT){
            case 1:
                spiWrite(POT0);
                break;
            case 2:
                spiWrite(POT1);
                break;
        }
        
        SSPIF = 0;
    }
    
    ei();                           //POP
}

void main (void){
    setup();   
    while(1){
         ADC_CHANNELS(CANAL1,VALOR_ADC,&POT0);  //POT0
         __delay_ms(10);
         ADC_CHANNELS(CANAL0,VALOR_ADC,&POT1);   //POT1
         __delay_ms(10);
    }

}

//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0B00000011;          //RA0 RA1 ANALOGICO, RA5 ESCLAVO 
    ANSELH = 0X00;              //PINES COMO DIGITALES
    
    TRISA = 0B00100011;          //PORTA COMO OUTPUT
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
    

    
    ADC_INIT(CANAL0);           //INICIO DEL ADC
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    PIR1bits.SSPIF = 0;         // BORRAMOS BANDERA INTERRUPCION MSSP
    PIE1bits.SSPIE = 1;         // HABILITAMOS INTERRUPCION MSSP        

}

