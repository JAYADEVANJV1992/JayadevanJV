#include <xc.h>
#include <pic16f877a.h>
#include <stdio.h>
#include "lib.h"

// Define symbolic constants
#define _XTAL_FREQ 20000000   // Define crystal frequency
#define BATTERY_VOLTAGE_FACTOR 12
#define SOLAR_VOLTAGE_FACTOR 12
#define GRID_VOLTAGE_FACTOR 12
#define SOC_VOLTAGE_FACTOR 12
#define ADC_TO_VOLTAGE_DIVISOR 650
#define MAX_VOLTAGE_THRESHOLD 5
#define MIN_VOLTAGE_THRESHOLD 2

// Define system configuration bits
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

void initialize_system(void);
void display_message(const char* message);
void display_voltage(const char* label, unsigned int voltage);
void update_output_controls(unsigned int battery, unsigned int solar, unsigned int grid);

void main()
{
    unsigned int battery_voltage, solar_voltage, grid_voltage, soc_voltage;
    char battery_str[20], solar_str[20], grid_str[20], soc_str[20];

    initialize_system();

    while (1)
    {
        __delay_ms(200);

        // Read battery voltage
        battery_voltage = ADC_Read(0);
        battery_voltage = (battery_voltage * BATTERY_VOLTAGE_FACTOR) / ADC_TO_VOLTAGE_DIVISOR;
        sprintf(battery_str, "Battery volt = %d", battery_voltage);

        // Read solar voltage
        solar_voltage = ADC_Read(1);
        solar_voltage = (solar_voltage * SOLAR_VOLTAGE_FACTOR) / ADC_TO_VOLTAGE_DIVISOR;
        sprintf(solar_str, "Solar volt = %d", solar_voltage);

        // Read grid voltage
        grid_voltage = ADC_Read(2);
        grid_voltage = (grid_voltage * GRID_VOLTAGE_FACTOR) / ADC_TO_VOLTAGE_DIVISOR;
        sprintf(grid_str, "Grid volt = %d", grid_voltage);

        // Read SOC voltage
        soc_voltage = ADC_Read(3);
        soc_voltage = (soc_voltage * SOC_VOLTAGE_FACTOR) / ADC_TO_VOLTAGE_DIVISOR;
        sprintf(soc_str, "SOC = %d", soc_voltage);

        // Update the LCD display with the voltage readings
        display_voltage(battery_str, battery_voltage);
        display_voltage(solar_str, solar_voltage);
        display_voltage(grid_str, grid_voltage);
        display_voltage(soc_str, soc_voltage);

        // Check the voltage levels and control outputs accordingly
        update_output_controls(battery_voltage, solar_voltage, grid_voltage);
    }
}

// Initialize system components (ADC, UART, LCD, etc.)
void initialize_system(void)
{
    TRISA = 0xFF;      // Set PORTA as input (for ADC)
    PORTA = 0x00;      // Clear PORTA

    TRISD = 0x00;      // Set PORTD as output (for LCD)
    TRISB = 0x00;      // Set PORTB as output
    PORTB = 0x00;      // Clear PORTB

    ADC_Init();        // Initialize ADC
    UART_Init();       // Initialize UART
    Lcd_Init();        // Initialize LCD

    Lcd_Clear();
    Lcd_Set_Cursor(1, 5);
    Lcd_Write_String("WELCOME");
    __delay_ms(2000);
}

// Display a message on the LCD
void display_message(const char* message)
{
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(message);
}

// Display voltage information on the LCD
void display_voltage(const char* label, unsigned int voltage)
{
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String(label);
    Lcd_Set_Cursor(1, 16);
    Lcd_Write_Char('V');
    __delay_ms(2000);
}

// Update output controls based on voltage levels
void update_output_controls(unsigned int battery, unsigned int solar, unsigned int grid)
{
    if (battery < MIN_VOLTAGE_THRESHOLD && grid < MIN_VOLTAGE_THRESHOLD && solar < MIN_VOLTAGE_THRESHOLD)
    {
        // Case 1: Battery, Solar, and Grid are below threshold
        RB5 = 0;
        RB2 = 0;
        RB1 = 1;
    }
    else if (battery > MAX_VOLTAGE_THRESHOLD && grid > MAX_VOLTAGE_THRESHOLD && solar > MAX_VOLTAGE_THRESHOLD)
    {
        // Case 2: All voltages are above threshold
        RB5 = 1;
        RB3 = 0;
        RB4 = 0;
        RB1 = 0;
        RB2 = 0;
    }
    else
    {
        // Case 3: Intermediate voltages
        RB5 = 0;

        if (battery < MIN_VOLTAGE_THRESHOLD)
        {
            RB5 = 0;
            RB1 = 1;
            RB2 = 1;

            if (solar > MAX_VOLTAGE_THRESHOLD)
            {
                RB3 = 1;
                RB4 = 0;
                Lcd_Clear();
                Lcd_Set_Cursor(1, 1);
                Lcd_Write_String("Solar charging");
                __delay_ms(2000);
            }
            else
            {
                RB3 = 0;
                RB4 = 1;
                Lcd_Clear();
                Lcd_Set_Cursor(1, 1);
                Lcd_Write_String("Grid charging");
                __delay_ms(2000);
            }
        }
        else
        {
            RB1 = 0;
            RB2 = 0;
            RB3 = 0;
            RB4 = 0;
        }
    }
}
