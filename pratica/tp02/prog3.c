#include <detpic32.h>

void delay(int ms);

int main(void)
{
    int cnt1 = 0;
    int cnt5 = 0;
    int cnt10 = 0;

    while(1){
        delay(100);
        if(cnt10 % 2 == 0){//5 Hz
            cnt5++;
        }
        if(cnt10 % 10 == 0){//1 Hz
            cnt1++;
        }
        printInt(cnt1, 0x5000A);
        putChar(' ');
        printInt(cnt5, 0x5000A);
        putChar(' ');
        printInt(cnt10, 0x5000A);
        cnt10++;
        putChar('\r');      
    }
    return 0;
}

void delay(int ms){
    for(; ms > 0; ms --){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}