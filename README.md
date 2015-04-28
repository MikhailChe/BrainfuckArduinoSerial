# BrainfuckAruinoSerial
Brainfuck interpreter for arduino with command line interface. Serial commuinaction is used to control it.

Currently implemented commands:
 * load -- starts a loading procedure. Just write in some brainfuck and press Ctrl+c to finish
 * exec -- start executing a brainfuck code from the begining. Code and data pointers are being reset automaticly before execution
 * step -- execute one instruction at IP
 * debug -- turn on debugging output, like IP (instruction pointer), DP (data pointer) and its values
 * reset -- resets all code and data memory, reset code and data pointers
 * resetptr -- resets code and data pointers
 

TODO:
 * Command line interface is still not perfect. Backspace, delete and cursor movement are not yet 100% implemented.
 * Really want to do commands history (at least for one session).
