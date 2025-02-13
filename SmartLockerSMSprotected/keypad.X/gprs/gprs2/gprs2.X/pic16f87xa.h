/* 
 * File:   pic16f87xa.h
 * Author: Acer
 *
 * Created on April 8, 2018, 5:06 PM
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
// char char_mode[]="AT+CSCS=\"GSM\"\r";   // to set character mode
// char param[]="AT+CSMP=17,167,0,0";   // set the parameter of character
// char a,lattitude[11],n[7],longitude[30];
 //char gprs_value1[]="AT+SAPBR=3,1,\"Contype\",\"GPRS\"";
 //unsigned char gprs_value2[]="AT+SAPBR=3,1,\"APN\",\"CMNET\"";
 //unsigned char gprs_value3[]="AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"";
 char gprs_value4[]="AT+SAPBR=1,1";
 char gprs_value5_ip[]="AT+SAPBR=2,1";
 char gprs_location[]="AT+CIPGSMLOC=1,1";
 char gprs_web1[]="AT+HTTPINIT";
// unsigned char gprs_web2[]="AT+HTTPPARA=\"CID\",1";
 //unsigned char gprs_web3_updation[]="AT+HTTPPARA=\"URL\",\"http:loadshedding-001-site1.1tempurl.com/tvm1.php?value1=suresh&value2=suresh&value3=suresh&value4=suresh\"";
 //char gprs_web4[]="AT+HTTPPARA=\"CID\",1";
 char gprs_web5[]="AT+HTTPACTION=0";
 char gprs_web6[]="AT+HTTPREAD";
 //lattitude[11] = '\0';
 char c1[]="00001531459",c2[]="00013947056",c3[]="00001502192",c4[]="00001495682";
    char b1[]="RAGU",b2[]="00013947056",b3[]="00001502192",b4[]="00001495682";
    typedef struct
{
 unsigned char gprs_location[26];
char gprs_location_E[5];
}Location;
Location locate;
char i,rfid[13];
void Lcd_Port(char);
void Lcd_Cmd(char );
Lcd_Clear();
void Lcd_Init();
void Lcd_Set_Cursor(char , char );
void Lcd_Write_Char(char );
void Lcd_Write_String(char *);

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
void UART2_Init(void);
unsigned char UART2_Read(void);
void UART1_Read_String(char *,char *);
void UART3_Init(void);
void GPRS1_Init();
void GPRS1_Location();
void GPRS1_Update(char *,char *,char *);
unsigned char UART3_Read(void);
void GPS_Send3();

#ifdef	__cplusplus
}
#endif

#endif	/* PIC16F87XA_H */

