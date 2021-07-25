/* 
 * File:   ADC_LIB.h
 * Author: GABRIEL ALEXANDER FONG PENAGOS
 *LIBRERIA ADC
 * Created on July 16, 2021, 5:55 PM
 */

#ifndef ADC_LIB_H
#define	ADC_LIB_H
#define _XTAL_FREQ 4000000

void ADC_INIT(unsigned int canal){
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
unsigned int ADC_READ(void){
    unsigned int LECT;
    LECT = ADRESH;
    return LECT;
}
void ADC_CHANNELS(unsigned int CANAL, unsigned int VALOR_ADC, unsigned int* VALOR0, unsigned int* VALOR1){
    //CANAL AL QUE QUEREMOS CAMBIAR, LECTURA DEL ADC, VARIABLE 1, VARIABLE 2,ADCON0bits.CHS 
    switch (ADCON0bits.CHS){
        case 0:
            *VALOR0 = VALOR_ADC;
            ADCON0bits.CHS = CANAL;
            __delay_us(50);
            ADCON0bits.GO = 1;
            break;
        case 1:
            *VALOR1 = VALOR_ADC;
            ADCON0bits.CHS = CANAL;
            __delay_us(50);
            ADCON0bits.GO = 1;
            break;
    }
    
    
}
#endif	/* ADC_LIB_H */