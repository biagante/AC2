#include "detpic32.h"

void delay(int ms){
    for(; ms > 0; ms --){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;

    int digit_high = display7Scodes[value >> 4];       
    int digit_low = display7Scodes[value & 0x0F];     

    if(displayFlag == 0)
    {    
        LATDbits.LATD6 = 1;                         //set display_high
        LATDbits.LATD5 = 0;
        LATB = (LATB & 0x80FF) | (digit_low << 8);       //send dh to display_high
    }
    else{    
    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 1;                         //set display low
    LATB = (LATB & 0x80FF) | (digit_high << 8);         //send digit_low to display_low
    }
    displayFlag = !displayFlag;     //toggle displayFlag variable
}

int main(void)
{
    unsigned int counter;
    unsigned int i;

    TRISD = TRISD & 0xFF9F;           //configure RD5, RD6 as outputs
    TRISB = TRISB & 0x80FF;          //configure RB14 to RB8 as outputs

    counter = 0;
    i = 0;
    while(1)
    {   
        i++;
        send2displays(counter);
        delay(10);
        if(i % 4 == 0){
            counter++;
        }
    }             
    return 0;
}

