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


//---------LIBRERÍAS---------
#include <xc.h>
#include <stdint.h>

//---------CONSTANTES---------
#define _tmr0_value 200
#define _XTAL_FREQ 8000000

//---------VARIABLES GLOBALES---------


//---------PROTOTIPOS DE FUNCIÓN---------
void setup (void);


//---------INTERRUPCIONES-----------
//void __interrupt() isr (void)
//{
//    if(T0IF)
//    {
//        //tmr0
//        PORTC++;
//        INTCONbits.T0IF = 0;
//        TMR0 = _tmr0_value;
//    }
//    return;
//}

void main(void) 
{
   setup();
    while(1){
        //POT1
        ADCON0bits.CHS = 0b0000; //AN0 para primera entrada analógica
        ADCON0bits.GO = 1; //Activar la conversión
        while(ADCON0bits.GO); // Esperar a que termine la conversión
        int adc_value_1 = ADRESH; //Guardamos el valor convertido
        PORTC = (char) adc_value_1; //Desplegar el valor en PORTC
        __delay_ms(10); //Delay
        
        //POT2
        ADCON0bits.CHS = 0b0001; //AN1 para la segunda entrada analógica
        ADCON0bits.GO = 1; //Activar la conversión
        while(ADCON0bits.GO); // Esperar a que termine la conversión
        int adc_value_2 = ADRESH; //Guardamos el valor convertido
        PORTD = (char) adc_value_2; //Desplegar el valor en PORTD
        __delay_ms(10); //Delay
    }
    return;
}

//--------CONFIGURACIÓN I/O----------------
void setup (void)
{
    //Setup I/O
    TRISA = 1; //PORTA como entrada
    TRISC = 0; //PORTC como salida
    TRISD = 0; //PORTD como salida
    
    //Configuracioón Oscilador
    OSCCONbits.IRCF = 0b111; //Frecuencia 8MHz
    OSCCONbits.SCS = 1; //Oscilador Interno
    
    //Configuración del TMR0
    OPTION_REGbits.T0CS = 0; //Tmr0 como temporizador
    OPTION_REGbits.PSA = 0; //Prescaler para tmr0
    OPTION_REGbits.PS = 0b111; //Prescaler de 256
    TMR0 = _tmr0_value; //Valor TMR0
    
    //Configuración de las interrupciones
    INTCONbits.T0IF = 0; //bandera de TMR0
    INTCONbits.T0IE = 1; //Interrupción TMR0
    INTCONbits.PEIE = 1; //Interrupción periférica para ADC
    INTCONbits.GIE = 1; //Interrupciones globales
    
    //Configuración ADC
    TRISAbits.TRISA0 = 1; //Entrada - pin0
    TRISAbits.TRISA1 = 1; //Entrada - pin1
    ANSELbits.ANS0 = 1; //Entrada analógica - pin0
    ANSELbits.ANS1 = 1; //Entrada analógica - pin1
    ADCON0bits.CHS = 0;  // Canal: 0b0000 = AN0
    __delay_ms (1); //Delay requerido
    ADCON1bits.VCFG0 = 0; //Voltaje a GND
    ADCON1bits.VCFG1 = 0; //Voltaje a 5V
    ADCON0bits.ADCS = 0b10; //Fuente de reloj: F0SC/32
    ADCON1bits.ADFM = 0; //Formato: Justificado a la izquierda
    ADCON0bits.ADON = 1; //ADC habilitado
    ADIF = 0; //Bandera de ADC apagada
    
    return; 
}