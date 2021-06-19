#include <detpic32.h>

volatile int voltage;

void setPWM(unsigned int dutyCycle)
{
    if(dutyCycle > 100){// duty_cycle must be in the range [0, 100]
        return;
    }
    else{
        OC1RS = 500*dutyCycle; // OC1RS = (PR3+1)*(dutycycle/100) = 500*dutycyle
    }
}

unsigned char toBcd(unsigned char value)
{
    return ((value/10)<<4) + (value%10);
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


void configureAll(){
    // configure all (digital I/O, analog
    // input, A/D module, timers T1 and T3, interrupts)
    // Reset AD1IF, T1IF and T3IF flags
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

    //configura I/O
    TRISB = TRISB & 0x80FF;             //RB15-RB8 as OUTPUTS
    LATB = LATB & 0x80FF;
    
    TRISD = TRISD & 0xFF9F;             //RD6-RD5 as OUTPUTS
    LATD = LATD & 0xF9FF;

    //configure Timer T1
    T3CONbits.TCKPS = 3;            //Kprescaler = 256, FoutPrescaler = 78125
    PR1 = 19530;                    //Fout = 4Hz (PBLCK / (256*(19530+1))
    TMR1 = 0;                       //Reset timer T1 count 
    
    //configure Timer T1 interrupts
    IPC1bits.T1IP = 2;              //set priority
    IEC0bits.T1IE = 1;              //enable interrupts
    IFS0bits.T1IF = 0;              //reset interrupts flag

    T1CONbits.TON = 1;              //Enable timer T1

    //configure Timer T3
    T3CONbits.TCKPS = 2;            //Kprescaler = 32, FoutPrescaler = 62500
    PR3 = 49999;                    //Fout = 10Hz (PBLCK / (32*(62499+1))
    TMR3 = 0;                       //Reset timer T3 count 
    
    //configure Timer T3 interrupts
    IPC3bits.T3IP = 2;              //set priority
    IEC0bits.T3IE = 1;              //enable interrupts
    IFS0bits.T3IF = 0;              //reset interrupts flag

    T3CONbits.TON = 1;              //Enable timer T3

}

int main(void)
{
    int dutyCycle;
    int portVal;
    configureAll();
    EnableInterrupts(); // Global Interrupt Enable
    
    while(1)    
    {
        portVal = PORTB & 0x0003; // Read RB1, RB0 to the variable "portVal"
        
        switch(portVal)
        {
            case 0: // Measure input voltage
                IEC0bits.T1IE = 1;  // Enable T1 interrupts
                setPWM(0);          // LED OFF
                break;
            case 1: // Freeze
                IEC0bits.T1IE = 0;  // Disable T1 interrupts
                setPWM(100);        // LED ON (maximum bright)
                break;
            default: // LED brigthness control
                IEC0bits.T1IE = 1;  // Enable T1 interrupts
                dutyCycle = voltage * 3;
                setPWM(dutyCycle);
                break;
        }
    }
    return 0;
}

void _int_(4) isr_T1(void)
{
    AD1CON1bits.ASAM = 1;   // Start A/D conversion
    IFS0bits.T1IF = 0;      // Reset T1IF flag
}

void _int_(12) isr_T3(void)
{
    send2displays(voltage);   // Send "voltage" global variable to displays
    printInt(ADC1BUF0, 16 | 3 << 6);// Print ADC1BUF0 value // Hexadecimal (3 digits format)
    printStr("\n");
    IFS0bits.T3IF = 0;      // Reset T3IF flag
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
    voltage = toBcd(voltage & 0xFF);// Convert voltage amplitude to decimal. Assign it to "voltage"
    IFS1bits.AD1IF = 0;                         // Reset AD1IF flag
} 
