# Psammite Virtual Machine
## A simple 64-bit VM written in C.
Psammite is currently a work in progress. Check the **[Developer Log](DEVLOG.md)**

## How to Build and Run
Psammite uses CMake for its build system. 

1. Generate the build files: `cmake -S . -B build`
2. Compile the VM: `cmake --build build`
3. Run the VM:
  * Linux/MacOS: `./run.sh`
  * Windows: `.\build\bin\Debug\psammite_vm.exe` if using MSVC, otherwise `.\build\bin\psammite_vm.exe`

### Specification
* Architecture: 64-bit word size, little endian.
* Instruction width: fixed 32-bit instructions.
* Memory: 64 KB, byte-addressable.
* Stack: full-descending (grows downwards, points to the last element).
* ABI rule: Each function (Callee) must  restore the Link Register and Base Pointer of their caller.
* Program counter: single 64-bit register (PC).
* Integer registers (32x64-bit):
  * R0/ZR is hardwired to 0.
  * R1/SP: Stack Pointer.
  * R2/BP: Base Pointer (anchors the stack frame for local variables).
  * R3/LR: Link Register (holds the return address).
  * R4-R31: general purpose registers.
* Floating-point registers: 32x64-bit registers (FR0-FR31).


#### Instruction Set Architecture
