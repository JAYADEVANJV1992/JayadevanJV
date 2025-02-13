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
char i,rfid[13];
int k=0;
char a1[]="0001531459",a2[]="0013947056",a3[]="0001502192",a4[]="0001495682";
void main()
{
    
    TRISD=0X00;
   
    PORTD=0X00;
     GIE = 1;  
    PEIE=1;
    //INTEDG =1; 
     RCIE=1;
      RCIF=0;   
     //INTE = 1; 
    UART1_Init();
    //UART2_Init();
    Lcd_Init();
    
    Lcd_Clear();
     
   //  GSM2_Init();
   Lcd_String(1,4," WELCOME");
  __delay_ms(2000);
  rfid[12] = '\0';
  while(1)
  {
      
    Lcd_Clear();
    Lcd_String(1,1,"Show your card");
     __delay_ms(200);
       if(k==1)
        {
           k=0;
            if(rfid[7]==a1[7]&&rfid[9]==a1[9]&&rfid[8]==a1[8])
            {
                rfid[0]='\0';
                Lcd_Clear();
             Lcd_Set_Cursor(2,1);
           Lcd_Write_String("RAGU");
           UART1_Write('1');
         
          __delay_ms(2000);
          
       
            }
            else if(rfid[7]==a2[7]&&rfid[9]==a2[9]&&rfid[8]==a2[8])
            {
                Lcd_Clear();
             Lcd_Set_Cursor(2,1);
           Lcd_Write_String("SIVA");
           UART1_Write('2');
          __delay_ms(2000);
          //goto s;
            }
            else if(rfid[7]==a3[7]&&rfid[9]==a3[9]&&rfid[8]==a3[8])
            {
                Lcd_Clear();
             Lcd_Set_Cursor(2,1);
           Lcd_Write_String("RAJ");
           UART1_Write('3');
          __delay_ms(2000);
            }
            else if(rfid[7]==a4[7]&&rfid[9]==a4[9]&&rfid[8]==a4[8])
            {
                Lcd_Clear();
             Lcd_Set_Cursor(2,1);
           Lcd_Write_String("KARU");
           UART1_Write('4');
          __delay_ms(2000);
            }
            else
            {
                
                 Lcd_Clear();
                Lcd_Set_Cursor(1,1);
           
           Lcd_Write_String("Invalid");
          __delay_ms(2000);
            }
        }
  }
   
  }




void interrupt ISR()             
{

    //if(RCIF==1)
    {
      //RCIF=0;
    //if(UART_Data_Ready())  
      {
        while(!RCIF);
        a=RCREG;
        {
    for(i=0;i<12;)
     {
       if(UART1_Data_Ready())
       {
         a = UART1_Read();
         rfid[i]=a;
         i++;
       }
     }
    k=1;
     
      RCIF=0;
   
     {
     
    
     }
    }
  RCIF=0;
    }
    }
}

