gcc -c .\src\CPU.c -o .\bin\CPU.o
gcc -c .\src\ALU.c -o .\bin\ALU.o
gcc -c .\src\Decoder.c -o .\bin\Decoder.o
gcc main.c .\bin\CPU.o .\bin\Decoder.o .\bin\ALU.o -o .\CPU

gcc -c .\src\Common.c -o .\bin\Common.o
gcc -c .\src\InstrParser.c -o .\bin\InstrParser.o
gcc ASM.c .\bin\InstrParser.o .\bin\Common.o -o ASM