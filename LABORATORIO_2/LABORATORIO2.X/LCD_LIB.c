/* 
 * File:   LCD_LIB.c
 * Author: DELL
 *
 * Created on July 26, 2021, 4:50 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "LCD_LIB.h"
/*
 * 
 */
void LCD_PORT(unsigned char a){ //COLOCAMOS VALOR EN CADA UNO DE LOS PINES
    if (a & 1){
        D0 = 1;}
    else {
        D0 = 0;}
    if (a & 2){
        D1 = 1;}
    else {
        D1 = 0;}
    if (a & 4){
        D2 = 1;}
    else {
        D2 = 0;}
    if (a & 8){
        D3 = 1;}
    else {
        D3 = 0;}
    if (a & 16){
        D4 = 1;}
    else {
        D4 = 0;}
    if (a & 32){
        D5 = 1;}
    else {
        D5 = 0;}
    if (a & 64){
        D6 = 1;}
    else {
        D6 = 0;}
    if (a & 128){
        D7 = 1;}
    else {
        D7 = 0;}
}
void Lcd_Cmd(char a) {   //
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    LCD_PORT(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}
void Lcd_Clear(void) {  //LIMPIAR LA LCD
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Init(void) {   //INICIALIZAR LA LCD
    LCD_PORT(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x30);
    __delay_ms(5);
    Lcd_Cmd(0x30);
    __delay_us(200);
    Lcd_Cmd(0x30);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x3C);
    Lcd_Cmd(0x08);
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x06);

}

void Lcd_Write_Char(char a) {
    
    char temp, y;
    temp = a;// & 0x0F;
    y = temp;// & 0xF0;
    RS = 1;// => RS = 1 es para que lea caracteres
    LCD_PORT(y);// >> 4); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Set_Cursor(char a, char b) {
char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1;
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
       
        Lcd_Cmd(y);
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4;
        y = temp & 0xF0; //esta linea cambie
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}

