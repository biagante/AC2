
 // Configure UART2:
 // 1 - Configure BaudRate Generator
 // 2 – Configure number of data bits, parity and number of stop bits
 // (see U1MODE register)
 // 3 – Enable the trasmitter and receiver modules (see register U1STA)
 // 4 – Enable UART2 (see register U1MODE)

#include "detpic32.h"

void delay(int ms){
    for(; ms > 0; ms --){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

 void putc(char byte2send)
{
    while(U2STAbits.UTXBF == 1);                    //wait while UTXBF==1
    U2TXREG = byte2send;

}

 void puts(char *str)
 {
    // use putc() function to send each charater ('\0' should not
    // be sent)
    while(*str != '\0'){
        putc(*str);
        str++;
    }
 } 


int main(void)
{
    // Configure UART2 (115200, N, 8, 1) 
    U2BRG = ((PBCLK + 8*115200)/(16*115200))-1;     //constant U2BRG, baudrate = 115200
    U2MODEbits.BRGH = 0;                            //16x baud clock enabled
    U2MODEbits.PDSEL = 0;                           //8bits, no parity
    U2MODEbits.STSEL = 1;                           //1 stop bit
    U2STAbits.UTXEN = 1;                            //UART2 transmiter is enabled
    U2STAbits.URXEN = 1;                            //UART2 receiver is enabled
    U2MODEbits.ON = 1;                              //UART2 enabled

    while(1)
    {
        puts("String de teste\n");
        delay(1000);                                //wait 1 s
    }

    return 0;
}
