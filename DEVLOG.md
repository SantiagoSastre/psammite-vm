# Psammite Devlog

## [28/5/26]  Foundation
A long time itch of mine is building a simple virtual machine. Computers have always fascinated me, not in an abstract way, I'm deeply drawn to the way they work, and in a journey to understand them better I will try my hand at making my very own VM. My main goals are:
* Coding a modern, fully capable VM using the least abstractions as possible.
* To do it on my own. it is ok to read and ask, but not to blindly follow tutorials.


I will make a 64-bit word size 32-bit fixed width instructions register-based VM, as to follow modern architectures. I plan to take some inspiration on RISC architectures. One important point I decided early is the number of registers in the VM; it will have 32 integer registers, 32 float registers (as in real machines they are separate), and one special register for the Program Counter.
I find names inspiring, having a name for the project motivates me to continue it, so I spent a little time thinking on one, and after a while I landed on "Psammite", which is a broad category for sandstone and its derivatives. I felt drawn to it as it is a simple material, yet reliable, and those are the two values I want to represent the most in this project, it must work simply and without problem. Psammite won't be a beautiful state of the art vm, but I plan to have a polished and working product, using it as a learning plataform, and a stepping stone for harder projects down the line.
I will develop the project on my spare time, and will do three different stages:
* Working VM
* Assembler
* A simple programming language native to Psammite


If you happen to stumble upon my project, Hi!!!
I hope you come back to see the development of Psammite!
