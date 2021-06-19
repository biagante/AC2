#include <detpic32.h>

void delay(int ms)
{
 for(; ms > 0; ms--)
 {
 resetCoreTimer();
 while(readCoreTimer() < K);
 }
} 