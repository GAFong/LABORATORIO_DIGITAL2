/* 
 * File:   LCD_LIB.h
 * Author: GABRIEL FONG
 *
 * Created on July 26, 2021, 4:44 PM
 */

#ifndef LCD_LIB_H
#define	LCD_LIB_H


#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif


#ifndef RS
#define RS PORTEbits.RE0
#endif

#ifndef EN
#define EN PORTEbits.RE1
#endif

#ifndef D0
#define D0 PORTDbits.RD0
#endif

#ifndef D1
#define D1 PORTDbits.RD1
#endif

#ifndef D2
#define D2 PORTDbits.RD2
#endif

#ifndef D3
#define D3 PORTDbits.RD3
#endif

#ifndef D4
#define D4 PORTDbits.RD4
#endif

#ifndef D5
#define D5 PORTDbits.RD5
#endif

#ifndef D6
#define D6 PORTDbits.RD6
#endif

#ifndef D7
#define D7 PORTDbits.RD7
#endif

#include <xc.h> 

void LCD_PORT(char a);

void Lcd_Cmd(char a); 

void Lcd_Clear(void); 

void Lcd_Init(void); 

void Lcd_Write_Char(char a);

void Lcd_Write_String(char *a); 

void Lcd_Set_Cursor(char a, char b);

void Lcd_Shift_Right(void);

void Lcd_Shift_Left(void);


#endif	/* LCD_LIB_H */

