
 // Configure UART2:
 // 1 - Configure BaudRate Generator
 // 2 – Configure number of data bits, parity and number of stop bits
 // (see U1MODE register)
 // 3 – Enable the trasmitter and receiver modules (see register U1STA)
 // 4 – Enable UART2 (see register U1MODE)

#include "detpic32.h"

 void configUart(unsigned int baud, char parity, unsigned int stopbits)
 {
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

