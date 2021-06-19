#include <detpic32.h>

void main(void)
{
 int counter = 0;
 while(1)
 {
    resetCoreTimer();
    while(readCoreTimer() < 200000);
    printInt(++counter, 10 | 4 << 16); // Ver nota1
    putChar('\r'); // cursor regressa ao início da linha
}
 return 0;
} 