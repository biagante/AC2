# cnt1:		$t0
# cnt5:		$t1
# cnt10:	$t2
# temp:		$t3
# key:		$t4
# delay:	$t5

		.data
		.equ	READ_CORE_TIMER, 11
		.equ	RESET_CORE_TIMER, 12
		.equ	putChar, 3
		.equ	printInt, 6
		.equ	inkey, 1
		.text
		.globl main

main:	addiu $sp,$sp,-24			#reservar espaço na stack
		sw $ra, 0($sp)
		sw $s0, 4($sp)
		sw $s1, 8($sp)
		sw $s2, 12($sp)
		sw $s3, 16($sp)
		sw $s4, 20($sp)
		li $s0, 0					#cnt10 = 0
		li $s1, 0					#cnt5 = 0
		li $s2, 0					#cnt1 = 0
		li $s3, 100					#t = 100
		li $s4, 1					#flag = true
	
while:								#while(1)
in:		li $v0, inkey
		syscall

if3:	bne $v0, 'A', if4			#if(c == 'A')
		li $s3, 50					#t = 50

if4:	bne $v0, 'N', if5			#if(c == 'N')
		li $s3, 100					#t = 100

if5:	bne $v0, 'S', if6			#if(c == 'S')
		li $s4, 0					#flag = false

if6:	bne $v0, 'R', if7			#if(c == 'R')
		li $s4, 1					#flag = true

if7:	beq $s4, 1, endif			#if(flag == true)
		j in						#inkey()

endif:	move $a0, $s3
		jal	delay					#delay(100)
		
		li $a0, '\r'				
		li $v0, putChar			
		syscall						#putChar('\r');  

    	addi $s0, $s0, 1			#cnt10++
		move $a0, $s0				#$a0 = ++cnt10
		li $a1, 0x5000A				#10 | 4 << 16
		li $v0, printInt
		syscall						#printInt(++cnt10, 0x5000A);
		
if:		rem $t0, $s0, 2				#if(cnt10 % 2 == 0){//5 Hz
		bne $t0,0,if2
		addi $s1,$s1,1				#cnt5++;

if2:	rem $t0, $s0, 10			#if(cnt10 % 10 == 0){//1 Hz
		bne $t0,0, endifs
		addi $s2, $s2,1				#cnt1++

endifs: # print tab
		li	$a0, ' '
		li	$v0, putChar
		syscall						#putChar(' ')
		move $a0, $s1
		li $a1, 0x5000A 			#10 | 4 << 16
		li	$v0, printInt
		syscall						#printInt(cnt5, 0x5000A);
		
		# print tab
		li	$a0, ' '
		li	$v0, putChar			
		syscall						#putChar(' ')
		move $a0, $s2
		li $a1, 0x5000A 			#10 | 4 << 16
		li	$v0, printInt
		syscall						#printInt(cnt1, 0x5000A);
		
		j while
		li $v0, 0					#return 0
		lw $ra, 0($sp)
		lw $s0, 4($sp)
		lw $s1, 8($sp)
		lw $s2, 12($sp)
		lw $s3, 16($sp)
		lw $s4, 20($sp)
		addiu $sp,$sp,20			#repoe espaço da stack
    	jr	$ra						#termina o programa


#funcao delay -> gerar um atraso temporal programável múltiplo de 1ms
delay:  move $t0, $a0               #$t0 = ms
dfor:   li $v0, RESET_CORE_TIMER    
        syscall                     #resetCoreTimer()
dread:  li $v0, READ_CORE_TIMER
        syscall                     #readCoreTimer()
        blt $v0, 20000, dread        #while(readCoreTimer() < k)
        addi $t0,$t0,-1             #ms--
        bge $t0,0, dfor
dendf:  jr $ra                      #termina a sub-rotina
