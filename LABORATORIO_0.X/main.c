/* 
 * File:   main.c
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 * LABORATORIO 0
 * Created on July 10, 2021, 12:23 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
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


#define TMR1H_value	0x3C;
#define TMR1L_value 0xB0;

//------------------------------VARIABLES---------------------------------------
int CONTADOR1 = 0;
int DISPLAY_valor = 3; 
int RA1_FLAG = 0;
int RA2_FLAG = 0;
int RA3_FLAG = 0;
int START_FLAG = 0;
int JUGADOR1 = 0;
int JUGADOR2 = 0;
int FLAG_J1 = 0;
int FLAG_J2 = 0;

//------------------------------TABLA-------------------------------------------
int digitos [10] = {
0B00111111, 0B00000110, 0B01011011, 0B01001111, 0B01100110, 0B01101101,
0B01111101, 0B00000111, 0B01111111, 0B01100111};

//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);
void display(int u);
//void cuenta_regresiva(int numero, int *u);
void secuencia (int numero);
int CONTADOR_DEC(int JUGADOR, int* FLAGJ);

//---------------------------INTERRUPCION--------------------------------------
void __interrupt()isr(void){
    di();                   //PUSH
    if (PIR1bits.TMR1IF == 1){
        CONTADOR1++;
        TMR1H = TMR1H_value;     //VALORES PARA 100 MS DEL TMR1
        TMR1L = TMR1L_value;
        if (CONTADOR1 >= 10){
            DISPLAY_valor--;
            CONTADOR1 = 0;
        }
        if (DISPLAY_valor == 0){
            T1CONbits.TMR1ON = 0;       //APAGAMOS EL TMR1    
        }
        display(DISPLAY_valor);
    }
    PIR1bits.TMR1IF = 0;
    ei();                           //POP
}

void main (void){
    setup();                        //FUNCION DE SETUP
    
    while(1){
        
        if (RA1 == 0 && RA1_FLAG == 1){//HASTA DEJAR DE SER PRESIONADO
            //RE1 = 1;
            T1CONbits.TMR1ON = 1;
            DISPLAY_valor = 3;
            PORTB = 0X00;
            PORTC = 0X00;
            START_FLAG = 0;
            FLAG_J1 = 0;
            FLAG_J2 = 0;
            JUGADOR1 = 0;
            JUGADOR2 = 0;
                   
        }
        RA1_FLAG = RA1;
       // cuenta_regresiva(CONTADOR1, &DISPLAY_valor);
        secuencia(DISPLAY_valor);
        while(START_FLAG == 1){
            
            if (RA2 == 0 && RA2_FLAG == 1 && FLAG_J2 == 0){//HASTA DEJAR DE SER PRESIONADO
                JUGADOR1++;
                PORTB = CONTADOR_DEC(JUGADOR1,&FLAG_J1);
            }
            RA2_FLAG = RA2;
            
            if (RA3 == 0 && RA3_FLAG == 1 && FLAG_J1 == 0){//HASTA DEJAR DE SER PRESIONADO
                JUGADOR2++;
                PORTC = CONTADOR_DEC(JUGADOR2, &FLAG_J2);
            }
            RA3_FLAG = RA3;
            
            if (FLAG_J1 == 1){
                PORTB= 0B11111111;
                START_FLAG = 0;
                DISPLAY_valor = 1;
                display(DISPLAY_valor);
            }
            if (FLAG_J2 == 1){
                PORTC= 0B11111111;
                START_FLAG = 0;
                DISPLAY_valor = 2;
                display(DISPLAY_valor);
            }
            
        }
    }
}

//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0X00;               //PINES COMO DIGITALES 
    ANSELH = 0X00;
    
    TRISA = 0B00011110;          //PORTA COMO OUTPUT
    TRISC = 0X00;               //PORTC COMO OUTPUT
    TRISD = 0X00;               //PORTD COMO OUTPUT
    TRISE = 0X00;               //PORTE COMO OUTPUT
    TRISB = 0X00;                //PORTB COMO OUTPUT
    
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
    
    //CONFIGURACION DEL TMR1
    T1CON = 0x10;            //CONFIGURACIONES DEL TMR1 (DESHABILITAMOS EL TMR1 ON)
    TMR1H = TMR1H_value;     //VALORES PARA 100 MS DEL TMR1
    TMR1L = TMR1L_value;
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    PIE1bits.TMR1IE = 1;        //HABILITAMOS BANDERA DEL TMR1
    PIR1bits.TMR1IF = 0;        //LIMPIAMOS BANDERA DEL TMR1
    
}

void display(int u){
    PORTAbits.RA0= 1;
    //CARGAMOS LOS VALORES DE UNIDADES AL DISPLAY
    PORTD = digitos[u];           
}

void secuencia(int numero){ //SECUENCIA DE SEMAFORO 
    switch (numero){
        case 3:
            if (CONTADOR1 == 1 || CONTADOR1 == 3 || CONTADOR1 == 5 || CONTADOR1 == 7 || CONTADOR1 == 9){
                PORTE = 0X07;}
            if (CONTADOR1 == 2 || CONTADOR1 == 4 || CONTADOR1 == 6 || CONTADOR1 == 8 || CONTADOR1 == 0){
                PORTE = 0X00;}
            break;
        case 2:
            PORTE = 0X04;
            break;
        case 1:
            PORTE = 0X02;
            break;
        case 0:
            PORTE = 0X01;
            START_FLAG = 1;
            break;
       
    }
}

int CONTADOR_DEC(int JUGADOR, int* FLAGJ){
    int PUERTO;
    switch (JUGADOR){
        case 0:
            PUERTO = 0X00;
            return PUERTO;
            break;
        case 1:
            PUERTO = 0B00000001;
            return PUERTO;
            break;
        case 2:
            PUERTO = 0B00000010;
            return PUERTO;
            break;
        case 3:
            PUERTO = 0B00000100;
            return PUERTO;
            break;
        case 4:
            PUERTO = 0B00001000;
            return PUERTO;
            break;
        case 5:
            PUERTO = 0B00010000;
            return PUERTO;
            break;
        case 6:
            PUERTO = 0B00100000;
            return PUERTO;
            break;
        case 7:
            PUERTO = 0B01000000;
            return PUERTO;
            break;
        case 8:
            PUERTO = 0B10000000;
            return PUERTO;
            break;
        case 9:
            //PUERTO = 0B11111111;
            *FLAGJ = 1;
            return PUERTO;
            break;
        return PUERTO;
    }
}
/*
void cuenta_regresiva(int numero, int *u){
    if (numero >= 10){
        *u = *u--;
        numero = 0;
    }
    if(*u == 0){
        T1CONbits.TMR1ON = 0;       //APAGAMOS EL TMR1
        RE1 = 0;
    }
}*/