// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// R2 = 0;
// 
// if (R0 >= R1) {
//     X = R0;
//     Count = R1;
// } else {
//     X = R1;
//     Count = R0;
// }
// 
// while (Count != 0) {
//     Count--;
//     R2 = R2 + X;
// }
// 

// R2 = 0;
    @0
    D=A
    @R2
    M=D

// if (R0 >= R1) {X=R0; Count=R1;} else {X=R1; Count=R0;}
    @R0
    D=M
    @R1
    D=D-M
    @xGEy
    D;JGE
    @yGTx
    0;JMP

(xGEy)
    // Initialise X
    @R0
    D=M
    @X
    M=D
    // Initialize Counter
    @R1
    D=M
    @Count
    M=D
    // Jump to Loop
    @Loop
    0;JMP

(yGTx)
    // Initialise X
    @R1
    D=M
    @X
    M=D
    // Initialise Counter
    @R0
    D=M
    @Count
    M=D
    // Jump to Loop
    @Loop
    0;JMP

// while (Count != 0) {Count--; R2=R2+X;}
(Loop)
    // If Counter is 0, jump to end
    @Count
    D=M
    @End
    D;JEQ
    // Else decrement counter
    @Count
    M=D-1
    
    // Add R0 to R2
    @X
    D=M
    @R2
    M=D+M

    // Jump to Loop
    @Loop
    0;JMP

// End
(End)
    @End
    0;JMP
