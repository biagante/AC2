#int main(void){
#char c;
#do{
#while( (c = inkey()) == 0 );
#if( c != '\n' )
#putChar( c );
#} while( c != '\n' );
#return 0;

        .equ inkey,1
        .equ putChar,3
        .data
        .text
        .globl main

main:   li $v0, inkey           #c = inkey())
        syscall                 #*e deste também much big brain srsl*
        beq $v0, 0, main        #if (inkey() == 0)
        beq $v0,'\n',end        #if (inkey() != "\n")

        move $a0, $v0           #*tinha-me esquecido disto uau*
        li $v0, putChar
        syscall                 #putChar( c );

        j main

end:    li $v0,0
        jr $ra
        