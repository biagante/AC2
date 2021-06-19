//um contador crescente, 
//atualizado a uma frequência de 4Hz. 

#include "detpic32.h"

void delay(int ms){
    for(; ms > 0; ms --){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

int main(void)
{
    LATE = LATE & 0xFFF0;               //valor inicial antes da configuracao
    TRISE = TRISE & 0xFFF0;             //portos RE0,RE1,RE2,RE3 como outputs

    int count = 0;
    while(1){
        delay(250);                     //frequencia de 4 Hz
        LATE = (LATE & 0xFFF0) | count; //escreve valor contador registo LATE
        count++;
    }
    return 0;
}
