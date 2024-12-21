# VIRTUAL CPU #

## Introduction ##

This term project for the Computer Architecture course involves the design and implementation of a 32-bit Virtual CPU based on a custom architecture inspired by ARM32 and 6502. The project includes the development of an accompanying assembler to generate programs for the processor. However, the assembler is currently incomplete, with label resolution for branch instructions yet to be implemented.

## Build and Execute ##

Clone the repository to local folder, create `bin` folder if it does not exists:

```Bash
git clone https://github.com/Cavid2002/Virtual-CPU.git
cd Virtual-CPU
mkdir bin
./compile.sh  // In unix-like OS
./compile.bat // Windows or MSDOS
```

Once all the necessary executables are built, create a simple text file containing the assembly code. Then, compile the assembly text file into CPU-readable bytecode.

```
./mpu_asm input.txt output.bin
./mpu output.bin 
```

It is also provided some sample codes inside the folder of the same name.


## Instruction Layout ##


| Condition | Opcode|  Imd | Flag|  ALU | Rdest |  Rsrc1 | Rsrc2 | Unused|
| ----------|-------|------|-----|------|-------|--------|-------|-------| 
|   31-28    | 27-26 | 25 |  24  | 23-20 | 19-16 | 15-11  | 11-8 | 7-0   |


| Condition | Opcode|  Imd | Flag|  ALU | Rdest |  Rsrc1 | Immd-val      |
| ----------|-------|------|-----|------|-------|--------|---------------| 
|   31-28    | 27-26 | 25 |  24  | 23-20 | 19-16 | 15-11  | 11-0   |


| Condition | Opcode|  Immd-val    |
| ----------|-------|--------------| 
|   31-28    | 27-26 |   25-0      |        


All the immediate values referenced above are represented in 2's complement format.


## Supported Instructions: ##

It is important to note all the instruction except for branch requires 3 operands:

```
[instr] [reg_dest] [src1_reg] [immd] // 
[instr] [reg_dest] [src1_reg] [src2_reg]
```

Instructions and operands are separated by a space. Avoid using commas or other symbols as separators, as this may result in undefined behavior.

```ASM
add r0 r0 #23 // Correct
add r0, r0, #23 // Wrong
```

list of all aviable instruction are listed below:

| Mnemonic        | Definition |
| ------------- |:-------------:
|  add          | addtion      |
|  sub          | sub          |
|  lsh          | left-shift   |
|  rsh          | right-shift  |
|  orr          | boolean-or   |
|  and          | boolean-and  |
|  xor          | exclusive-or |
|  ldr          | load         |
|  str          | store        |
|  brc          | brach        |

Conditional mnemonics are listed below:



| Mnemonic        | Definition |
| ------------- |:-------------:
| eq | equal|
| ne | not-equal|
| hs | higher-same|
| lo | lower|
| mi | minus|
| pl | plus|
| vs | overflow-set|
| vc | overflow-clear|
| hi | higher|
| ls | lower-same|
| ge | greater-equal|
| lt | less-than|
| gt | greater-than|
| le | less-equal|
| al | always|

Sample code:

```
ldr r4 r0 #0
str r5 r1 #-4
subs r8 r4 r5
addlt r10 r10 #252
```

There is also pseudo-instruction like `out` which will print the state of registers to the standart output:

`Sample-Output`
```
r0 = 1
r1 = 2
r2 = 0
r3 = 0
r4 = 10
r5 = 32
r6 = 0
r7 = 0
r8 = -22
r9 = 0
r10 = 252
r11 = 0
r12 = 0
r13 = 0
r14 = 0
r15 = 12
psw = 1
```