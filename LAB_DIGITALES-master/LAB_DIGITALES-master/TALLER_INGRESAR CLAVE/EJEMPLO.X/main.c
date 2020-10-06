/*
 * File:   main.c
 * Author: COMPUTADORA
 *
 * Created on 5 de mayo de 2018, 10:38 AM
 */

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

#define _XTAL_FREQ 4000000
#include <xc.h>
#include "lcd.h"
#include "teclado.h"
#include "eeprom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void) {
 INICIO:  
    TRISC = 0;
    TRISD = 0;
    
    char tecla = 0; //  GUARDAR LA TECLA PRESIONADA
    char inclave[5] = {0}; //  GUARDAR LOS DATOS INGRESADOS EN UN ARREGLO
    int countinclave = 0; //  CONTADOR PARA LA POSICION EN EL ARREGLO
    int clave1check = 0; //  VERIFICACION DE LA CLAVE DIGITO1
    int clave2check = 0; //  VERIFICACION DE LA CLAVE DIGITO2
    int clave3check = 0; //  VERIFICACION DE LA CLAVE DIGITO3
    int clave4check = 0; //  VERIFICACION DE LA CLAVE DIGITO4
    int clavechecktotal = 0; //  VERIFICACION DE LA CLAVE TOTAL
    char clave1 = '3'; //  CLAVE POR DEFECTO DIGITO1
    char clave2 = '5'; //  CLAVE POR DEFECTO DIGITO2
    char clave3 = '8'; //  CLAVE POR DEFECTO DIGITO3
    char clave4 = '9'; //  CLAVE POR DEFECTO DIGITO4
    char direccion1 = 0b0011; //  DIRECCION DE LA MEMORIA EEPROM
    char direccion2 = 0b0101; //  DIRECCION DE LA MEMORIA EEPROM
    char direccion3 = 0b1000; //  DIRECCION DE LA MEMORIA EEPROM
    char direccion4 = 0b1001; //  DIRECCION DE LA MEMORIA EEPROM

    int DERE[4] = {0b1100, 0b0110, 0b0011, 0b1001};
    int IZQ[4] = {0b1001, 0b0011, 0b0110, 0b1100};
    //  INICIALMENTE LOS DATOS EN LA MEMORIA ESTA CON 0xFF(255))
    if (eeprom_readx(direccion1) == 0xff) { // si la memoria esta vacia
        eeprom_writex(direccion1, clave1); // escribir clave1    
    }
    if (eeprom_readx(direccion2) ==0xff ) { // si la memoria esta vacia
        eeprom_writex(direccion2, clave2); // escribir clave2    
    }
    if (eeprom_readx(direccion3) == 0xff) { // si la memoria esta vacia
        eeprom_writex(direccion3, clave3); // escribir clave3    
    }
    if (eeprom_readx(direccion4) == 255) { // si la memoria esta vacia
        eeprom_writex(direccion4, clave4); // escribir clave3    
    }


    LCD_init(); //  inicia la configuracion del LCD
    teclado_init(); //  inicial la configuracion del TECLADO MATRICIAL

    LCD_gotoxy(2, 1);
    LCD_puts("INGRESAR CLAVE", 14);

    while (1) {
        tecla = teclado_getc(); //  RETORNA LA TECLA PRESIONADA
        if (tecla != 0) {

            //  SOLO LOS NUMEROS 
            if (tecla != 'A' && tecla != 'B' && tecla != 'C' && tecla != 'D' && tecla != '#' && tecla != '*') {

                LCD_gotoxy(2, 1);
                LCD_puts("INGRESAR CLAVE", 14);
                inclave[countinclave] = tecla; //  LA TECLA PRESIONADA SE ALMACENA EN EL ARREGLO
                countinclave++; //  INCREMENTA LA POSICION EN EL ARREGLO
                LCD_gotoxy(countinclave + 5, 2); //columna,fila
                //LCD_putc(tecla);
                LCD_puts("*",2);
                
                //LCD_putc(tecla);
                
            }
            

            //  SOLO LAS LETRAS
            if (tecla == '#') //  ENTER
            {
                if (eeprom_readx(direccion1) == inclave[0]) clave1check = 1;
                else clave1check = 0;
                if (eeprom_readx(direccion2) == inclave[1]) clave2check = 1;
                else clave2check = 0;
                if (eeprom_readx(direccion3) == inclave[2]) clave3check = 1;
                else clave3check = 0;
                if (eeprom_readx(direccion4) == inclave[3]) clave4check = 1;
                else clave4check = 0;

                clavechecktotal = clave1check + clave2check + clave3check+clave4check;

                if (clavechecktotal == 4) {
                    LCD_gotoxy(1, 1);
                    LCD_puts(" CLAVE CORRECTA ",16);
                    __delay_ms(1000);
                    LCD_clear();
                    LCD_gotoxy(2, 1);
                    LCD_puts(" *BIENVENIDO* ", 16);
                    __delay_ms(1000);
                    
                    PORTC =IZQ;
                    __delay_ms(1000);
                    PORTC = 0B00000001;
                    __delay_ms(1000);
                    PORTC = 0;
                    goto INICIO;
 

                } else {
                    
                    
                    LCD_gotoxy(1, 1);
                    LCD_puts("CLAVE INCORRECTA",16);
                    __delay_ms(10);
                    PORTDbits.RD3=1;
                    __delay_ms(2000);
                    PORTDbits.RD3=0;
                    __delay_ms(1000);
                    LCD_clear();
                    LCD_gotoxy(3, 1);
                    LCD_puts("**BLOQUEO**", 16);
                    __delay_ms(4000);
                    PORTC = 0B00000001;
                    __delay_ms(500);
                    PORTC = 0B0000001;
                    __delay_ms(500);
                    PORTC = 0B00000011;
                    __delay_ms(1000);
                    PORTC = 0;
                    LCD_clear();
                    LCD_gotoxy(2, 1);
                    LCD_puts("INGRESAR CLAVE",16);
                    
                    //  REINICIO DE TODAS LAS VARIABLES
                    goto INICIO;
                    /*clave1check = 0;
                     
                    clave2check = 0;
                    clave3check = 0;
                    clavechecktotal = 0;
                    countinclave = 0;*/

                }

            }
        }
    }

    return;
}
