# flispvm
A VM emulating the FLISP (FLexible Instruction Set) processor, which is an 8-bit processor used in
introductionary courses to computer engineering on Chalmers University of Technology.

Specifications:
- 1 Accumulator register (Register A)
- 16 registers in total (including Acc)
- 256 bytes of memory

This is a silly project I started because I wanted to learn about VMs, but it also serves as a method 
for testing [flispcc](https://github.com/kjeller/flispcc), which is a C compiler for the same processor.

# Coverage
22/250 instructions implemented. 

Note: I did not aim to implement all of these when I started this project. Feel free to add more via pull requests.
