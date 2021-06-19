#include "detpic32.h"

void delay(int ms){
    for(; ms > 0; ms --){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

// Interrupt Handler
 void _int_(27) isr_adc(void) // Replace VECTOR by the A/D vector number (27)
 {
    printInt(ADC1BUF0, 16 | 3 << 6);// Print ADC1BUF0 value // Hexadecimal (3 digits format)
    printStr(" "); 
    delay(500);
    AD1CON1bits.ASAM = 1;// Start A/D conversion
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
 } 

int main(void)
 {
    // Configure all (digital I/O, analog input, A/D module)
    TRISBbits.TRISB4 = 1;           //Desligar a componente digital de saída do porto
    AD1PCFGbits.PCFG4 = 0;          //Configurar o porto como entrada analógica (AN4)

    AD1CON1bits.SSRC = 7;               
    AD1CON1bits.CLRASAM = 1;        //Parar conversões quando uma interrupção for gerada

    AD1CON3bits.SAMC = 16;              
    AD1CON2bits.SMPI = 1-1;         //Número de conversões consecutivas no canal é 1

    AD1CHSbits.CH0SA = 4;           //Selecionar AN4 como entrada para o Conversor A/D

    AD1CON1bits.ON = 1;             //ativar o conversor

    // Configure interrupt system
    IPC6bits.AD1IP = 2;             //Interruption priority is 2
    IEC1bits.AD1IE = 1;             //A/D interruptions are enabled
    IFS1bits.AD1IF = 0;             //reset AD1IF flag
    EnableInterrupts();             //Global interrupt enable

    AD1CON1bits.ASAM = 1;           // Start A/D conversion
    
    while(1) { }                    // all activity is done by the ISR
    return 0;
 } 
