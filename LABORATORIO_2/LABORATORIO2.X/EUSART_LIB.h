/* 
 * File:   EUSART_LIB.h
 * Author: DELL
 *
 * Created on July 26, 2021, 6:05 PM
 */

#ifndef EUSART_LIB_H
#define	EUSART_LIB_H

#define _XTAL_FREQ      4000000 

void EUSART_INIT(unsigned int tx, unsigned int rx){ //Enable interrupt Tx y Rx
//CONFIG EUSART
    
  //TX CONFIG
  TXSTAbits.SYNC = 0;       //ASINCRONO
  TXSTAbits.BRGH = 1;       //HIGH SPEED
  TXSTAbits.TX9 = 0;         //ENVIO DE 8 BITS
  TXSTAbits.TXEN= 1;        //TX ENCENDIDO
  RCSTAbits.SPEN = 1;       //SPEN ACTIVADO
  
  //RX CONFIG
  RCSTAbits.RX9 = 0;        //RECIBIDO DE 8 BITS
  RCSTAbits.CREN = 1;       //CREN ACTIVADO
  
  //BAUD RATE CONTROL
    BAUDCTLbits.BRG16 = 0;  
    SPBRG =25;              //SEÑAL DE 9600
    SPBRGH = 1;
   
    PIE1bits.TXIE =tx;      //INTERRUPCIONES
    PIE1bits.RCIE =rx;
}


#endif	/* EUSART_LIB_H */

