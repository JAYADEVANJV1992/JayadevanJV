#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include "lib.h"

// Configuration bits
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// Pin Definitions
#define RS RD1
#define EN RD0
#define RW RD2
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

// Global Variables
unsigned int c, i;
char a;
float b;
char d[20];
int get_value;

// Main function
void main() {
    // Set up pins
    TRISD = 0x00;               // Set PORTD as output
    TRISB1 = 1;                 // Set RB1 as input (for fire detection)
    RB1 = 1;                    // Initialize RB1
    TRISB2 = 0;                 // Set RB2 as output (used for control)
    RB2 = 1;                    // Initialize RB2
    TRISC1 = TRISC2 = TRISC3 = TRISC4 = 0; // Set RC1 to RC4 as outputs
    RC1 = RC2 = RC3 = RC4 = 0; // Initialize RC pins

    // LCD Initialization
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1, 5);
    Lcd_Write_String("WELCOME");
    __delay_ms(2000);

    // Display Bluetooth status
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(" BLUETOOTH ");
    Lcd_Set_Cursor(2, 1);
    __delay_ms(2000);

    // Initialize UART communication
    UART_Init();
    UART_Send("hello");        // Send "hello" message via UART
    broadcast_BT();            // Broadcast Bluetooth message

    // Main loop
    while(1) {
        // Get data from Bluetooth module
        get_value = BT_get_char();

        // Fire detection logic
        if (RB1 != 1) {
            RB2 = 0;                // Reset control signals
            RC1 = RC2 = RC3 = RC4 = 0;
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("FIRE DETECTED");
            Lcd_Set_Cursor(2, 1);
            __delay_ms(1000);       // Display "Fire Detected" for 1 second
        }

        // Handle Bluetooth commands (forward, right, back, left)
        if (get_value == '0') {
            RC1 = 1; RC2 = 0; RC3 = 1; RC4 = 0;  // Forward motion
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("   forward ");
            __delay_ms(200);
            RC1 = RC2 = RC3 = RC4 = 0;
        }
        else if (get_value == '1') {
            RC1 = 0; RC2 = 1; RC3 = 1; RC4 = 0;  // Right turn
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("   right ");
            __delay_ms(200);
            RC1 = RC2 = RC3 = RC4 = 0;
        }
        else if (get_value == '2') {
            RC1 = 0; RC2 = 1; RC3 = 0; RC4 = 1;  // Backward motion
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("   back ");
            __delay_ms(200);
            RC1 = RC2 = RC3 = RC4 = 0;
        }
        else if (get_value == '3') {
            RC1 = 1; RC2 = 0; RC3 = 0; RC4 = 1;  // Left turn
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String("   left ");
            __delay_ms(200);
            RC1 = RC2 = RC3 = RC4 = 0;
            __delay_ms(100); // Small delay before accepting the next command
        }
    }
}
