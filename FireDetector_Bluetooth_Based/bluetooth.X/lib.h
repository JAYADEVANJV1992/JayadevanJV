/* 
 * File:   lib.h
 * Author: SPIRO
 *
 * Created on February 27, 2018, 4:47 PM
 */

#ifndef LIB_H
#define	LIB_H

#ifdef	__cplusplus
extern "C" {
#endif
    #include<stdio.h>  
  #define _XTAL_FREQ 20000000
char AT[]="AT";   // To initialize mode
 char noecho[]="ATE0";   // To stop echo
 char mode_text[]="AT+CMGF=1";   // to set text mode
 char char_mode[]="AT+CSCS=\"GSM\"\r";   // to set character mode
 char param[]="AT+CSMP=17,167,0,0";   // set the parameter of character
// char mobile_no[]="AT+CMGS=\"+919747154363\"";   //use to set receinpent number and mesg
 char mesg[]="hHHHH";

void Lcd_Port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}
void Lcd_Cmd(char a)
{
	RS = 0;  
    RW=0;// => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;             // => E = 0
}

Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

void Lcd_Init()
{
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
	__delay_ms(5);
  Lcd_Cmd(0x03);
	__delay_ms(11);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;     
   RW=0;// => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
}

void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}


  void UART_Init(void)
    {
         TRISC7 = 1;
        TRISC6 = 0;
        TXSTA=0X24;
        BRGH=1;
        RCSTA=0X90;
        TXEN=1;
        SPEN=1;
        SYNC = 0;
        CREN = 1;
        TXEN = 1;
        SPBRG=129;
    }
    
void UART_Write(char data)
{
    while(!TRMT);
    TXREG = data;
    __delay_ms(0.5);

}
char BT_get_char(void)   
{
    if(OERR) // check for over run error 
    {
        CREN = 0;
        CREN = 1; //Reset CREN
    }
    
    if(RCIF==1) //if the user has sent a char return the char (ASCII value)
    {
    while(!RCIF);  
    return RCREG;
    }
    else //if user has sent no message return 0
        return 0;
}

void UART_Send(char *s)
{
    int i;
  for(i=0;s[i]!='\0';i++)
	  UART_Write(s[i]);
 }
char UART_Data_Ready()
{
   return RCIF;
}
char UART_Read()
{
 
  while(!RCIF);
  return RCREG;
}

void UART_Read_Text(char *Output, unsigned int length)
{
	unsigned int i;
	for(int i=0;i<length;i++)
		Output[i] = UART_Read();
}

void broadcast_BT()
{
  TXREG = 13;  
  __delay_ms(500);
}
  


void GSM_Init(void)
{
  int i;
  UART_Init();
  UART_Send(AT);
  UART_Write(0X0D);
  __delay_ms(100);
  UART_Send(noecho);
  UART_Write(0X0D);
  __delay_ms(100);
  UART_Send(mode_text);
  UART_Write(0X0D);
  __delay_ms(100);
  
  
}
void GSM_Send(char *n,char *m)
{
//char mobile_no[]="AT+CMGS=\"+92090078601\"";   
     
     UART_Send("AT+CMGS=\"");
     UART_Send(n);
     UART_Send("\"");
  UART_Write(0X0D);
  __delay_ms(100);
     UART_Send(m);
  UART_Write(0X0D);
  __delay_ms(100);  
  UART_Write(0X1A);



    }

void ADC_Init()
{
    TRISA=0XFF;
    PORTA=0X00;
     ADCON0 = 0x81;
     ADCON1 = 0x80;
}

unsigned int ADC_Read(unsigned char channel)
{
   if(channel > 7)
   return 0;

   ADCON0 &= 0xC5;
   ADCON0 |= channel<<3;
   __delay_ms(2);
   GO_nDONE = 1;
   while(GO_nDONE);
   return ((ADRESH<<8)+ADRESL);
}





#ifdef	__cplusplus
}
#endif

#endif	/* LIB_H */

