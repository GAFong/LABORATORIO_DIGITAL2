/* 
 * File:   main.c
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 *ESCLAVO I2C, ADC
 * Created on August 4, 2021, 10:48 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "ADC_LIB.h"
#include "I2C.h"


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
#define CANAL0 0

//------------------------------VARIABLES---------------------------------------
uint8_t VALOR_ADC = 0;
uint8_t z;
//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);

//---------------------------INTERRUPCION--------------------------------------
void __interrupt()isr(void){
    di();                   //PUSH
     if (ADIF == 1){                    //INTERRUPCION DEL ADC
         
        VALOR_ADC = ADC_READ();
        
        PIR1bits.ADIF = 0;              //LIMPIAMOS LA BANDERA DEL ADC
    }
       if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // LEEMOS EL VALOR DEL BUFFER Y AGREGAMOS A UNA VARIABLE
            SSPCONbits.SSPOV = 0;       // LIMPIAMOS LA BANDERA DE OVERFLOW
            SSPCONbits.WCOL = 0;        // LIMPIAMOS EL BIT DE COLISION
            SSPCONbits.CKP = 1;         // HABILITAMOS SCL
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // LEEMOS EL VALOR DEL BUFFER Y AGREGAMOS A UNA VARIABLE
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // LIMPIAMOS BANDERA DE INTERUPCION RECEPCION/TRANSMISION SSP
            SSPCONbits.CKP = 1;         // HABILITA LOS PULSOS DEL RELOJ SCL
            while(!SSPSTATbits.BF);     // HASTA QUE LA RECEPCION SE REALICE
           // PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = VALOR_ADC;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
    
    ei();                           //POP
}

void main (void){
    setup(); 
    ADCON0bits.GO = 1;
    while(1){
        PORTB = VALOR_ADC;
        ADC_CHANNELS(CANAL0,VALOR_ADC,&VALOR_ADC);  //POT0
        __delay_ms(10);
    }
}

//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0B0000001;          //RA0 ANALOGICO
    ANSELH = 0X00;              //PINES COMO DIGITALES
    
    TRISA = 0B0000001;          //RA0 INPUT
    TRISD = 0X00;               //PORTD COMO OUTPUT
    TRISE = 0X00;               //PORTE COMO OUTPUT
    TRISB = 0X00;         //PORTB COMO OUTPUT
    
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
    
    ADC_INIT(CANAL0);           //INICIO DEL ADC
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    I2C_Slave_Init(0x70);       //DIRECCION DEL I2C ESCLAVO
}