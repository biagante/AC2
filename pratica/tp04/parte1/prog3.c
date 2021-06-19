#include "detpic32.h"

int main(void){
    LATB = LATB & 0x00FF; //valor inicial antes da configuracao
    LATD = LATD & 0xFF9F; 

    TRISB = TRISB & 0x00FF; //0 para output ->0000.0000.FFFF.FFFF
    TRISD = TRISD & 0xFF9F; // FFFF.FFFF.F00F.FFFF

    //dislay low
    LATDbits.LATD5 = 1; //RD5 = 1
    LATDbits.LATD6 = 0; //RD6 = 0
    //display high (RD6=1 e RD5=0)
    //LATDbits.LATD5 = 0; //RD5 = 0
    //LATDbits.LATD6 = 1; //RD6 = 1    

    while(1)
    {
        char kb = getChar();

        switch(kb)
        {
            case 'a' | 'A':
            LATB = LATB & 0x00FF;
            LATBbits.LATB8 = 1;
            break;

            case 'b' | 'B':
            LATB = LATB & 0x00FF;
            LATBbits.LATB9 = 1;
            break;

            case 'c' | 'C':
            LATB = LATB & 0x00FF;
            LATBbits.LATB10 = 1;
            break;

            case 'd' | 'D':
            LATB = LATB & 0x00FF;
            LATBbits.LATB11 = 1;
            break;

            case 'e' | 'E':
            LATB = LATB & 0x00FF;
            LATBbits.LATB12 = 1;
            break;

            case 'f' | 'F':
            LATB = LATB & 0x00FF;
            LATBbits.LATB13 = 1;
            break;

            case 'g' | 'G':
            LATB = LATB & 0x00FF;
            LATBbits.LATB14 = 1;
            break;

            default:
            LATB = LATB & 0x00FF;
            break;
        }  
    }
    return 0;
}
