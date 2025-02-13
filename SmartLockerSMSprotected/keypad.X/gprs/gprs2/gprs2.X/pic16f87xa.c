#define _XTAL_FREQ 20000000
#include"pic16f87xa.h"
#include<pic16f877a.h>
#include<string.h>
char rgps;
char gr[]="55";
int gps_flag = 0, j ;

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
void UART1_Read_Text(char *Output, unsigned int length)
{
	unsigned int i;
	for(int i=0;i<length;i++)
		Output[i] = UART1_Read();
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
void UART3_Init(void)		// Initialize UART pins to proper values
{
    #define Baudrate3              9600                    
#define OneBitDelay3           98
#define DataBitCount3          8                        
#define UART_RX3               RB3						
#define UART_TX3         RB4						
#define UART_RX_DIR3			  TRISB3					
#define UART_TX_DIR3			  TRISB4					
	UART_TX3 = 1;			// TX pin is high in idle state
	
	UART_RX_DIR3 = 1;		// Input
	UART_TX_DIR3 = 0;		// Output
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
unsigned char UART3_Read(void)
{
	// Pin Configurations
    // GP1 is UART RX Pin

	unsigned char DataValue3 = 0;

	//wait for start bit
	while(UART_RX3==1);

	__delay_us(OneBitDelay3);
	__delay_us(OneBitDelay3/2);   // Take sample value in the mid of bit duration

	for ( unsigned char i = 0; i < DataBitCount3; i++ )
	{
		if ( UART_RX3 == 1 )   //if received bit is high
		{
			DataValue3 += (1<<i);
		}

		__delay_us(OneBitDelay3);
	}

	// Check for stop bit
	if ( UART_RX3 == 1 )       //Stop bit should be high
	{
		__delay_us(OneBitDelay3/2);
		return DataValue3;
	}
	else                      //some error occurred !
	{
		__delay_us(OneBitDelay3/2);
		return 0x000;
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
    __delay_ms(1000);
       __delay_ms(1000);
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
  // if(value==0)
   {
       UART1_Send(AT);
       UART1_Write(0X0D);
       __delay_ms(1000);
       UART1_Send("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
       UART1_Write(0X0D);
       __delay_ms(1000);
       __delay_ms(1000);
       UART1_Send("AT+SAPBR=3,1,\"APN\",\"CMNET\"");
       UART1_Write(0X0D);
       __delay_ms(1000);
       __delay_ms(1000);
       UART1_Send("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");
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
   //else
   {
       
       //goto gprs;
       
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

void GPS_Send3()
{
   
    UART3_Init();
 
	
	while(gps_flag == 0)
	{
	 t1:  
    rgps = UART3_Read();

	if(rgps == '$')
	{
        t2:
        rgps = UART3_Read();
		if(rgps == 'G')
		{
            t3:
			rgps = UART3_Read();
			if(rgps == 'P')
			{
                t4:
              rgps = UART3_Read();
				if(rgps == 'G')
				{
                    t5:
					rgps = UART3_Read();
					if(rgps == 'L')
					{
                        t6:
					rgps = UART3_Read();
						if(rgps == 'L')
						{
                            t7:
							rgps = UART3_Read();
							if(rgps == ',')
							{
                                rgps = UART3_Read();
									gps_flag = 1;
									for(j = 0 ; j < 25 ; j++)
									{
										rgps = UART3_Read();
										locate.gprs_location[j] = rgps;
									}
                                    locate.gprs_location[23] = '\0';
                                   // UART1_Send(locate.gprs_location);
                                    break;
                                   
                     
                            }
                            else
                            {
                                goto t7;
                            }
								}
                        else
                        {
                            goto t6;
                        }
							}
                    else
                    {
                        goto t5;
                    }
						}
                else
                {
                    goto t4;
                }
					}
            else
            {
                goto t3;
            }
				}
        else
        {
            goto t2;
        }
			}
   
    else
    {
        goto t1;
    }
		}
}




void GPRS1_Location()
{
    
    char gpp,w;
    
    GPRS1_Check_OK();
    UART1_Send(gprs_value5_ip);
       UART1_Write(0X0D);
       __delay_ms(2000);
       __delay_ms(2000);
       UART1_Send(gprs_location);
       UART1_Write(0X0D);
       //__delay_ms(2000);
       //__delay_ms(2000);
       
        while(!RCIF);
        w=RCREG;
        if(w=='+')
        {
            
              Lcd_Clear();
      Lcd_String(1,1,"YES");
 
      __delay_ms(1000);
            while(!RCIF);
        w=RCREG;
        if(w=='C')
        {  
        }
        }
        {
     //  UART1_Read_String(locate.gprs_location_N,",");
      // UART1_Read_String(locate.gprs_location_E,",");
       
       }
     
}


void GPRS1_Update(char *r,char *n,char *l)
{
    //GPRS1_Check_OK();
    UART1_Send(AT);
       UART1_Write(0X0D);
       __delay_ms(1000);
      // __delay_ms(2000);
     UART1_Send(gprs_value5_ip);
       UART1_Write(0X0D);
       __delay_ms(2000);
       __delay_ms(2000);
       UART1_Send(gprs_web1);
       UART1_Write(0X0D);
       __delay_ms(2000);
       __delay_ms(2000);
       UART1_Send("AT+HTTPPARA=\"CID\",1");
       UART1_Write(0X0D);
       __delay_ms(2000);
       __delay_ms(2000);
       __delay_ms(1000);
       UART1_Send("AT+HTTPPARA=\"URL\",\"http://loadshedding-001-site1.1tempurl.com/tvm1.php?value1=587&value2=");
         UART1_Send(r);
        UART1_Send("&value3="); 
        UART1_Send(n);
        UART1_Send("&value4=");
       UART1_Send(l);
        UART1_Send("\"\r\n");
        
       UART1_Write(0X0D);
       __delay_ms(2000);
       __delay_ms(2000);
       __delay_ms(1000);
       __delay_ms(1000);
       UART1_Send("AT+HTTPPARA=\"CID\",1");
       UART1_Write(0X0D);
      
       __delay_ms(2000);
       __delay_ms(1000);
       UART1_Send(gprs_web5);
       UART1_Write(0X0D);
       __delay_ms(2000);
       __delay_ms(2000);
       __delay_ms(2000);
       UART1_Send(gprs_web6);
       UART1_Write(0X0D);
       __delay_ms(2000);
       __delay_ms(2000);
       
}
