#Contador Johnson de 4 bits com deslocamento à 
#esquerda ou à direita, dependendo do valor lido
#do porto de entrada RB2: se RB2=1, deslocamento 
#à esquerda; frequência de atualização de 1.5 Hz.

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

main:	addiu $sp,$sp,-20              #reservar espaço na stack
	    sw $ra, 0($sp)
	    sw $s0, 4($sp)
	    sw $s1, 8($sp)
        sw $s2, 12($sp)
        sw $s3, 16($sp)

        lui $s0, SFR_BASE_HI            #
        lw $t1, TRISE($s0)              #READ (Mem_addr = 0xBF880000 + 0x6100)
        andi $t1, $t1, 0xFFF0           #TRISE[3..0] = 0;
        sw $t1, TRISE($s0)              #WRITE (Write TRISE register)
        
        lw $s1, TRISB($s0)              # READ (Mem_addr = 0xBF880000 + 0x6040)
        andi $s1, $s1, 0x000F           #MODIFY/bit0=bit1=bit2=bit3=1 Input)
        sw $s1, TRISB($s0)              #WRITE (Write TRISB register)
        
        li $s2, 0                       #count = 0

while:  lw $s1, PORTB($s0)               #ler valor do LATE 
        andi $s1, $s1, 0x0004            #extrair bit 2

        bne $s1, 0x0004, right          #if(RB[2] == 1)
        andi $s3, $s2, 0x0008           #MSB
        sll $s2,$s2, 1                  #count << 1
        andi $s2,$s2, 0xFFFE

if1l:	bne $s3, 0x8, if0l		        #if(bit[3] = 1)
	    ori $s2, $s2, 0x0000		    #cnt[bit0]=0;
	    j endl			
if0l:	ori $s2, $s2, 0x0001		    #else{ cnt[bit0]=1 }
endl:	j end				
					
right:	andi $s3, $s2, 0x1		        #else if(rb2==0)
	    srl  $s2, $s2, 1		        #cnt = cnt>>1;
	    andi $s2, $s2, 0xFFF7		    #manter todos os bits excepto o que se pretende mudar
if1:	bne $s3, 0x1, if0		        #if(bit0 = 1)
	    ori $s2, $s2, 0x0000		    #cnt[bit3]=0;
	    j   end				            
if0:	ori $s2, $s2, 0x0008		    #else{ cnt[bit3]=1 }

end:	lw  $s1, LATE($s0)		        #ler valores presentes noporto de saída
	    andi $s1, $s1, 0xFFF0		    #"extrair" bits0,1,2,3
	    or  $s1, $s1, $s2		        #colocar o valor de cnt nos bits0,1,2,3
	    sw  $s1, LATE($s0)		        #escrever valores alterados no porto de saída
	    li  $a0, 667			        #delay(667); frequencia de 1.5Hz
	    jal delay			
	    j while			

endw:   lw $ra, 0($sp)
	    lw $s0, 4($sp)
	    lw $s1, 8($sp)
        lw $s2, 12($sp)
        lw $s3, 16($sp)
	    addiu $sp,$sp,20                #repoe espaço da stack
        jr $ra				            #termina o programa


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
