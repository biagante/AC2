        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .equ PUT_CHAR,3
        .equ PRINT_INT,6
        .data
        .text
        .globl main

main:   addiu $sp,$sp,-8
        sw $ra,0($sp)
        sw $s0,4($sp)
        li $s0,0
        li $t1,0                    #counter = 0;

while:  li $a0,100                  #freq = 10 Hz
       #li $a0,200                  #freq = 5 Hz
       #li $a0, 1000                #freq = 1 Hz
        jal delay
        addi $t1,$t1,1              #counter++
        move $a0, $t0
        li $a1,0x0004000A           #10 | 4 << 16 = 0x0004000A
        li $v0, PRINT_INT           #printInt(++counter)
        syscall
        li $a0, '\r'
        li $v0, PUT_CHAR            #putChar('\r'); 
        syscall
        j while
        lw $ra, 0($sp)
        lw $s0, 4($sp)
        addiu $sp,$sp,8
        jr $ra                      #termina o programa

#funcao delay -> gerar um atraso temporal programável múltiplo de 1ms
delay:  move $t0, $a0               #$t0 = ms
dfor:   li $v0, RESET_CORE_TIMER    
        syscall                     #resetCoreTimer()
dread:  li $v0, READ_CORE_TIMER
        syscall                     #readCoreTimer()
        blt $v0, 20000, dread       #while(readCoreTimer() < k)
        addi $t0,$t0,-1             #ms--
        bge $t0,0, dfor
dendf:  jr $ra                      #termina a sub-rotina

