/*
 * File:   TECLADO4x4.c
 * Author: jorge_
 *
 * Created on 30 de julio de 2020, 12:36 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "Bits_Congifuracion.h"
#include "LCD.h"
#include "KEYPAD4x4.h"

#define _XTAL_FREQ 8000000


unsigned char buffer_lcd[16];  // Buffer de entero 16 bits Almacena el formato de las variables a mostrar en la pantalla LCD.

void main(void) 
{
    ADCON1bits.PCFG=0b1111;         // Deshabilitamos las entradas analógicas, activamos Puerto A y B entradas/salidas digitales.
    lcd_init();
    Keypad_Init();                  // Inicializamos la librería KEYPAD4x4
    Boton_Presionado();
    while(1)
    {
        lcd_gotoxy(1,1);
        lcd_putc("JORGE");
        
    }
    return;
}
