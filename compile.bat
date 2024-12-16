gcc -c .\src\CPU.c -o .\bin\CPU.o
gcc -c .\src\ALU.c -o .\bin\ALU.o
gcc -c .\src\Decoder.c -o .\bin\Decoder.o
gcc main.c .\bin\CPU.o .\bin\Decoder.o .\bin\ALU.o -o .\CPU
gcc ASM.c -o ASM