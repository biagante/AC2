        .equ putChar, 3
        .equ printInt, 6
        .equ inkey, 1
        .data
        .text
        .globl main

main:   addiu $sp,$sp, -16
        sw $ra, 0($sp)
        sw $s0, 4($sp)
        sw $s1, 8($sp)
        sw $s2, 16($sp)

        li $s0, 0               #s = 0
        li $s1, 0               #cnt = 0

do:     li $a0, '\r'
        li $v0, putChar
        syscall                 #putChar('\r')
        li $a1, 0x0003000A
        li $v0, printInt
        syscall                 #printInt( cnt, 10 | 3 << 16 );
        li $a0, '\t'
        li $v0, putChar
        syscall                 #putChar('\t')
        li $a1, 0x00080002
        li $v0, printInt 
        syscall                 #printInt( cnt, 2 | 8 << 16 ); 

        li $a0, 5
        jal wait                #wait(5)

        li $v0, inkey
        syscall                 #c = inkey()
        move $s2, $v0

if_s:   bne $s2, 's', if_r      #if( c == 's' )
        li $s1, 0               #cnt = 0 (pára o contador)

if_r:   bne $s2, 'r', if_p      #if( c == 'r' )
        j do                    #reinicia

if_p:   bne $s2, '+', if_l      #if( c == '+' )
        move $s0, $0            #s = 0

if_l:   bne $s2, '-', if_0      #if( c == '-' )
        li $s0, 1               #s = 1

if_0:   bne $s2, '0',else       #if( s == 0 )
        addi $s1,$s1,1          #cnt+1
        andi $s1,$s1, 0xFF       #cnt = (cnt + 1) & 0xFF;

else:   sub $s1,$s1,1           #cnt-1
        andi $s1,$s1, 0xFF      #cnt = (cnt - 1) & 0xFF;
        bne $s2, 'q', do

        lw $ra, 0($sp)
        lw $s0, 4($sp)
        lw $s1, 8($sp)
        lw $s2, 16($sp)
        addiu $sp,$sp, 16
        li $v0,0                #return 0
        jr $ra                  


#funcção wait(int ts)
# ts -> $a1
# i  -> $t0
wait:   li $t0, 0               #i = 0;

wfor:   mul $a0, $a0, 515000   #515000 * ts
        bge $a0, $t0, wend      # i < multiplicação
        addi $t0,$t0,1          #i++

wend:   jr $ra
