/* 
 * File:   pic16f87xa.h
 * Author: Jayadevan.J.V
 *
 * Created on March 16, 2018, 9:24 PM
 */

#ifndef PIC16F87XA_H
#define	PIC16F87XA_H

#ifdef	__cplusplus
extern "C" {
#endif
#include<xc.h>
#include<pic16f877a.h>
 #define RS RD1
 #define EN RD0
 #define RW RD2
 #define D4 RD4
 #define D5 RD5
 #define  D6 RD6
 #define D7 RD7

char AT[]="AT";   // To initialize mode
 char noecho[]="ATE0";   // To stop echo
 char mode_text[]="AT+CMGF=1";   // to set text mode
 char char_mode[]="AT+CSCS=\"GSM\"\r";   // to set character mode
 char param[]="AT+CSMP=17,167,0,0";   // set the parameter of character
 char a,lattitude[11],n[7],longitude[30];
 char gprs_value1[]="AT+SAPBR=3,1,\"Contype\",\"GPRS\"";
 char gprs_value2[]="AT+SAPBR=3,1,\"APN\",\"CMNET\"";
 char gprs_value3[]="AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"";
 char gprs_value4[]="AT+SAPBR=1,1";
 char gprs_value5_ip[]="AT+SAPBR=2,1";
 char gprs_location[]="AT+CIPGSMLOC=1,1";
 char gprs_web1[]="AT+HTTPINIT";
 char gprs_web2[]="AT+HTTPPARA=\"CID\",1";
 char gprs_web3_updation[]="AT+HTTPPARA=\"URL\",\"http:loadshedding-001-site1.1tempurl.com/tvm1.php?value1=suresh&value2=suresh&value3=suresh&value4=suresh\"";
 char gprs_web4[]="AT+HTTPPARA=\"CID\",1";
 char gprs_web5[]="AT+HTTPACTION=0";
 char gprs_web6[]="AT+HTTPREAD";
 //lattitude[11] = '\0';
 //longitude[12] = '\0';
// char mobile_no[]="AT+CMGS=\"+919747154363\"";   //use to set receinpent number and mesg
 //char mesg[]="hHHHH";
    #define Baudrate              9600                    
#define OneBitDelay           98
#define DataBitCount          8                        
#define UART_RX               RB1						
#define UART_TX               RB2						
#define UART_RX_DIR			  TRISB1					
#define UART_TX_DIR			  TRISB2	
void Lcd_Port(char);
void Lcd_Cmd(char );
Lcd_Clear();
void Lcd_Init();
void Lcd_Set_Cursor(char , char );
void Lcd_Write_Char(char );
void Lcd_Write_String(char *);
void Lcd_Shift_Right();
void Lcd_Shift_Left();
void Lcd_Char(char , char ,char );
void Lcd_String(char ,char ,char *);
void p(char ,char ,char ,char ,char ,char ,char );
void UART1_Send(char *);
void UART1_Write(char );
void UART1_Init(void);
void UART1_Read_Text(char *, unsigned int );
char UART1_Read();
char UART1_Data_Ready();
char UART1_TX_Empty();
void UART2_Send(char *);
void UART2_Write(const char);
unsigned char UART2_Read(void);
void UART2_Init(void);
void ADC_Init();
unsigned int ADC_Read(unsigned char );
char uart_char();
void UART1_Read_String(char *,char *);
void UART2_Read_String(char *,char *);
void GSM1_Send(char *,char *);
void GSM1_Init(void);
void GSM2_Init(void);
void GSM2_Send(char *,char *);
void GPS_GSM_Send1(char,char *,char *);
void GPS_GSM_Send2(char,char *,char *);
void GPRS1_Init();
#ifdef	__cplusplus
}
#endif

#endif	/* PIC16F87XA_H */

