#include <detpic32.h>

static int count = 1;

void _int_(12) isr_t3(void)
{
    if(count == 2){             //calling putChar at 1Hz frequency
        putChar('.');
        count = 1;
    }
    else{
        count++;
    }
    IFS0bits.T3IF = 0;          //reset T3IF interrupt flag
}

int main(void)
{
    //configure Timer T3
    T3CONbits.TCKPS = 7;            //Kprescaler = 256, FoutPrescaler = 78125
    PR3 = 39062;                    //Fout = 2Hz (PBLCK / (256*(39062+1))
    TMR3 = 0;                       //Reset timer T3 count register
    T3CONbits.TON = 1;              //Enable timer T3

    //configure Timer T3 interrupts
    IPC3bits.T3IP = 2;              //set priority
    IEC0bits.T3IE = 1;              //enable interrupts
    IFS0bits.T3IF = 0;              //reset interrupts flag
    EnableInterrupts();

    while(1);
    
}
