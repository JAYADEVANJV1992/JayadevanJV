#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include "lib_1.h"

// Macro Definitions
#define _XTAL_FREQ 20000000  // Define crystal frequency for delay
#define RS RD1               // Register select pin for LCD
#define EN RD0               // Enable pin for LCD
#define RW RD2               // Read/Write pin for LCD
#define D4 RD4               // Data pin 4 for LCD
#define D5 RD5               // Data pin 5 for LCD
#define D6 RD6               // Data pin 6 for LCD
#define D7 RD7               // Data pin 7 for LCD

// Global Variables
char rcv[50];              // Buffer to hold received data from UART
float a;                   // Variable to hold UART data
int buf = 0;               // Buffer index (not used in this code)

// Interrupt Service Routine (ISR)
void interrupt isr() {
    if (RCIF) {
        RCIF = 0;          // Clear UART receive interrupt flag
        receiveData();     // Call function to handle received data
    }
}

// UART Receive Data Function
void receiveData(void) {
    unsigned int i = 0;
    
    // Read data from UART until buffer is full or no data is received
    for (i = 0; i < 12;) {
        if (UART_Data_Ready()) {  // Check if UART data is ready
            if (OERR) {           // Check if overrun error occurred
                CREN = 0;         // Reset the receiver enable bit
                CREN = 1;         // Re-enable receiver
            }
            
            while (!RCIF);        // Wait for data to be received
            a = RCREG;            // Read received data
            rcv[i] = a;           // Store received data in buffer
            i++;                  // Increment buffer index
        }
    }
    
    // Clear LCD and display received data
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(rcv);
    __delay_ms(500);           // Delay for a moment to display data
}

// Main Function
void main() {
    TRISD = 0x00;              // Set PORTD as output
    TRISB = 0x00;              // Set PORTB as output
    PORTB = 0x00;              // Clear PORTB

    GIE = 1;                   // Enable global interrupts
    PEIE = 1;                  // Enable peripheral interrupts

    RCIE = 1;                  // Enable UART receive interrupt

    ADC_Init();                // Initialize ADC
    GSM_Init();                // Initialize GSM module
    UART_Init();               // Initialize UART
    Lcd_Init();                // Initialize LCD
    
    // Display Welcome message on LCD
    Lcd_Clear();
    Lcd_Set_Cursor(1, 5);
    Lcd_Write_String("WELCOME");
    __delay_ms(2000);          // Delay for 2 seconds

    Lcd_Clear();
    while (1) {
        // Display waiting message
        Lcd_Clear();
        Lcd_Set_Cursor(1, 5);
        Lcd_Write_String("waiting");
        __delay_ms(2000);      // Delay for 2 seconds
    }
}
