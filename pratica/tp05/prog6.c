#include "detpic32.h"

int main(void){
    volatile int aux;
    TRISEbits.TRISE0 = 0;               //RE0 as digital output
    
    TRISBbits.TRISB4 = 1;               //Desligar a componente digital de saída do porto
    AD1PCFGbits.PCFG4 = 0;              //Configurar o porto como entrada analógica (AN4)

    AD1CON1bits.SSRC = 7;               
    AD1CON1bits.CLRASAM = 1;            //Parar conversões quando uma interrupção for gerada

    AD1CON3bits.SAMC = 16;              
    AD1CON2bits.SMPI = 1-1;             //Número de conversões consecutivas no canal é 1

    AD1CHSbits.CH0SA = 4;               //Selecionar AN4 como entrada para o Conversor A/D

    AD1CON1bits.ON = 1;                 //ativar o conversor

    while(1)
    {
        LATEbits.LATE0 = 1;             //set LATE0
        AD1CON1bits.ASAM = 1;           //start conversion
        while(IFS1bits.AD1IF == 0)      //wait while conversion not done
        {
            LATEbits.LATE0 = 0;         //reset LATE0
            aux = ADC1BUF0;             //store conversion result in aux variable
            IFS1bits.AD1IF = 0;         //reset AD1IF
        }                  
    }
    return 0;

}
