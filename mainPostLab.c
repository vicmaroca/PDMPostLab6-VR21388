/*
 * File:   mainPreLabb6.c
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

//-------LIBRERIAS-------
#include <xc.h>
#include <stdint.h>

//--------VARIABLES GLOBALES----------
int flags; //Variable de banderas
int unidades; //Variable de Unidades
int decenas; //Variable de decenas
int centenas; //Variable de centenas

    //Tabla de Números
const unsigned char segmentos[] = { 0b00111111, // 0
                                    0b00000110, // 1
                                    0b01011011, // 2
                                    0b01001111, // 3
                                    0b01100110, // 4
                                    0b01101101, // 5
                                    0b01111101, // 6
                                    0b00000111, // 7
                                    0b01111111, // 8
                                    0b01100111};// 9

//---------CONSTANTES------
#define _tmr0_value 200 //Valor TMR0
#define _XTAL_FREQ 8000000

//-----PROTOTIPOS DE FUNCIÓN---------
void setup(void); //Función Setup
void conversor(int num); //Función de conversión

//--------INTERRUPCIONES----------
void __interrupt() isr(void)
{
    if(T0IF) //Verificación de bandera de TMR0
    {
        PORTD = 0; //Limpiamos PORTD
        
        if (flags == 0) //Verificación de baderas
        {
            PORTC = segmentos[centenas]; //Cargamos el valor del display en las centenas
            PORTD = 0b0001; //Activamos el display de las centenas
            flags = 1; //Asignamos valor a la bandera
        }
        else if (flags == 1) //Verificación de bandera
        {
            PORTC = segmentos[decenas]; //Cargamos el valor del display a las decenas
            PORTD = 0b0010; //Activamos el display de las decenas
            flags = 2; //Asignamos valor a la bandera
        }
        else if (flags == 2) //Verificamos la bandera
        {
            PORTC = segmentos[unidades]; //Cargamos el valor de las unidades
            PORTD = 0b0100; //Activamos el valor de las unidades
            flags = 0; //Asiganmos valor a la bandera
        }
        
        INTCONbits.T0IF = 0; //Limpiamos la bandera
        TMR0 = _tmr0_value; //Asignamos valor a TMR0
    }
    return;
}

void main(void) {
    
    setup();
    while(1){
        ADCON0bits.CHS = 0; //Canal AN0
        ADCON0bits.GO = 1; // Iniciar la conversión
        while(ADCON0bits.GO); // Esperar a que termine la conversión
        int adc_value_1 = ADRESH; // Leer el valor convertido
        PORTB = (char) adc_value_1;
        __delay_ms(10);
        
        ADCON0bits.CHS = 1; // Seleccionar el canal AN1 para la segunda entrada analógica
        ADCON0bits.GO = 1; // Iniciar la conversión
        while(ADCON0bits.GO); // Esperar a que termine la conversión
        int adc_value_2 = ADRESH; // Leer el valor convertido
        int volt = (adc_value_2 * 2);
        conversor((int) volt);
        __delay_ms(10);
    }
    return;
}

void setup(void){
    
    // CONFIGURACION DE ENTRADAS Y SALIDAS
    
    // Pines digitales
    ANSEL = 0;
    ANSELH = 0;
    // Puerto A como salida
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    
    // Configuracion oscilador interno
    OSCCONbits.IRCF = 0b111; // 8MHz
    OSCCONbits.SCS = 1; //Oscilador interno
    
    //Configuracion TMR0
    OPTION_REGbits.T0CS = 0; //Se selecciona el timer como temporizador
    OPTION_REGbits.PSA = 0; // Prescaler activado para TMR0
    OPTION_REGbits.PS = 0b111; // Prescaler 1:256
    TMR0 = _tmr0_value;
    
    //Configuracion de las interrupciones
    INTCONbits.T0IF = 0; // Bandera tmr0
    INTCONbits.T0IE = 1; // Enable tmr0
    INTCONbits.PEIE = 1; // Interrupciones perifericas
    INTCONbits.GIE = 1; // Interrupciones globales
    
    // Configuracion ADC
    ANSELbits.ANS0 = 1; //Entrada analógica
    ANSELbits.ANS1 = 1; //Entrada analógica 
    TRISAbits.TRISA0 = 1; //Entrada
    TRISAbits.TRISA1 = 1; //Entrada
    ADCON0bits.ADCS = 0b10; // FOSC/32
    __delay_ms(1);
    ADCON1bits.ADFM = 0; //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0; //Voltaje de ref. a GND
    ADCON1bits.VCFG1 = 0; //Voltaje de ref. a 5v
    ADCON0bits.ADON = 1; // ADC is enabled
    ADIF = 0;
    
    return;
}

void conversor (int num) {
    centenas = num / 100;
    num %= 100;
    decenas = num / 10;
    unidades = num % 10;
    
    if (centenas == 5){
        if (decenas >= 0){
            if (unidades >= 0){
                decenas = 0;
                unidades = 0;
            }
        }
    }
    return;
}