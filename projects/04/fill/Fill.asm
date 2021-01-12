// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// @ScreenEmpty = 1;
// 
// Main
// While 1 {
//     if (@Keyboard != 0) {
//         if (@ScreenEmpty != 0) {continue}
//         for (pixel in @Screen) {
//             pixel = 1;
//         }
//         @ScreenEmpty = 1;
//     } else {
//         if (@ScreenEmpty == 0) {continue}
//         for (pixel in @Screen) {
//             pixel = 0;
//         }
//         @ScreenEmpty = 0;
//     }
// }

    // Initialise ScreenEmpty variable to 0
    @ScreenEmpty
    M=0

    // Initialise n to be number of bytes in memory map
    @8192
    D=A
    @n
    M=D

(Main)
    @KBD
    D=M
    @FillScreen
    D;JNE
    @EmptyScreen
    0;JMP

(FillScreen)
    // if (ScreenEmpty != 0) {Continue Main Loop;}
    @ScreenEmpty
    D=M
    @Main
    D;JNE

    // Set ScreenEmpty to 1
    @ScreenEmpty
    M=1
    
    // Set i to 0
    @i
    M=0

(FillScreenLoop)
    // Break to Main when i == n
    @i 
    D=M
    @n
    D=M-D
    @Main
    D;JEQ

    // Select Byte using i 
    @SCREEN
    D=A
    @i
    A=D+M
    // Set selected byte to 11111111
    M=-1

    // i++
    @i
    M=M+1
    
    // Jump to FillScreenLoop
    @FillScreenLoop    
    0;JMP

(EmptyScreen)
    // if (ScreenEmpty == 0) {Continue Main Loop;}
    @ScreenEmpty
    D=M
    @Main
    D;JEQ

    // Set ScreenEmpty to 0
    @ScreenEmpty
    M=0
    
    // Set i to 0
    @i
    M=0

(EmptyScreenLoop)
    // Break to Main when i == n
    @i 
    D=M
    @n
    D=M-D
    @Main
    D;JEQ

    // Select Byte using i 
    @SCREEN
    D=A
    @i
    A=D+M
    // Set selected byte to 00000000
    M=0

    // i++
    @i
    M=M+1
    
    // Jump to EmptyScreenLoop
    @EmptyScreenLoop    
    0;JMP
