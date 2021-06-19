#int main(void){
#char c;
#do{ 
#c = getChar();
#if( c != '\n' )
#putChar( c );
#} while( c != '\n' );
#return 0;}

        .equ getChar, 2
        .equ putChar, 3
        .data
        .text
        .globl main
main:   
do:     la $v0,getChar      
        syscall                 #getChar();
    
if:     beq $v0,'\n',end        #if( c != '\n' )
        move $a0,$v0       

        li $v0, putChar
        syscall                 #putChar( c );

        j do                    #} while( c != '\n' );
        
end:    li $v0,0                # return 0;
        jr $ra
