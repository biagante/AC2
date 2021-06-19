#void main(void)
#{int v = 0;
#TRISE0 = 0; // Configura o porto RE0 como saída
#while(1){LATE0 = v; // Escreve v no bit 0 do porto E
#delay(500); // Atraso de 500ms
#v ^= 1; // complementa o bit 0 de v (v = v xor 1)}}
        .equ SFR_BASE_HI, 0xBF88        #16 MSbits of SFR area
        #porto E
        .equ TRISE, 0x6100              #TRISE address is 0xBF886100
        .equ PORTE, 0x6110              #PORTE address is 0xBF886110
        .equ LATE, 0x6120               #LATE address is 0xBF886120
        
        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12

        .data
        .text
        .globl main

main:   addiu $sp,$sp,-12                   # Reservar espaço na stack
        sw $s0,0($sp)                      
        sw $s1,4($sp)                       
        sw $ra,8($sp)                       
                           
        li $s1,0                            #int v = 0;
        lui $s0, SFR_BASE_HI                
                                            #TRISE0 = 0; // Configura o porto RE0 como saída
        lw $t0, TRISE($s0)                  #READ (Read TRISE register)
        andi $t0, $t0, 0xFFFE               #MODIFY (bit0 = 0 (0 means OUTPUT))
        sw $t0, TRISE($s0)                  #WRITE (Write TRISE register)
while:                                      #while(1)
                                            
        lw $t1,LATE($s0)                    #Read LATE
        andi $t1,$t1,0xFFFE                 #LATE_bit0 = 0
        or $t1,$t1,$s1                      #LATE_bit0 = v
        sw $t1, LATE($s0)                   #WRITE LATE  #LATE0 = v; // Escreve v no bit 0 do porto E
        
        li $a0,500                          
        jal delay                           #delay(500); // Atraso de 500ms
        xor $s1,$s1,0x0001                  #v ^= 1; // complementa o bit 0 de v (v = v xor 1)
        j while                             

        lw $s0,0($sp)                       
        lw $s1,4($sp)                       
        lw $ra,8($sp)                       
        addiu $sp,$sp,12                    # Libertar  espaço na stack

        li $v0,0                            # return 0;
        jr $ra                              

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
