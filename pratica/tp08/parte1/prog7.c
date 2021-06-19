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


 void configUart(unsigned int baud, char parity, unsigned int stopbits)
 {

    if(baud < 600 || baud > 115200)
        baud = 115200;
    if(parity!='N' || parity!='E' || parity!='O')
        parity = 'N';
    if(stopbits!=1 || stopbits!=2)
        stopbits = 1;

    // Configure BaudRate Generator
    U2BRG = ((PBCLK + 8*baud)/(16*baud))-1;
    U2MODEbits.BRGH = 0;                            //16x baud clock enabled
    // Configure number of data bits (8), parity and number of stop bits
    switch (parity)
    {
    case 'E':
        U2MODEbits.PDSEL = 1;                       //8bits, even parity
        break;
    case 'O':
        U2MODEbits.PDSEL = 2;                       //8bits, odd parity
        break;
    default:
        U2MODEbits.PDSEL = 0;                       //8bits, no parity
        break;
    }

    switch (stopbits)
    {
    case 2:
        U2MODEbits.STSEL = 1;                           //2 stop bits
        break;
    
    default:
        U2MODEbits.STSEL = 0;                           //1 stop bit
        break;
    }
    
    // Enable the trasmitter and receiver modules
    U2STAbits.UTXEN = 1;                            //UART2 transmiter is enabled
    U2STAbits.URXEN = 1;                            //UART2 receiver is enabled
    // Enable UART2
    U2MODEbits.ON = 1;                              //UART2 enabled
 } 

int main(void)
{
    configUart(115200,'N',1);   //default "pterm" parameters (8 data bits)
    //configUart(57600, 'N', 1);    //baudrates de 57600 e 19200 bps
    //configUart(19200, 'N', 1);
    TRISBbits.TRISB6 = 0;       // config RB6 as output
    while(1)
    {
        LATBbits.LATB6 = 1;// Set RB6
        puts("12345");

        LATBbits.LATB6 = 0;// Reset RB6


    }
}
