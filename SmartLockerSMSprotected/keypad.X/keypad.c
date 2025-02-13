#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include <string.h>
#include "lib.h"

/* Define necessary macros */
#define _XTAL_FREQ 20000000   // Define crystal frequency
#define MAX_RFID_LENGTH 12    // Maximum RFID length

/* Pin Definitions */
#define RS RD1
#define EN RD0
#define RW RD2
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

/* External Interrupt Configuration */
#define EMERGENCY_PHONE_NUMBER "xxxxxxxxxx"   // Phone number for emergency alerts

/* Variable Definitions */
static unsigned int k = 0;      // OTP attempt counter
static unsigned int j = 0;      // User status flag (0 for locked, 1 for unlocked)
static char rfid[MAX_RFID_LENGTH + 1];  // RFID card data (terminated with '\0')
static char z[4];    // OTP input buffer
static char temp[10]; // Temporary OTP storage
static char predefined_rfids[3][MAX_RFID_LENGTH + 1] = {"0001485389", "A123", "458B"}; // Predefined valid RFID numbers
static char predefined_otps[3][10] = {"A123", "458B", "1A34"}; // Predefined OTPs

/* Function Prototypes */
void initialize_system(void);
void handle_emergency(void);
void process_rfid_and_otp(void);
void display_message(const char* message);
void send_otp_message(const char* phone_number, const char* otp);
void read_rfid(void);

/* Main Program */
void main(void)
{
    initialize_system();  // System Initialization

    while (1)
    {
        display_message("Show your card");
        __delay_ms(500);

        if (j != 0)
        {
            display_message("Enter your OTP");
            __delay_ms(2000);
            process_rfid_and_otp();
        }
    }
}

/* Initialize all system components (LCD, UART, GSM, etc.) */
void initialize_system(void)
{
    TRISD = 0x00;      // Set PORTD as output (for LCD)
    PORTD = 0x00;      // Clear PORTD

    TRISC = 0x00;      // Set PORTC as output
    TRISB = 0xFF;      // Set PORTB as input for RFID and keypad
    RB4 = RB5 = RB6 = RB7 = 0;  // Initialize the pins

    GIE = 1;           // Enable global interrupt
    PEIE = 1;          // Enable peripheral interrupt
    INTEDG = 0;        // Set the external interrupt on falling edge
    RCIE = 1;          // Enable USART receive interrupt
    INTE = 1;          // Enable external interrupt
    INTF = 0;          // Clear external interrupt flag

    Lcd_Init();        // Initialize LCD
    UART_Init();       // Initialize UART
    GSM_Init();        // Initialize GSM Module

    display_message("Please wait...");
    __delay_ms(2000);
}

/* Display a message on the LCD */
void display_message(const char* message)
{
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(message);
}

/* Handle emergency situations by sending an alert message */
void handle_emergency(void)
{
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Emergency");
    GSM_Send(EMERGENCY_PHONE_NUMBER, "LOCKER SECURITY UNIT:\n\rCaution: Security alert\n\rEmergency");
    __delay_ms(2000);
}

/* Process RFID reading and OTP input */
void process_rfid_and_otp(void)
{
    int otp_attempts = 0;
    while (otp_attempts < 4)
    {
        char x = readKeyboard();
        __delay_ms(500);
        z[otp_attempts] = x;
        otp_attempts++;
        Lcd_Set_Cursor(2, otp_attempts);
        Lcd_Write_Char(x);
        __delay_ms(200);
    }

    z[4] = '\0';  // Terminate OTP string
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(z, predefined_otps[i]) == 0)
        {
            j = 0;
            display_message("Password Match");
            __delay_ms(2000);
            send_otp_message(EMERGENCY_PHONE_NUMBER, predefined_otps[i]);
            return;
        }
    }

    display_message("Password mismatch");
    __delay_ms(2000);
}

/* Send OTP message using GSM module */
void send_otp_message(const char* phone_number, const char* otp)
{
    if (k == 0)
    {
        k++;
        GSM_Send(phone_number, predefined_otps[0]);
        strcpy(temp, predefined_otps[0]);
    }
    else if (k == 1)
    {
        k++;
        GSM_Send(phone_number, predefined_otps[1]);
        strcpy(temp, predefined_otps[1]);
    }
    else if (k == 2)
    {
        k = 0;
        GSM_Send(phone_number, predefined_otps[2]);
        strcpy(temp, predefined_otps[2]);
    }
    __delay_ms(200);
}

/* Read RFID data */
void read_rfid(void)
{
    int i = 0;
    while (i < MAX_RFID_LENGTH)
    {
        if (UART_Data_Ready())
        {
            char a = UART_Read();
            if (a == '$')
            {
                for (i = 0; i < MAX_RFID_LENGTH; i++)
                {
                    if (UART_Data_Ready())
                    {
                        a = UART_Read();
                        rfid[i] = a;
                    }
                }
                rfid[MAX_RFID_LENGTH] = '\0'; // Null terminate the RFID string
                return;
            }
        }
    }
}

/* Interrupt Service Routine */
void interrupt ISR(void)
{
    if (INTE && INTF)
    {
        INTF = 0;  // Clear interrupt flag
        handle_emergency();  // Handle emergency case
    }
    else if (RCIE && RCIF)
    {
        // Read data from UART
        read_rfid();
        for (int i = 0; i < 3; i++)
        {
            if (strcmp(rfid, predefined_rfids[i]) == 0)
            {
                Lcd_Clear();
                Lcd_Set_Cursor(1, 1);
                Lcd_Write_String("OTP Sending...");
                __delay_ms(2000);
                process_rfid_and_otp();
                return;
            }
        }
        display_message("Wrong user");
        __delay_ms(2000);
    }
}
