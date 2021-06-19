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

char getc(void)
{
    // If OERR == 1 then reset OERR
    if(U2STAbits.OERR == 1){
        U2STAbits.OERR = 0;
    }
    // Wait while URXDA == 0
    while(U2STAbits.URXDA == 0);
    // If FERR or PERR then
    char a;
    if(U2STAbits.FERR == 1 || U2STAbits.PERR){
        a = U2RXREG;   //read UxRXREG (to discard the character) and return 0
        return 0;
    }
    else{
        return U2RXREG;
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
    configUart(115200,'N',1); // default "pterm" parameters
    while(1)
    {
        putc( getc() );
    }
    return 0;
}
