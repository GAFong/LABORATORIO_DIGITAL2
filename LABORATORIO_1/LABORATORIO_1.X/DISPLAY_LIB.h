/* 
 * File:   DISPLAY_LIB.h
 * Author: GABRIEL ALEXANDER FONG PENAGOS 
 *
 * Created on July 16, 2021, 9:43 PM
 */

#ifndef DISPLAY_LIB_H
#define	DISPLAY_LIB_H
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
    };

void DISPLAY_HEX(int* select, int VHEX1, int VHEX2){
    
    switch(*select){
        case 1:                     //VALORES PARA LA PRIMERA POS EN HEX
            PORTD = TABLA_HEX[VHEX1];
            PORTA = 0B00000001;
            *select = 2;
            break;
        case 2:
            PORTD = TABLA_HEX[VHEX2];
            PORTA = 0B00000010;
            *select = 1;
            break;
    }
}
void BIN_HEX(int VBIN, int* VHEX1, int* VHEX2){
    *VHEX1 = VBIN & 0X0F;       //OBTENEMOS EL PRIMER NIBBLE
    *VHEX2 = (VBIN & 0XF0)/16;  /*OBTENEMOS EL SEGUNDO NIBLLE Y LO CORREMOS 4 
                                 * ESPACIOSCADA CORRIMIENTO ES UN /2        */
}                           
#endif	/* DISPLAY_LIB_H */

