/* 
 * File:   ADC_LIB.h
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 *LIBRERIA ADC
 * Created on July 16, 2021, 5:55 PM
 */

#ifndef ADC_LIB_H
#define	ADC_LIB_H
#define _XTAL_FREQ 4000000

void ADC_INIT(int canal){
    //CONFIGURACION DEL ADC
    ADCON0bits.ADON = 0X01;     //ENCENDEMOS EL MODULO
    ADCON0bits.CHS = canal;      //ESCOGEMOS EL CANAL 0
    __delay_us(100);
    ADCON0bits.ADCS = 0X01;     //ADC CLOCK FOSC/8  
    ADCON1bits.ADFM = 0;        //JUSTIFICADO A LA IZQUIERDA
    ADCON1bits.VCFG0 = 0;       //VOLTAGE DE REFERENCIA EN VDD
    ADCON1bits.VCFG1 = 0;       //VOLTAGE DE REFERENCIA EN VSS
    PIE1bits.ADIE = 1;          //HABILITAMOS LA INTERRUPCION DEL ADC
    PIR1bits.ADIF = 0;
}
int ADC_READ(void){
    int LECT;
    LECT = ADRESH;
    return LECT;
}
/*
void ADC_CHANNELS(int channel,int next_channel){
    switch (channel){             //PODEMOS LEER LOS 13 CANALES
        case 0:
            //*out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 1:
           // *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 2:
           // *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 3:
          //  *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break; 
        case 4:
         //   *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 5:
         //   *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 6:
         //   *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 7:
         //   *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break; 
        case 8:
        //    *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 9:
         //   *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 10:
         //   *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 11:
         //   *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 12:
         //   *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break;
        case 13:
        //    *out = ADRESH;       //AGREGAMOS EL VALOR ADC A LA VARIABLE
            ADCON0bits.CHS = next_channel;//AGREGAMOS EL CANAL SI QUEREMOS CAMBIAR
            __delay_us(100);
            ADCON0bits.GO = 1;
            break; 
    }
}*/
#endif	/* ADC_LIB_H */

