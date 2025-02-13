#define _XTAL_FREQ 20000000

/* LCD Pin Configurations */
#define RS RD1
#define EN RD0
#define RW RD2
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

/* ADC Read Conversion Factor */
#define TEMP_CONVERSION_FACTOR 0.489

/* Temperature Threshold */
#define TEMP_HIGH_THRESHOLD 40

/* Configuration Bits */
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
#include "lib.h"
#include <stdio.h>

/* Global Variables */
volatile int k = 0; /* Used for emergency flag */
char tempDisplay[20];

/* Function Prototypes */
void Initialize(void);
void DisplayEmergency(void);
void CheckTemperature(void);

void main(void)
{
    /* Initialize system */
    Initialize();

    /* Display initial welcome messages */
    Lcd_Clear();
    Lcd_Set_Cursor(1,5);
    Lcd_Write_String("WELCOME");
    __delay_ms(2000);

    Lcd_Set_Cursor(1,2);
    Lcd_Write_String("FIRE FIGHTING");
    Lcd_Set_Cursor(2,6);
    Lcd_Write_String("ROBOT");
    __delay_ms(2000);
    Lcd_Clear();

    /* Main loop */
    while (1)
    {
        __delay_ms(100);
        CheckTemperature();
    }
}

/* Initialize system peripherals */
void Initialize(void)
{
    /* Set analog pin configurations */
    TRISA = 0xFF;      // Set PORTA as input
    TRISD = 0x00;      // Set PORTD as output
    TRISB2 = 0;        // Set RB2 as output for motor control

    /* Initialize peripherals */
    ADC_Init();
    UART_Init();
    Lcd_Init();

    /* Enable Interrupts */
    GIE = 1;           // Global interrupt enable
    PEIE = 1;          // Peripheral interrupt enable
    INTE = 1;          // Enable external interrupt
    INTEDG = 0;        // Interrupt on falling edge of RB0
    INTF = 0;          // Clear interrupt flag
}

/* Check the temperature and control the motor accordingly */
void CheckTemperature(void)
{
    unsigned int temp = ADC_Read(0); // Read ADC value

    temp = (unsigned int)(temp * TEMP_CONVERSION_FACTOR); // Convert to temperature

    if (temp > TEMP_HIGH_THRESHOLD)
    {
        RB2 = 1;               // Turn on motor (e.g., activate water pump)
        UART_Send("TEMP HIGH");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("MOTOR ON");
        __delay_ms(2500);
    }
    else
    {
        RB2 = 0;               // Turn off motor
        UART_Send("TEMP LOW");
    }

    /* Display temperature on LCD */
    sprintf(tempDisplay, "Temp = %d", temp);
    UART_Send(tempDisplay);         // Send to UART for monitoring
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(tempDisplay);
    Lcd_Set_Cursor(1,10);
    Lcd_Write_String("'C");
    __delay_ms(1000);
}

/* Emergency handling on external interrupt */
void interrupt button(void)
{
    /* Check for external interrupt flag */
    if (INTF)
    {
        INTF = 0;             // Clear interrupt flag
        DisplayEmergency();
    }
}

/* Display emergency message */
void DisplayEmergency(void)
{
    Lcd_Clear();
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("EMERGENCY");
    UART_Send("EMERGENCY");
    __delay_ms(1000);
}
