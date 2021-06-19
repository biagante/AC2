#Configure agora os portos RB0 a RB3 como entradas e os portos RE0 a RE3 como saídas
#e, em ciclo infinito, faça as seguintes atribuições nas saídas:
#RE0 = RB0\, RE1 = RB1, RE2 = RB2 e RE3 = RB3\
        .equ SFR_BASE_HI, 0xBF88    #16 MSbits of SFR area
        #porto E
        .equ TRISE, 0x6100          #TRISE address is 0xBF886100
        .equ PORTE, 0x6110          #PORTE address is 0xBF886110
        .equ LATE, 0x6120           #LATE address is 0xBF886120
        #porto B
        .equ TRISB, 0x6040          #TRISB address is 0xBF886040
        .equ PORTB, 0x6050          #PORTB address is 0xBF886050
        .equ LATB, 0x6060           #PORTB address is 0xBF886060

        .data
        .text
        .globl main

main:   lui $t1, SFR_BASE_HI        #
        lw $t2, TRISE($t1)          #READ (Mem_addr = 0xBF880000 + 0x6100)
        andi $t2, $t2, 0xFFF0       #MODIFY (bit0=bit1=bit2=bit3=0 (0 means OUTPUT))
        sw $t2, TRISE($t1)          #WRITE (Write TRISE register)

        lw $t2, TRISB($t1)          #READ (Read LATB register)
        andi $t2, $t2, 0x000F       #MODIFY (bit0=bit1=bit2=bit3=1 (1 means INPUT))
        sw $t2, TRISB($t1)          #WRITE (Write TRISB register)

while:  lw $t2, PORTB($t1)          #ler valor entrada
        andi $t2, $t2, 0x000F       #extrair bits 0, 1, 2 e 3
        xor $t2,$t2,0x0009          #forçar RB1 e RB2 (RE0.RE1.RE2.RE3 = RB0\.RB1.RB2.RB3\)
        lw $t3, LATE($t1)           #ler valor do LATE (porto de saida)
        andi $t3, $t3, 0xFFF0       #alterar bits 0, 1, 2 e 3
        or $t2, $t3, $t2            #guardar o bit 0 de RB0 em RE0
        sw $t2, LATE($t1)           #escrever valor no porto saida
        j while

        jr $ra                      #termina o programa
