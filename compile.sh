#!/bin/bash

gcc -c ./src/CPU.c -o ./bin/CPU.o
gcc -c ./src/ALU.c -o ./bin/ALU.o
gcc -c ./src/Decoder.c -o ./bin/Decoder.o
gcc mpu.c ./bin/CPU.o ./bin/Decoder.o ./bin/ALU.o -o ./mpu

gcc -c ./src/ASM.c -o ./bin/ASM.o
gcc -c ./src/InstrParser.c -o ./bin/InstrParser.o
gcc -c ./src/LabelParser.c -o ./bin/LabelParser.o
gcc mpu_asm.c ./bin/InstrParser.o ./bin/LabelParser.o ./bin/ASM.o -o mpu_asm