#int main(void){
#int value;
#while(1){
#printStr("\nIntroduza um numero (sinal e módulo): ");
#value = readInt10();
#printStr("\nValor lido, em base 2: ");
#printInt(value, 2);
#printStr("\nValor lido, em base 16: ");
#printInt(value, 16);
#printStr("\nValor lido, em base 10 (unsigned): ");
#printInt(value, 10);
#printStr("\nValor lido, em base 10 (signed): ");
#printInt10(value);}
#return 0;}

        .equ printInt10,7
        .equ printInt,6
        .equ printStr,8
        .equ readInt10, 5
        .data
str1:   .asciiz "\nIntroduza um numero (sinal e módulo): "
str2:   .asciiz "\nValor lido, em base 2: "
str3:   .asciiz "\nValor lido, em base 16: "
str4:   .asciiz "\nValor lido, em base 10 (unsigned): "
str5:   .asciiz "\nValor lido, em base 10 (signed): "
        .text
        .globl main

main:   la $a0, str1
        li $v0, printStr
        syscall             #printStr("...");
        li $v0, readInt10   #readInt10
        syscall
        move $t0, $v0       #*este também não foi*

        la $a0, str2
        li $v0, printStr
        syscall             #printStr("...");
        move $a0, $t0
        li $a1,2            #printInt(value,2)
        li $v0, printInt
        syscall

        la $a0, str3
        li $v0, printStr
        syscall             #printStr("...");
        move $a0, $t0
        li $a1,16           #printInt(value,16)
        li $v0, printInt
        syscall

        la $a0, str4
        li $v0, printStr
        syscall             #printStr("...");
        move $a0, $t0
        li $a1,10           #printInt(value,10)
        li $v0, printInt
        syscall

        la $a0, str5
        li $v0, printStr
        syscall             #printStr("...");
        move $a0,$t0
        li $v0, printInt10  #printInt10(value)
        syscall

        j main
        




