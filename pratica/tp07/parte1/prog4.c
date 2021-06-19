#include <detpic32.h>

void _int_(4) isr_T1(void)
{
    putChar('1');               // print character '1'
    IFS0bits.T1IF = 0;          // Reset T1IF flag
}
void _int_(12) isr_T3(void)
{
    putChar('3');               // print character '3'
    IFS0bits.T3IF = 0;          // Reset T3IF flag
}

int main(void)
{
    //configure Timer T1
    T3CONbits.TCKPS = 8;            //Kprescaler = 256, FoutPrescaler = 78125
    PR1 = 39062;                    //Fout = 2Hz (PBLCK / (256*(39062+1))
    TMR1 = 0;                       //Reset timer T1 count 
    T1CONbits.TON = 1;              //Enable timer T1

    //configure Timer T3
    T3CONbits.TCKPS = 7;            //Kprescaler = 32, FoutPrescaler = 62500
    PR3 = 62499;                    //Fout = 10Hz (PBLCK / (32*(62499+1))
    TMR3 = 0;                       //Reset timer T3 count 
    T3CONbits.TON = 1;              //Enable timer T3

    //configure Timer T1 interrupts
    IPC1bits.T1IP = 2;              //set priority
    IEC0bits.T1IE = 1;              //enable interrupts
    IFS0bits.T1IF = 0;              //reset interrupts flag

    //configure Timer T3 interrupts
    IPC3bits.T3IP = 2;              //set priority
    IEC0bits.T3IE = 1;              //enable interrupts
    IFS0bits.T3IF = 0;              //reset interrupts flag

    EnableInterrupts();
    while(1);
    
    return 0;
}
