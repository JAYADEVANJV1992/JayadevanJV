#define _XTAL_FREQ 20000000
#define RS RD1
#define EN RD0
#define RW RD2
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7


// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include <pic16f877a.h>
#include "pic16f87xa.h"

#include <stdio.h>
 char a1[]="00001531459",a2[]="00013947056",a3[]="00001502192",a4[]="00001495682";
   // char b1[]="RAGU",b2[]="00013947056",b3[]="00001502192",b4[]="00001495682";
char i,rfid[13];
void main()
{
   
    char a;
    TRISD=0X00;
   
    PORTD=0X00;
  
    UART1_Init();
    UART2_Init();
   UART3_Init();
    Lcd_Init();
     Lcd_Clear();
    
    GPRS1_Init();
    Lcd_Clear();
   Lcd_String(1,4," WELCOME");
  __delay_ms(2000);
  rfid[12] = '\0';
  while(1)
  {
     
    s:
     Lcd_Clear();
    Lcd_String(1,1,"Waiting...");
     __delay_ms(200);
            a = UART2_Read();
            if(a=='1')
            {
             Lcd_Clear();
             Lcd_Set_Cursor(1,1);
           Lcd_Write_String("STAFF: RAGU");
           GPS_Send3();
           GPRS1_Update(a1,b1,locate.gprs_location);
          __delay_ms(2000);
          goto s;
            }
            else if(a=='2')
            {
             Lcd_Clear();
             Lcd_Set_Cursor(1,1);
           Lcd_Write_String("STAFF: SIVA");
          __delay_ms(2000);
            }
            else if(a=='3')
            {
             Lcd_Clear();
             Lcd_Set_Cursor(1,1);
           Lcd_Write_String("STAFF: RAJ");
          __delay_ms(2000);
            }
            else if(a=='4')
            {
             Lcd_Clear();
             Lcd_Set_Cursor(1,1);
           Lcd_Write_String("STAFF: KARU");
          __delay_ms(2000);
            }
            else
            {
                goto s;
            }
     
       
  }
   
 

}



