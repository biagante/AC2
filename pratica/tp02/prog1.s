        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .equ PUT_CHAR,3
        .equ PRINT_INT,6
        .data
        .text
        .globl main
        
main:   li $t0,0                    #int counter = 0;
while:  li $v0, RESET_CORE_TIMER    #resetCoreTimer(); 
        syscall     
while2: li $v0,READ_CORE_TIMER      # while (1) {
        syscall                     #
        blt $v0,10000000,while      #while(readCoreTimer() < 10M);
       #blt $v0,2000000,while      #2000000 -> 10Hz
       #blt $v0,4000000,while      #2000000 -> 5Hz
       #blt $v0,20000000,while     #2000000 -> 1Hz
        addi $t0,$t0,1              #counter++
        move $a0, $t0
        li $a1,0x0004000A           #10 | 4 << 16 = 0x0004000A
        li $v0, PRINT_INT           #printInt(++counter)
        syscall
        li $a0, '\r'
        li $v0, PUT_CHAR            #putChar('\r'); 
        syscall
        j while2
        li $v0, 0                   #return 0
        jr $ra                      #termina o programa
