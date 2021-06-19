#include "detpic32.h"

void delay(int ms){
    for(; ms > 0; ms --){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}


int main(void)
 {
    int segment;
    LATDbits.LATD6 = 1; // display high active
    LATDbits.LATD5 = 0; // display low inactive

    LATB = LATB & 0x00FF;

    TRISB = TRISB & 0x00FF; // configure RB8-RB14 as outputs
    TRISD = TRISD & 0xFF9F; // configure RD5-RD6 as outputs
    while(1)
    {
        LATDbits.LATD6 = !LATDbits.LATD6; //
        LATDbits.LATD5 = !LATDbits.LATD5; // toggle display selection
        segment = 0x100;
        int i;
        for(i=0; i < 7; i++)
        {
            LATB = (LATB & 0x80FF) | segment; // send "segment" value to display
            //output para RB8 a RB14 - F000.0000.FFFF.FFFF
            delay(500); // wait 0.5 second (T = 2 Hz)
            segment = segment << 1;
        }
    }
 return 0;
 } 
