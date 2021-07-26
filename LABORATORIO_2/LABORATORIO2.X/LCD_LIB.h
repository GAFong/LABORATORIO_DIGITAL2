/* 
 * File:   LCD_LIB.h
 * Author: DELL
 *
 * Created on July 26, 2021, 4:44 PM
 */

#ifndef LCD_LIB_H
#define	LCD_LIB_H


#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif


#ifndef RS
#define RS RB2
#endif

#ifndef EN
#define EN RB3
#endif

#ifndef D0
#define D0 RD0
#endif

#ifndef D1
#define D1 RD1
#endif

#ifndef D2
#define D2 RD2
#endif

#ifndef D3
#define D3 RD3
#endif

#ifndef D4
#define D4 RD4
#endif

#ifndef D5
#define D5 RD5
#endif

#ifndef D6
#define D6 RD6
#endif

#ifndef D7
#define D7 RD7
#endif

#include <xc.h> 

void LCD_PORT(unsigned char a);
void Lcd_Cmd(char a); 
void Lcd_Clear(void); 
void Lcd_Init(void); 
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a); 
void Lcd_Set_Cursor(char a, char b);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);


#endif	/* LCD_LIB_H */

