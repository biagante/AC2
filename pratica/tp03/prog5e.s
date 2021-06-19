#Contador Johnson de 4 bits com deslocamento à direita 
#(sequência: 0000, 1000, 1100,
#1110, 1111, 0111, 0011, 0001, 0000, 1000, ...);
#frequência de atualização de 1.5 Hz

        .equ SFR_BASE_HI, 0xBF88        #16 MSbits of SFR area
        #porto E
        .equ TRISE, 0x6100              #TRISE address is 0xBF886100
        .equ PORTE, 0x6110              #PORTE address is 0xBF886110
        .equ LATE, 0x6120               #LATE address is 0xBF886120
        #porto B
        .equ TRISB,0x6040               #TRISB address
        .equ PORTB, 0x6050              #PORTB address

        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12

        .data
        .text
        .globl main

main:	addiu $sp,$sp,-12               #reservar espaço na stack
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)

        lui $s0, SFR_BASE_HI            #
        lw $t1, TRISE($s0)              #READ (Mem_addr = 0xBF880000 + 0x6100)
        andi $t1, $t1, 0xFFF0           #TRISE[3..0] = 0;
        sw $t1, TRISE($s0)              #WRITE (Write TRISE register)
        li $s1, 0                       #count = 0

while:  lw $t1, LATE($s0)               #ler valor do LATE 
        andi $t1, $t1, 0xFFF0           #RE[3..0] = 0;
        or $t1, $s1, $t1                #RE[3..0] = count;
        sw $t1, LATE($s0)               #escrever valor no porto saída
        li $a0, 667
	jal delay			#delay(667) -> 1.5 Hz [1/(1.5 Hz) = 0.6667 s]
	

        andi $t2, $t1, 0x0001           #$t2 = RE[3]
        sll $t2,$t2, 0x0003             #LSB 
        srl $s1,$s1, 1                  #count = count << 1
        xori $t2,$t2, 0x0008            #$t2 = RE3\
        or $s1,$s1, $t2                 #first bit of count
        andi $s1,$s1, 0x000F            #count &= 0x000F
        j while
		
        lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	addiu $sp,$sp,12                #repoe espaço da stack
    	jr $ra				#termina o programa

#funcao delay -> gerar um atraso temporal programável múltiplo de 1ms
delay:  move $t0, $a0                   #$t0 = ms
dfor:   li $v0, RESET_CORE_TIMER    
        syscall                         #resetCoreTimer()
dread:  li $v0, READ_CORE_TIMER
        syscall                         #readCoreTimer()
        blt $v0, 20000, dread           #while(readCoreTimer() < k)
        addi $t0,$t0,-1                 #ms--
        bge $t0,0, dfor
dendf:  jr $ra                          #termina a sub-rotina
