
#include "mcc_generated_files/mcc.h"
#include	<ctype.h>
#include	<stdlib.h>



//CONEXIONES DEL LCD

#define RS PORTBbits.RB7
#define EN PORTBbits.RB6
#define D4 PORTBbits.RB5
#define D5 PORTBbits.RB4
#define D6 PORTBbits.RB3
#define D7 PORTBbits.RB2

#include "lcd.h"


#define led  LATC0
#define led1 LATA0



unsigned char tecla;
char texto[20];
unsigned int retardo=100;
char retardo_cad[10];

enum teclado_estado {n1,n2,n3,n4,en};
char estado_teclado=n1;

const unsigned char password[5]= {'7', '8', '9', '0', 0};

 void Keypad_Init()
 {
 TRISD=0B00001111; //inica el puerto del teclado
 //TRISD = 0x0F;
  }

unsigned char Keypad_Key_Press(void)
{
 const unsigned char keypad_deco[17]=   
 {'7', '8', '9', '/', '4', '5', '6', '*', '1', '2', '3', '-', 13, '0', '=', '+', 0};
 
    unsigned int tec = 0, fila;
 
    
    for (fila = 0b00000001; fila < 0b00010000; fila <<= 1)
    {
        LATD =fila<<4;  //lo traslada 4  posicones para sacar por las filas
        if (PORTDbits.RD0== 1) break; tec++;
        if (PORTDbits.RD1 == 1) break; tec++;
        if (PORTDbits.RD2 == 1) break; tec++;
        if (PORTDbits.RD3 == 1) break; tec++;
    }
    PORTD = 0x00;
    return keypad_deco[tec];//retorna el codigo deseao de la tecla
}


uint16_t keypad_read()
{
	unsigned char count = 0;
	unsigned char bitmask = 0x10;
	// Valor de retorno de la funcion keypad_read() entero de 16 bits
	uint16_t keys =0;
	// Revisamos si alguna de las lineas en la parte baja esta en estado alto
	// debido a que se ha cerrado un switch en el teclado.
	if( PORTD & 0x0F )
	{
		// Escaneamos las filas una por una para determinar la tecla oprimida
		do
		{
			PORTD &= 0x0F; // Apagamos todas las filas (parte alta)
			TRISD &= ~bitmask; // Seleccionamos solo una como salida (bit del registro tris = 0)
			PORTD |= bitmask; // Y colocamos el pin en estado alto
            __delay_ms(100);
			//delay_ms(100); // Esperamos unos cuantos milisegundos y leemos las columnas
			keys |= (PORTD & 0x0F); // Guardamos las columnas en una variable de 16 bits
			if( count == 3 ) // Si ya hemos leido 4 columnas, las siguientes instrucciones no son necesarias, por lo que salimos del ciclo
				break;
			keys = keys<<4; // Recorremos la lectura a la izquierda para hacer espacio
			bitmask = bitmask<<1; // Recorremos el bitmask de la fila a leer
			count ++; // Incrementar el contador de filas
		} while(1);

		TRISD = 0x0F;	// Mantenemos parte alta = salidas y parte baja = entradas
		PORTD = 0xF0; // Mantenemos los bits mas significativos del puerto en estado alto
		return keys;
	}
    
	return 0;
}


/*
                         Main application
 */


void teclado_estado()
{
    tecla=Keypad_Key_Press();
    
    switch (estado_teclado)
    {
      case n1:

              if ( (tecla!=0)  && (isdigit(tecla)))
              {
              led1=1;
              retardo_cad[0]=tecla;
              estado_teclado=n2;
              Lcd_Set_Cursor(2,8);   
              Lcd_Write_Char(tecla);
              __delay_ms(400); // retardo antirrebote
              }
            break;
            

      case n2:
              if ((tecla!=0)  && (isdigit(tecla))) //isdigit=si es digito
              {
              retardo_cad[1]=tecla;
              estado_teclado=n3;
              Lcd_Set_Cursor(2,9);
              Lcd_Write_Char(tecla);
              __delay_ms(400); // retardo antirrebote
              }
            break;


      case n3:

              if ((tecla!=0)  && (isdigit(tecla)))
              {
              retardo_cad[2]=tecla;
              estado_teclado=n4;
              Lcd_Set_Cursor(2,10);
              Lcd_Write_Char(tecla);
              __delay_ms(400); // retardo antirrebote
              }
            break;


      case n4:
              if ((tecla!=0)  && (isdigit(tecla)))
              {
              retardo_cad[3]=tecla;
              estado_teclado=en;
              Lcd_Set_Cursor(2,11);
              Lcd_Write_Char(tecla);
              __delay_ms(400); // retardo antirrebote

              }
            break;

        case en:

               if ((tecla!=0)  && (tecla!='='))  // no se presiono el igual
              {
              led1=0;
              estado_teclado=n1; // vuelve al estado de  captura del primer numero
             Lcd_Set_Cursor(2,8);
               Lcd_Write_String("      ");// borra la pantalla
             __delay_ms(600); // retardo antirrebote


              }
               // si si se presiono el enter
              else if ((tecla!=0)  && (tecla=='='))  //si se presiono el igual
              {
              led1=0;
             retardo_cad[4]=0;  //final de cadena
              retardo=atoi(retardo_cad); // lo pasa a numero y calcula el retardo deseado
              estado_teclado=n1; // vuelve al estado de caprura del primer numero.
             Lcd_Set_Cursor(2,8);
             Lcd_Write_String("      ");// borra la pantalla
             __delay_ms(600); // retardo antirrebote
              }

              break;


    
    }// fin del switch


}

void miretardo(signed int valor)
{

while (valor>0)
{ 
   __delay_us(700);
     teclado_estado();
     valor--;
 }

}
void main(void)
{
 
    const unsigned char keypad_deco[17]=   
 {'7', '8', '9', '/', '4', '5', '6', '*', '1', '2', '3', '-', 13, '0', '=', '+', 0};
    
    SYSTEM_Initialize();



 
  ANSELA=0;
  ANSELB=0;
  ANSELC=0;
  ANSELD=0;
  ANSELE=0;
  
  Keypad_Init();
  
  TRISB=0; //SALIDA PINES DEL lcd
  
      Lcd_Init();
       Lcd_Clear();
        Lcd_Set_Cursor(1,6);
        Lcd_Write_String("TECLADO");
        
   //LOS LEDS     
       TRISC0=0; //salida led
       TRISA=0; //salida led1
       //PORTA = 0;
              
       led=1;
       led1=0;

    while (1)
    {
        // Add your application code
        tecla= keypad_read();
        
        //PORTA = tecla;
        
       tecla= Keypad_Key_Press();
       if (tecla!=0)
       {
        Lcd_Set_Cursor(2,8);   
        Lcd_Write_Char(keypad_deco[tecla]);
        __delay_ms(400);
        
    }
        
       led=~led;
       
       //miretardo(retardo); 
        
}
       
}
