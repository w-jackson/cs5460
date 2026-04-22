# Part 1

## freerange() stack frame
1. 0x0000080000102da9
- Return address of kinit1 pushed when freerange is called.

## kinit1() stack frame
2. 0x00000000000100e8
- The value of `rbx` pushed by kinit1. `rbx` is callee saved, so kinit1 must push to the stack before using it.

3. 0x0000000000000000
- The value of `r12` pushed by kinit1. `r12` is callee saved, so kinit1 must push to the stack before using it. 

4. 0x000008000010fad8
- This is the value of `rbp` pushed during the prologue of kinit1.

5. 0x0000080000103ab7
- Return address of main pushed when kinit is called.

## main() stack frame
6. 0x0000000000007bf8
- This is the value of `rbp` pushed during the prologue of main.

## Values past the stack
7. 0x0000000000000000
8. 0x0000000000000000
9. 0x0000000000000000
10. 0x0000000000000000
11. 0x0000000000000000
12. 0x0000000000000000
