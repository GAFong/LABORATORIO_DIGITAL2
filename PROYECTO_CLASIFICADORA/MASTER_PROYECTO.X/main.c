/* 
 * File:   main.c
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 * MASTER
 * Created on August 21, 2021, 5:17 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "EUSART_LIB.h"
#include "LCD.h"
#include "I2C.h"
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
#define tx 0
#define rx 1
#define ENTER 13
#define COMA 0x2C
#define tx 0
#define rx 1

//------------------------------VARIABLES---------------------------------------
uint8_t POS1;
uint8_t POS2;
uint8_t POS3;
uint8_t VALOR_PESO = 0;
uint8_t SENSOR_MOV = 0; 
uint8_t COLOR = 0;

//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);
void VAL (uint16_t var);

void __interrupt()isr(void){
    di(); 
    if (PIR1bits.RCIF){
        if (RCREG >= 65 && RCREG <= 67 ){
                
             switch (RCREG){
                case 65:
                    VAL(VALOR_PESO);
                    EUSART_ENVIAR(POS1);     //ENVIAMOS VALOR DE PESO Y SEPARADOS POR COMAS
                    __delay_us(200);
                    EUSART_ENVIAR(POS2);
                    __delay_us(200);
                    EUSART_ENVIAR(POS3);
                    __delay_us(200);
                    EUSART_ENVIAR(ENTER);
                    __delay_us(200);
                    break;
                case 66:
                     VAL(COLOR);
                     EUSART_ENVIAR(POS3);     //ENVIAMOS VALOR DE COLOR Y SEPARADOS POR COMAS
                    __delay_us(200);
                    break;
                case 67:
                    VAL(SENSOR_MOV);
                    EUSART_ENVIAR(POS3);     //ENVIAMOS VALOR DE COLOR Y SEPARADOS POR COMAS
                    __delay_us(200);
                    EUSART_ENVIAR(ENTER);
                    __delay_us(200);
                    break; 
        }
        }
     }
     
    ei();                           //POP
}

void main (void){
    setup();
    Lcd_Init();                     //INICIALIZAMOS LA LCD
    Lcd_Clear();  //Limpiar LCD
    Lcd_Set_Cursor(1,1); //cursor fila uno primera posicion 
    Lcd_Write_String("PESO: COLOR: BANDA:");
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x50);     //
        I2C_Master_Write(0X01);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();         //INICIALIZAMOS LA COMUNICACION
        I2C_Master_Write(0x51);     //ESCRIBIMOS A LA DIRECCION PARA LEER ESCLAVO DE PESO
        VALOR_PESO = I2C_Master_Read(0);
        I2C_Master_Stop();          //DETENEMOS LA COMUNICACION
        __delay_ms(200);
        
        I2C_Master_Start();         //INICIALIZAMOS LA COMUNICACION
        I2C_Master_Write(0x71);     //ESCRIBIMOS A LA DIRECCION PARA LEER ESCLAVO DE PESO
        COLOR = I2C_Master_Read(0);
        I2C_Master_Stop();          //DETENEMOS LA COMUNICACION
        __delay_ms(200);
        
        switch (COLOR){
            case 1:
                Lcd_Set_Cursor(2,7);        //COLOCAMOS COLOR EN LA LCD
                Lcd_Write_String("BLANCO");
                break;
            case 2:
                Lcd_Set_Cursor(2,7);        //COLOCAMOS COLOR EN LA LCD
                Lcd_Write_String("AZUL  ");
                break;
            case 3:
                Lcd_Set_Cursor(2,7);        //COLOCAMOS COLOR EN LA LCD
                Lcd_Write_String("ROJO  ");
                break;
                
        }
        if (VALOR_PESO != 0 && SENSOR_MOV == 1){
            PORTD = VALOR_PESO;
            VALOR_PESO = (VALOR_PESO);
            Lcd_Set_Cursor(2,1);        //COLOCAMOS VALORES DEL PESO EN LA LCD
            VAL(VALOR_PESO);                  //EXTRAER LOS VALORES DE TEMP
            Lcd_Write_Char(POS1);
            Lcd_Write_Char(POS2);
            Lcd_Write_Char(POS3);
            Lcd_Write_String("gr ");
            
          
        }
        if (SENSOR_MOV == 1){
            Lcd_Set_Cursor(2,14);
            Lcd_Write_String("ON ");
        }
        else {
            Lcd_Set_Cursor(2,14);
            Lcd_Write_String("OFF");
        }
       
        I2C_Master_Start();
        I2C_Master_Write(0x50);     //
        I2C_Master_Write(0X02);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();         //INICIALIZAMOS LA COMUNICACION
        I2C_Master_Write(0x51);     //ESCRIBIMOS A LA DIRECCION PARA LEER ESCLAVO DE PESO
        SENSOR_MOV = I2C_Master_Read(0);
        I2C_Master_Stop();          //DETENEMOS LA COMUNICACION
        __delay_ms(200);
        
        
        
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
    TRISB = 0X00;               //PORTB COMO OUTPUT LCD
  //  TRISC6 = 0;
 //   TRISC7 = 1;                 //TX
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
    //EUSART
    TRISC7 = 1;                 //TX
    EUSART_INIT(tx,rx);
    
    //CONFIGURACION I2C
    I2C_Master_Init(100000);        // INICIALIZAR MASTER A FRECUENCIA DE 100kHz
    
   // EUSART_INIT(tx,rx);
    INTCONbits.GIE = 1;         //DESHABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //DESHABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    
   
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