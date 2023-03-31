/*
 * File:   mainPreLab6.c
 * Author: Victor Rodriguez
 *
 * Created on 24 de marzo de 2023, 09:59 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//---------LIBRERÍAS---------
#include <xc.h>
#include <stdint.h>

//---------CONSTANTES---------
#define _tmr0_value 80

//---------VARIABLES GLOBALES---------


//---------PROTOTIPOS DE FUNCIÓN---------
void setup (void);


//---------INTERRUPCIONES-----------
void __interrupt() isr (void)
{
    if(T0IF) //Si la bandera se activa se realiza lo siguiente: 
    {
        PORTA++; //Aumentamos PORTA
        INTCONbits.T0IF = 0; //Limpiamos la bandera
        TMR0 = _tmr0_value; //Asiganmos valor a TMR0
    }
    return;
}

//---------------MAIN----------------
void main(void) 
{
    setup();
    while(1) //Loop principal
    {
        
    }
    return;
}

//--------CONFIGURACIÓN SETUP----------------
void setup (void)
{
    //Pines Digitales
    ANSEL = 0;
    ANSELH = 0;
    
    //PORTA como salida
    TRISA = 0;
    
    //Configuracioón Oscilador
    OSCCONbits.IRCF = 0b100; //Frecuencia 1MHz
    OSCCONbits.SCS = 1; //Oscilador Interno
    
    //Configuración del TMR0
    OPTION_REGbits.T0CS = 0; //Tmr0 como temporizador
    OPTION_REGbits.PSA = 0; //Prescaler para tmr0
    OPTION_REGbits.PS = 0b111; //Prescaler de 256
    TMR0 = _tmr0_value;
    
    //Configuración de las interrupciones
    INTCONbits.T0IF = 0; //Bandera de Overflow de TMR0
    INTCONbits.T0IE = 1; //Interrupción de Overflow TMR0
    INTCONbits.GIE = 1; //Interrupción Global
    return; 
    
}