#include <detpic32.h>

int main(void)
{
    // Configure Timer T3 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 7;            //Kprescaler = 256, FoutPrescaler = 78125
    PR3 = 39062;                    //Fout = 2Hz (PBLCK / (256*(39062+1))
    TMR3 = 0;                       //Reset timer T3 count register
    T3CONbits.TON = 1;              //Enable timer T3

    while(1)
    {
        while(IFS0bits.T3IF == 0);  // Wait until T3IF = 1
        IFS0bits.T3IF = 0;          // Reset T3IF
        putChar('.');
    }

    return 0;
}
