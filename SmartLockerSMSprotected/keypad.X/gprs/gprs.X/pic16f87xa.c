#define _XTAL_FREQ 20000000
#include"pic16f87xa.h"
#include<pic16f877a.h>
#include<string.h>
char rgps;
int gps_flag = 0, j ;
char gps_data[35];
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
void Lcd_Char(char a, char b,char c)
{
    char temp,z,y;
   char temp1,y1;
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
   temp1 = c&0x0F;
   y1 = c&0xF0;
   RS = 1;     
   RW=0;// => RS = 1
   Lcd_Port(y1>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp1);
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
void Lcd_String(char a,char b,char *c)
{
    int i;
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
    for(i=0;c[i]!='\0';i++)
	   Lcd_Write_Char(c[i]);
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
void UART1_Init(void)
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
        TX9=0;
        RX9=0;
        SPBRG=129;
    }
void UART1_Write(char data)
{
    
  while(!TRMT);
  TXREG = data;
  __delay_us(10);
}

void UART1_Send(char *s)
{
 while(*s)
 UART1_Write(*s++);
 
 }
void UART2_Init(void)		// Initialize UART pins to proper values
{
    #define Baudrate              9600                    
#define OneBitDelay           98
#define DataBitCount          8                        
#define UART_RX               RB1						
#define UART_TX               RB2						
#define UART_RX_DIR			  TRISB1					
#define UART_TX_DIR			  TRISB2					
	UART_TX = 1;			// TX pin is high in idle state
	
	UART_RX_DIR = 1;		// Input
	UART_TX_DIR = 0;		// Output
}



unsigned char UART2_Read(void)
{
	// Pin Configurations
    // GP1 is UART RX Pin

	unsigned char DataValue = 0;

	//wait for start bit
	while(UART_RX==1);

	__delay_us(OneBitDelay);
	__delay_us(OneBitDelay/2);   // Take sample value in the mid of bit duration

	for ( unsigned char i = 0; i < DataBitCount; i++ )
	{
		if ( UART_RX == 1 )   //if received bit is high
		{
			DataValue += (1<<i);
		}

		__delay_us(OneBitDelay);
	}

	// Check for stop bit
	if ( UART_RX == 1 )       //Stop bit should be high
	{
		__delay_us(OneBitDelay/2);
		return DataValue;
	}
	else                      //some error occurred !
	{
		__delay_us(OneBitDelay/2);
		return 0x000;
	}
}




void UART2_Write(const char DataValue)
{
	/* Basic Logic
	   
	   TX pin is usually high. A high to low bit is the starting bit and 
	   a low to high bit is the ending bit. No parity bit. No flow control.
	   BitCount is the number of bits to transmit. Data is transmitted LSB first.

	*/

	// Send Start Bit
	UART_TX = 0;
	__delay_us(OneBitDelay);

	for ( unsigned char i = 0; i < DataBitCount; i++ )
	{
		//Set Data pin according to the DataValue
		if( ((DataValue>>i)&0x1) == 0x1 )   //if Bit is high
		{
			UART_TX = 1;
		}
		else      //if Bit is low
		{
			UART_TX = 0;
		}

	    __delay_us(OneBitDelay);
	}

	//Send Stop Bit
	UART_TX = 1;
	__delay_us(OneBitDelay);
}
void UART2_Send(char *s)
{
 while(*s)
 UART2_Write(*s++);
 
 }
char UART1_TX_Empty()
{
  return TRMT;
}

char UART1_Data_Ready()
{
   return RCIF;
}
char UART1_Read()
{
 
  while(!RCIF);
  return RCREG;
}

void UART1_Read_Text(char *Output, unsigned int length)
{
	unsigned int i;
	for(int i=0;i<length;i++)
		Output[i] = UART1_Read();
}
void ADC_Init()
{
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
char uart_char()
{
    if(OERR)
    {
        CREN=0;
        CREN=1;
    }
    while(!RCIF);
    return RCREG;
}
void UART1_Read_String(char *e,char *l)
{
    unsigned int i;
    unsigned char a;

   if(UART1_Data_Ready())
     {
      for(i=0;e[i]!=*l;) 
      {
        if(UART1_Data_Ready())
         {
           a= uart_char();
             if(a==*l)
           {
                 e[i]='\0';
                 break;
           }
             e[i]=a;
         
             i++;
           
            
       }
     }
   }
}
void UART2_Read_String(char *e,char *l)
{
    unsigned int i;
    unsigned char a;

  // if(UART1_Data_Ready())
     {
      for(i=0;e[i]!=*l;) 
      {
       // if(UART1_Data_Ready())
         {
           a= UART2_Read();
             if(a==*l)
           {
                 e[i]='\0';
                 break;
           }
             e[i]=a;
         
             i++;
           
            
       }
     }
   }
}
void GSM1_Init(void)
{
  int i;
  UART1_Init();
  UART1_Send(AT);
  UART1_Write(0X0D);
  __delay_ms(100);
  UART1_Send(noecho);
  UART1_Write(0X0D);
  __delay_ms(100);
  UART1_Send(mode_text);
  UART1_Write(0X0D);
  __delay_ms(100);
  
  
}
void GSM1_Send(char *n,char *m)
{
//char mobile_no[]="AT+CMGS=\"+92090078601\"";   
    int i;
  UART1_Init();
  UART1_Send(AT);
  UART1_Write(0X0D);
  __delay_ms(100);
  UART1_Send(noecho);
  UART1_Write(0X0D);
  __delay_ms(100);
  UART1_Send(mode_text);
  UART1_Write(0X0D);
  __delay_ms(100); 
     UART1_Send("AT+CMGS=\"");
     UART1_Send(n);
     UART1_Send("\"");
  UART1_Write(0X0D);
  __delay_ms(100);
     UART1_Send(m);
  UART1_Write(0X0D);
  __delay_ms(100);  
  UART1_Write(0X1A);
__delay_ms(100);
}
void GSM2_Init(void)
{
  int i;
  UART2_Init();
  UART2_Send(AT);
  UART2_Write(0X0D);
  __delay_ms(100);
  UART2_Send(noecho);
  UART2_Write(0X0D);
  __delay_ms(100);
  UART2_Send(mode_text);
  UART2_Write(0X0D);
  __delay_ms(100);
  
  
}
void GSM2_Send(char *n,char *m)
{
//char mobile_no[]="AT+CMGS=\"+92090078601\"";   
    int i;
  UART2_Init();
  UART2_Send(AT);
  UART2_Write(0X0D);
  __delay_ms(100);
  UART2_Send(noecho);
  UART2_Write(0X0D);
  __delay_ms(100);
  UART2_Send(mode_text);
  UART2_Write(0X0D);
  __delay_ms(100); 
     UART2_Send("AT+CMGS=\"");
     UART2_Send(n);
     UART2_Send("\"");
  UART2_Write(0X0D);
  __delay_ms(100);
     UART2_Send(m);
  UART2_Write(0X0D);
  __delay_ms(100);  
  UART2_Write(0X1A);
__delay_ms(100);
}

void GPS_GSM_Send1(char u,char *n,char *m)
{
    UART1_Init();
    //UART1_Send('j');
    
	gps_flag = 0 ;
	while(gps_flag == 0)
	{
	    while(!RCIF);
            rgps = RCREG;

	if(rgps == '$')
	{
        
		while(!RCIF);
                rgps = RCREG;
		if(rgps == 'G')
		{
			while(!RCIF);
                        rgps = RCREG;
			if(rgps == 'P')
			{
                
				while(!RCIF);
                                rgps = RCREG;
				if(rgps == 'G')
				{
					while(!RCIF);
                                        rgps = RCREG;
					if(rgps == 'L')
					{
						while(!RCIF);
          					rgps = RCREG;
						if(rgps == 'L')
						{
							while(!RCIF);
            						rgps = RCREG;
							if(rgps == ',')
							{
                                //UART1_Send("JAY");
								while(!RCIF);
            							rgps = RCREG;
									gps_flag = 1;
									for(j = 0 ; j < 25 ; j++)
									{
										while(!RCIF);
            						    rgps = RCREG;
										gps_data[j] = rgps;
									}
                                    gps_data[23] = '\0';
                                   if(u==1)
                                    {
                                     UART1_Send(AT);
                                     UART1_Write(0X0D);
                                    __delay_ms(100);
                                    UART1_Send(noecho);
                                    UART1_Write(0X0D);
                                    __delay_ms(100);
                                    UART1_Send(mode_text);
                                    UART1_Write(0X0D);
                                     __delay_ms(100); 
                                    UART1_Send("AT+CMGS=\"");
                                    UART1_Send(n);
                                    UART1_Send("\"");
                                    UART1_Write(0X0D);
                                    __delay_ms(100);
                                    UART1_Send(m);
                                    UART1_Send("Location:\n\r");
                                    UART1_Send(gps_data);
                                    UART1_Write(0X0D);
                                    __delay_ms(100);  
                                    UART1_Write(0X1A);
                                    __delay_ms(100);
                                    }
                                   else if(u==2)
                                    {
                                       UART2_Init();
                                      UART2_Send(AT);
                                     UART2_Write(0X0D);
                                    __delay_ms(100);
                                    UART2_Send(noecho);
                                    UART2_Write(0X0D);
                                    __delay_ms(100);
                                    UART2_Send(mode_text);
                                    UART2_Write(0X0D);
                                     __delay_ms(100); 
                                    UART2_Send("AT+CMGS=\"");
                                    UART2_Send(n);
                                    UART2_Send("\"");
                                    UART2_Write(0X0D);
                                    __delay_ms(100);
                                    UART2_Send(m);
                                    UART2_Send("Location:\n\r");
                                    UART2_Send(gps_data);
                                    UART2_Write(0X0D);
                                    __delay_ms(100);  
                                    UART2_Write(0X1A);
                                    __delay_ms(100);  
                                    }
                     
                            }
								}
							}
						}
					}
				}
			}
		}

	}
void GPS_GSM_Send2(char u,char *n,char *m)
{
    UART2_Init();
    //UART1_Send('j');
    
	gps_flag = 0 ;
	while(gps_flag == 0)
	{
	   
            rgps = UART2_Read();

	if(rgps == '$')
	{
        rgps = UART2_Read();
		if(rgps == 'G')
		{
			rgps = UART2_Read();
			if(rgps == 'P')
			{
              rgps = UART2_Read();
				if(rgps == 'G')
				{
					rgps = UART2_Read();
					if(rgps == 'L')
					{
					rgps = UART2_Read();
						if(rgps == 'L')
						{
							rgps = UART2_Read();
							if(rgps == ',')
							{
                                rgps = UART2_Read();
									gps_flag = 1;
									for(j = 0 ; j < 25 ; j++)
									{
										rgps = UART2_Read();
										gps_data[j] = rgps;
									}
                                    gps_data[23] = '\0';
                                   if(u==1)
                                    {
                                       UART1_Init();
                                     UART1_Send(AT);
                                     UART1_Write(0X0D);
                                    __delay_ms(100);
                                    UART1_Send(noecho);
                                    UART1_Write(0X0D);
                                    __delay_ms(100);
                                    UART1_Send(mode_text);
                                    UART1_Write(0X0D);
                                     __delay_ms(100); 
                                    UART1_Send("AT+CMGS=\"");
                                    UART1_Send(n);
                                    UART1_Send("\"");
                                    UART1_Write(0X0D);
                                    __delay_ms(100);
                                    UART1_Send(m);
                                    UART1_Send("Location:\n\r");
                                    UART1_Send(gps_data);
                                    UART1_Write(0X0D);
                                    __delay_ms(100);  
                                    UART1_Write(0X1A);
                                    __delay_ms(100);
                                    }
                                   else if(u==2)
                                    {
                                       UART2_Init();
                                      UART2_Send(AT);
                                     UART2_Write(0X0D);
                                    __delay_ms(100);
                                    UART2_Send(noecho);
                                    UART2_Write(0X0D);
                                    __delay_ms(100);
                                    UART2_Send(mode_text);
                                    UART2_Write(0X0D);
                                     __delay_ms(100); 
                                    UART2_Send("AT+CMGS=\"");
                                    UART2_Send(n);
                                    UART2_Send("\"");
                                    UART2_Write(0X0D);
                                    __delay_ms(100);
                                    UART2_Send(m);
                                    UART2_Send("Location:\n\r");
                                    UART2_Send(gps_data);
                                    UART2_Write(0X0D);
                                    __delay_ms(100);  
                                    UART2_Write(0X1A);
                                    __delay_ms(100);  
                                    }
                     
                            }
								}
							}
						}
					}
				}
			}
		}
}
void GPRS1_Init()
{
    int value=0;
    char gprs_value,gprs_data[8],response[]="OK";
    Lcd_Init();
    gprs:
      Lcd_Clear();
      Lcd_String(1,1,"GPRS configuring");
       Lcd_String(2,1,"Please wait...");
      __delay_ms(2000); 
     UART1_Send(AT);
    UART1_Write(0X0D);
     if(UART1_Data_Ready())
     {
   for(unsigned short c = 0 ; c < 2 ; c++)
    {
       while(!RCIF);
       gprs_value = RCREG;
	  gprs_data[c] = gprs_value;
	}
   gprs_data[2] = '\0';
   value=strcmp(gprs_data,response);
   if(value==0)
   {
       UART1_Send(AT);
       UART1_Write(0X0D);
       __delay_ms(1000);
       UART1_Send(gprs_value1);
       UART1_Write(0X0D);
       __delay_ms(1000);
       __delay_ms(1000);
       UART1_Send(gprs_value2);
       UART1_Write(0X0D);
       __delay_ms(1000);
       __delay_ms(1000);
       UART1_Send(gprs_value3);
       UART1_Write(0X0D);
       __delay_ms(1000);
       __delay_ms(1000);
       __delay_ms(1000);
       UART1_Send(gprs_value4);
       UART1_Write(0X0D);
       __delay_ms(1000);
       __delay_ms(1000);
       __delay_ms(1000);
      Lcd_Clear();
      Lcd_String(1,1,"GPRS configured");
       Lcd_String(2,1,"Successfully...");
      __delay_ms(1000); 
      
      
      
   }
   else
   {
       
       goto gprs;
       
   }
     }

}
void GPRS1_Check_OK()
{
    int value=0;
    char gprs_value,gprs_data[8],response[]="OK";
    Lcd_Init();
    gprs:
     
     UART1_Send(AT);
    UART1_Write(0X0D);
     if(UART1_Data_Ready())
     {
   for(unsigned short c = 0 ; c < 2 ; c++)
    {
       while(!RCIF);
       gprs_value = RCREG;
	  gprs_data[c] = gprs_value;
	}
   gprs_data[2] = '\0';
   value=strcmp(gprs_data,response);
   if(value==0)
   {
      return; 
      
   }
   else
   {
       
       goto gprs;
       
   }
     }

}
GPRS1_Location()
{
    
    char gpp,gprs_location_value[5];
    
    GPRS1_Check_OK();
    UART1_Send(gprs_value5_ip);
       UART1_Write(0X0D);
       __delay_ms(2000);
       __delay_ms(2000);
       UART1_Send(gprs_location);
       UART1_Write(0X0D);
       //__delay_ms(2000);
       //__delay_ms(2000);
        if(UART1_Data_Ready())
     {
   for(unsigned short c9 = 0 ; c9 < 2 ; c9++)
    {
       while(!RCIF);
       gpp = RCREG;
	  gprs_location_value[c9] = gpp;
	}
   gprs_location_value[2] = '\0';
   
       
    
     
}

}
