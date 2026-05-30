# Psammite Devlog

## [28/5/26] Foundation
A long-time itch of mine is building a simple virtual machine. Computers have always fascinated me, not in an abstract way. I'm deeply drawn to the way they work, and on a journey to understand them better, I will try my hand at making my very own VM. My main goals are:
* Coding a modern, fully capable VM using as few abstractions as possible.
* To do it on my own. It is okay to read and ask, but not to blindly follow tutorials.

I will make a 64-bit word size, 32-bit fixed-width instruction, register-based VM, to follow modern architectures. I plan to take some inspiration from RISC architectures. One important point I decided early is the number of registers in the VM: it will have 32 integer registers, 32 float registers (as in real machines, they are separate), and one special register for the Program Counter.

I find names inspiring, and having a name for the project motivates me to continue it. I spent a little time thinking of one, and after a while, I landed on "Psammite", which is a broad category for sandstone and its derivatives. I felt drawn to it as it is a simple material, yet reliable, and those are the two values I want to represent the most in this project: it must work simply and without problems. Psammite won't be a beautiful, state-of-the-art VM, but I plan to have a polished and working product, using it as a learning platform and a stepping stone for harder projects down the line.

I will develop the project in my spare time, and will do it in three different stages:
* Working VM
* Assembler
* A simple programming language native to Psammite

If you happen to stumble upon my project, Hi!!!
I hope you come back to see the development of Psammite!

## [30/5/26] Designing the ISA

One of the most fundamental design decisions when writing a VM is the Instruction Set Architecture: it consists of the instructions available to the CPU, or in this case, our virtual CPU, and their format. The first choice that needs to be made is whether the instructions are fixed or not in size. In the case of Psammite, as mentioned in the previous entry, the plan is to use fixed 32-bit instructions. The reasoning behind this choice is that, while fixed-size instructions waste space, they are simpler to decode and faster to run, which aligns with my main goal: building a simple and relatively performant VM. Additionally, like many other architectures, I chose 32-bit because it is the sweet spot between having enough space for our instructions while not wasting too many bits.

I have to admit I'm not an expert of any kind in VM development and architecture design, so I will write the instructions incrementally as I need them, and I will probably make some inefficient or suboptimal decisions. The main goal of this project is learning, so I find it perfectly fine to make mistakes and fix them. Copying a well-designed architecture to the letter would be much easier, but it would undermine the whole goal.

What are the first mini-goals or milestones for the ISA and the VM as a whole?
* Loading values to registers, as both immediates and from a constant pool
* Storing values to RAM
* Performing basic arithmetic operations

To make writing the VM and debugging easier, VM dumping will also be implemented.

### Op-codes

The instruction set isn't going to be big; in general, the project will follow the RISC psychology. For that reason, only the first 6 bits for our instructions will be used for the OP-codes, which allows for 64 instructions, which I find plenty for the project's goals.
To accomplish our first milestones, we need two different categories of layouts:

#### R-type instructions

R-type instructions are register-to-register operations, in many cases arithmetic operations. I took some inspiration from RISC-V with the function elements of the opcode, as this allows us to merge several operations in a single opcode. Given that we only have 64 opcode slots, it is a very needed optimization.

Two different function fields exist to quickly match a 4-bit value, and then a 7-bit value; instead of matching 11 bits at a time, which would slow the program, especially if many codes aren't in use.

[6-bit opcode][5-bit source register one][5-bit source register two][5-bit destination register][4-bit category][7-bit specific operation]

#### I-type instructions

I-type instructions work with immediate values. We will have 3 variations to this layout:

##### I1
[6-bit opcode][5-bit register][21-bit ram address, offset by the PC's current position]

This layout will be used to load values from RAM into registers.
If the 21 bits aren't enough, separate I2 instructions will use register values for ram access.

##### I2
[6-bit opcode][5-bit source register][5-bit destination register][16-bit immediate]

This layout will be used to perform immediate operations and register loading. Load immediate will probably be an alias of addi.

##### Chunk
[6-bit opcode][5-bit target register][2-bit chunk selector (to form 64-bit numbers)][3-bit padding][16-bit immediate]

### Starting instructions
#### Memory related
* LDC (Chunk) - forms 64-bit numbers from immediates
* LDR (I1) - load PC-relative values from RAM, with a 21-bit range
* LD (I2) - load RAM from an address stored in a register and put it in another register
* SD (I2) - store a value from a register in an address in RAM stored in another register

#### Arithmetic
* EXECUTE (R)

As the EXECUTE opcode has capability for many functions as an R-type instruction, here are the first I will implement:

Under the 4-bit category `0001` is going to be integer math. Inside of it we will have the following basic instructions, identified by the 7-bit function field:
* ADD
* SUB
* MUL
* DIV
* MOD

The 4-bit category `0000` is for syscalls, and inside its 7-bit function field:
* HALT

### The plan and some notes
I am going to set up CMake, write all the code to have the above functioning with a Fetch-Decode-Execute cycle. Then, after everything is working, I will go back to the README.md, update the specification, and start planning the next additions to the ISA. I originally planned to have a separate syscall opcode, imath, and fmath; but by merging them we use the space that would otherwise remain empty inside each, and leave space for more opcodes.

If you have been paying attention, you may have noticed the README.md mentions a 64kb RAM. Why would Psammite need something other than LDR and a hypothetical SDR? Everything is in range of LDR right now. You are completely right, but I do not intend to leave RAM at that size, so I'm planning ahead.

See you soon!
