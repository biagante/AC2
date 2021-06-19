        .equ printStr, 8
        .equ readStr, 9
        .equ printInt, 6
        .equ printInt10, 7
        .equ STR_MAX_SIZE, 20
        .data
st0:    .asciiz "Introduza 2 strings: "
st1:    .asciiz "\nResultados:"
str1:   .space 21               #MAX_SIZE +1
str2:   .space 21               #MAX_SIZE + 1
str3:   .space 41               #2*MAX_SIZE + 1
        .text
        .globl main

main:   addiu $sp,$sp, -4
        sw $ra, 0($sp)

        la $a0, st0
        li $v0, printStr
        syscall                 #printStr("introduza 2 strings")

        la $a0, str1
        li $a1, STR_MAX_SIZE
        li $v0, readStr
        syscall                 #readStr(str1, STR_MAX)

        la $a0, str2
        li $a1, STR_MAX_SIZE
        li $v0, readStr
        syscall                 #readStr(str2, STR_MAX)
        
        la $a0, st1
        li $v0, printStr
        syscall                 #print("Resultados")

        la $a0, str1
        jal strlen              #strlen(str1)
        move $a0, $v0
        li $a1, 10
        li $v0, printInt
        syscall                 #printInt(strlen(str1), 10);

        la $a0, str2
        jal strlen              #strlen(str2)
        move $a0, $v0
        li $a1, 10
        li $v0, printInt
        syscall                 #printInt(strlen(str2), 10);

        la $a0, str3
        la $a1, str1
        jal strcpy              #strcpy(str3,str1)

        la $a0, str3
        la $a1, str2
        jal strcat              #strcat(str3, str2)
        move $a0, $v0
        li $v0, printStr
        syscall                 #printStr(strcat(..))

        la $a0, str1
        la $s1, str2
        jal strcmp              #strcmp(str1, str2)
        move $a0, $v0
        li $v0, printInt10
        syscall                 #printInt10(..);

        lw $ra, 0($sp)
        addiu $sp,$sp,4

        li $v0,0                #return 0
        jr $ra


#função strlen - Cálculo da dimensão de uma string
strlen: li $t1,0                #len = 0
stlw:   lb $t0, 0($a0)          #*s
        addiu $a0, $a0, 1       #s++
        beq $t0, 0, stlend
        addi $t1,$t1,1          #len++
        j stlw
stlend: move $v0,$t1
        jr $ra

#função strcpy - Cópia de uma string
strcpy: lb $t0, 0($a1)          #$t0 = *dst
        sb $t0, 0($a0)          #*dst = *src

        addi $a1,$a1,1          #dst++
        addi $a0,$a0,1          #src++

        bne $t0, 0, strcpy     #while(*src!=0)
        move $v0, $a0           #return dst
        jr $ra

#função strcat - Concatenação de 2 strings
strcat: addi $sp,$sp,-16
        sw $ra, 0($sp)
        sw $s0, 4($sp)
        sw $s1, 8($sp)
        sw $s2, 16($sp)

        move $s0,$a0            #p = dst
        move $s1, $a1           #dst
stcw:   lb $s2,0($s1)
        beq $s2,0,stcend
        beq $s2,'\n', stcend
        addi $s1,$s1,1          #dst++
        j stcw
stcend: move $a0, $s1
        jal strcpy              #strcpy(dst, src)
        move $v0,$s0            #return p

        lw $ra, 0($sp)
        lw $s0, 4($sp)
        lw $s1, 8($sp)
        lw $s2, 16($sp)
        addi $sp,$sp,16
        jr $ra

#função strcmp - Comparação alfabética de 2 strings
strcmp: lb $t0, 0($a0)
        lb $t1, 0($a1)

        beq $t0, $t1, scoend    #(*s1==*s2)&&
        bne $t0, 0, scoend      #(*s1!='\0');
        addi $a0, $a0,1         #s1++
        addi $a1,$a1,1          #s2++
        j strcmp

scoend: sub $v0, $t0, $t1       #return(*s1-*s2);
        jr $ra
