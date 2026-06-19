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
### Opcodes

The instruction set isn't going to be big; in general, the project will follow the RISC psychology. For that reason, only the first 6 bits for our instructions will be used for the opcodes, which allows for 64 instructions, which I find plenty for the project's goals.
To accomplish our first milestones, we need two different categories of layouts:
#### R-type instructions

R-type instructions are register-to-register operations, in many cases arithmetic operations. I took some inspiration from RISC-V with the function elements of the opcode, as this allows us to merge several operations in a single opcode. Given that we only have 64 opcode slots, it is a very needed optimization.

Two different function fields exist to quickly match a 4-bit value, and then a 7-bit value; instead of matching 11 bits at a time, which would slow the program, especially if many codes aren't in use.

[6-bit opcode][5-bit source register one][5-bit source register two][5-bit destination register][4-bit category][7-bit specific operation]
#### I-type instructions

I-type instructions work with immediate values. We will have 3 variations to this layout:

#### I1

[6-bit opcode][5-bit register][21-bit RAM address, offset by the PC's current position]

This layout will be used to load values from RAM into registers.
If the 21 bits aren't enough, separate I2 instructions will use register values for RAM access.

#### I2

[6-bit opcode][5-bit source register][5-bit destination register][16-bit immediate]

This layout will be used to perform immediate operations and register loading. Load immediate will probably be an alias of addi.

#### Chunk

[6-bit opcode][5-bit target register][2-bit chunk selector (to form 64-bit numbers)][3-bit padding][16-bit immediate]
#### Starting instructions
* Memory related
  * `LDC` (Chunk) - forms 64-bit numbers from immediates
  * `LDR` (I1) - load PC-relative values from RAM, with a 21-bit range
  * `LD` (I2) - load RAM from an address stored in a register and put it in another register
  * `SD` (I2) - store a value from a register in an address in RAM stored in another register
* Arithmetic and System
  * `EXECUTE` (R)

As the EXECUTE opcode has capability for many functions as an R-type instruction, here are the first I will implement:

Under the 4-bit category 0001` is going to be integer math. Inside of it we will have the following basic instructions, identified by the 7-bit function field:
* `ADD`
* `SUB`
* `MUL`
* `DIV`
* `MOD`

The 4-bit category `0000` is for syscalls, and inside its 7-bit function field:
* `HALT`

### The plan and some notes

I am going to set up CMake, write all the code to have the above functioning with a Fetch-Decode-Execute cycle. Then, after everything is working, I will go back to the README.md, update the specification, and start planning the next additions to the ISA. I originally planned to have a separate syscall opcode, imath, and fmath; but by merging them we use the space that would otherwise remain empty inside each, and leave space for more opcodes.

If you have been paying attention, you may have noticed the README.md mentions a 64KB RAM. Why would Psammite need something other than `LDR`? Everything is in range of LDR right now. You are completely right, but I do not intend to leave RAM at that size, so I'm planning ahead.

See you soon!
## [4/6/26] Basic Fetch-Decode-Execute cycle

Hello! It has been a couple days, and I have been hard at work on making Psammite a functional VM.

I have implemented all the instructions I mentioned on the previous log, plus `NOP`, `SDIV`, and `SMOD` for signed operations. Also, the project structure has been overhauled to allow more modularity. CTest has been integrated as the testing framework, I plan to have unit tests checking the functionality of the VM. To that end I have taken a slightly less conventional approach to header files. Most inlined functions (inlined due to performance gains, albeit small, in the hot loop) have been written in header files. This is because, to have granular testing, access to the functions in the test file is needed.
I also have written the `VM_TO_HOST` macros so that the VM can run on both big endian and little endian hosts. And to make sure the code compiles correctly on multiple platforms, GitHub actions that compile the VM with Clang, GCC, and MSVC in Windows and Linux (little endian and big endian) have been configured.

Most of my time was spent writing the foundations of the VM so it is testable, portable, and reliable. So I have not updated the README.md yet with the instructions I implemented.

So... what's next? I will work on designing branching instructions (expect an update with the rationale and design process soon), and updating the README.md. I have had a lot of fun, and I have learnt a lot writing this project. The two most important lessons I have learnt are: making strong foundations is fundamental to create a mantainable project, and second, do not spend too much time optimizing for uncommon situations or hardware combinations; or at least, until the code runs correctly. I spent too much time endian proofing my VM, when it didn't work at all yet. If I were to start again, I would first have it run on my machine, and then handle those edge cases.

I know this has been a slightly smaller entry, I have been busy with the implementation and my personal life. But I plan to write some bigger ones in a few days.
Thanks for your time!

## [13/6/26] Assembly macros, more unit-tests, and some decisions

Hi! Today I'll be covering some of the latest additions to Psammite.

Firstly, I have added macros that make writing assembly for all currently implemented instructions much easier and convenient. I also coded a new instruction, `ADDI`, which allows for faster simple integer operations, also, it can be used under the hood for a `LOADI` pseudo-instruction using the `ZR`. Then, using those new macros, new unit-tests for the `ALU`, load and store related instructions have been made. I did this before adding branching instructions because I wanted to ensure the VM was functional as is before adding more complicated behavior. The unit-tests have been really useful to bug-discovery, as my initial implementation of `ADDI` had a problem when being used as `LOADI` for signed numbers, it didn't extend the values correctly. So when the VM executed `SDIV` and `SMOD` operations, instead of being handed small negative numbers, the operations were conducted on massive positive numbers. This has now been fixed.

One architectural choice I hadn't made, was whether any PC relative offset would start from the current instruction or from the next one. Currently the PC is increased in the fetch step, and I consider that best-practice as it avoids code duplication. If I wanted to have offsets relative to the current instruction, substracting one from the PC would be necessary. At least for me, the added convenience isn't worth an additional arithmetic operation in many instructions. Because of that, offsets will be relative to the next instruction.

On another note, the `NOP` instruction has been demoted to a pseudo-instruction that is an alias of `ADDI(ZR,ZR,0)`. Also, the `psammite_step` function has been inlined too to increase performance.


## [14/6/26] Some changes, jump and branching

Some quick changes: I have gone ahead and renamed the `I1` layout to `J` and `I2` to just `I`. This is because I realized the `I1` layout was exactly what was needed for the jump instruction layout, so renaming to `J` is much more adequate. Also, because the git commit history was getting cluttered with README and DEVLOG fixes, the project is migrating to a feature branch workflow. A new `B-type` layout has been introduced for branching instructions, it has exactly the same layout as `I-type`, however instead of having `rs` and `rd` fields, they are renamed `rs1` and `rs2` to better showcase intent.

For now unaligned memory access is allowed when reading constants, however, it isn't when performing jump/branching instructions. In that case the address is forcefully aligned because assuming the last two bits are 0 allows Psammite to increase its jump range by two more bits.

Jump and branching instructions have been implemented. The VM has the following jump/branching instructions: `JAL`, `JALR`,`BEQ`, `BNE`, `BLT`, `BGE`, `SBLT`, and `SBGE`. In the macro assembler.The macro assembler also includes pseudo-instructions for operations like Branch Greater Than to make coding easier. With this Psammite becomes Turing complete. I'm super excited about achieving such a milestone.

You may have noticed Psammite heavily resembles RISC-V and MIPS. Indeed I have taken heavy inspiration from them because of their simplicity and speed. However, I don't intend to write an emulator for either of those architectures. Even if similar, Psammite is its own project, and many design decisions have resulted in a VM that is unique and has its own ISA. The project will diverge more as more advanced features are added. The next objectives are logical operations and float support. So stay tuned for that.

## [19/6/26] More unit-tests, more pseudo-instructions, and layout changes

First, additional unit tests were added.

Second, the chunk instruction encoding was improved. The `LDC` instruction now has a `zero_flag` bit: when set, the target register is cleared before the 16-bit chunk is inserted. Two assembler macros were added to keep assembly simple.
* `LDC`: Preserves existing chunks (`zero_flag` = 0)
* `LDCZ`: Zeroes register first, then inserts chunk (`zero_flag` = 1)

`LOADI` has been replaced by `LI` and `SLI`, which expand to `LDCZ` and `ADDI`.
