/* 
 * File:   EUSART_LIB.h
 * Author: DELL
 *LIBRERIA PARA 8 MGHZ
 * Created on July 26, 2021, 6:05 PM
 */

#ifndef EUSART_LIB_H
#define	EUSART_LIB_H

#define _XTAL_FREQ      8000000 

void EUSART_INIT(unsigned int tx, unsigned int rx){ //Enable interrupt Tx y Rx
//CONFIG EUSART
    
  //TX CONFIG
  TXSTAbits.SYNC = 0;       //ASINCRONO
  TXSTAbits.BRGH = 0;       //HIGH SPEED
  TXSTAbits.TX9 = 0;         //ENVIO DE 8 BITS
  TXSTAbits.TXEN= 1;        //TX ENCENDIDO
  RCSTAbits.SPEN = 1;       //SPEN ACTIVADO
  
  //RX CONFIG
  RCSTAbits.RX9 = 0;        //RECIBIDO DE 8 BITS
  RCSTAbits.CREN = 1;       //CREN ACTIVADO
  
  //BAUD RATE CONTROL
    BAUDCTLbits.BRG16 = 0;  
    SPBRG =12;              //SEÑAL DE 9600
    SPBRGH = 0;
   
    PIE1bits.TXIE =tx;      //INTERRUPCIONES
    PIE1bits.RCIE =rx;
}

void EUSART_ENVIAR(uint8_t dato){
    if (PIR1bits.TXIF){
        TXREG = dato;
        __delay_ms(10);}
}
#endif	/* EUSART_LIB_H */

