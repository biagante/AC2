#include "detpic32.h"

void delay(int ms){
    for(; ms > 0; ms --){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;

    int digit_high = display7Scodes[value >> 4];       
    int digit_low = display7Scodes[value & 0x0F];     

    if(displayFlag == 1)
    {    
        LATDbits.LATD6 = 0;                             //set display_high
        LATDbits.LATD5 = 1;
        LATB = (LATB & 0x80FF) | (digit_low << 8);      //send dh to display_high
    }
    else{    
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;                                 //set display low
    LATB = (LATB & 0x80FF) | (digit_high << 8);         //send digit_low to display_low
    }
    displayFlag = !displayFlag;                         //toggle displayFlag variable
}

volatile unsigned char voltage = 0;                     // Global variable

int main(void)
 {
    unsigned int cnt = 0;
    // Configure all (digital I/O, analog input, A/D module, interrupts)
    TRISBbits.TRISB4 = 1;           //Desligar a componente digital de saída do porto
    AD1PCFGbits.PCFG4 = 0;          //Configurar o porto como entrada analógica (AN4)

    AD1CON1bits.SSRC = 7;               
    AD1CON1bits.CLRASAM = 1;        //Parar conversões quando uma interrupção for gerada

    AD1CON3bits.SAMC = 16;              
    AD1CON2bits.SMPI = 8-1;         //Número de conversões consecutivas no canal é 1

    AD1CHSbits.CH0SA = 4;           //Selecionar AN4 como entrada para o Conversor A/D

    AD1CON1bits.ON = 1;             //ativar o conversor

    // Configure interrupt system
    IPC6bits.AD1IP = 2;             //Interruption priority is 2
    IEC1bits.AD1IE = 1;             //A/D interruptions are enabled
    IFS1bits.AD1IF = 0;             //reset AD1IF flag
    EnableInterrupts();             //Global interrupt enable

    AD1CON1bits.ASAM = 1;           // Start A/D conversion
    

    TRISB = TRISB & 0x80FF;             //RB15-RB8 as OUTPUTS
    LATB = LATB & 0x80FF;
    
    TRISD = TRISD & 0xFF9F;             //RD6-RD5 as OUTPUTS
    LATD = LATD & 0xF9FF;


    while(1)
    {
        if(cnt % 25 == 0)           // 250 ms (4 samples/second)
        {
            AD1CON1bits.ASAM = 1;   // Start A/D conversion
        }
        send2displays(toBcd(voltage & 0xFF));       // Send "voltage" variable to displays
        cnt++;
        delay(10);                  // Wait 10 ms
    }
    return 0;
}
void _int_(27) isr_adc(void)
{
    int sum = 0;
    int *p = (int *) (&ADC1BUF0);
    for(; p<= (int *)(&ADC1BUFF); p+=4) {
        sum += *p;
    }
    
    double average = (double) sum/8.0;          // Calculate buffer average (8 samples)
    voltage = (char) ((average*33)/1023);       // Calculate voltage amplitude
    // Convert voltage amplitude to decimal. Assign it to "voltage"
    IFS1bits.AD1IF = 0;                         // Reset AD1IF flag
} 

