
#include <stdint.h>
#include "ALU.h"

#define MEM_SIZE 1024 * 1024

#define MASK_COND 		0xF0000000
#define MASK_OPCODE 	0x0C000000
#define MASK_FUNCODE 	0x03F00000
#define MASK_REGDEST	0x000F0000
#define MASK_REGSRC1	0x0000F000
#define MASK_REGSRC2	0x00000F00
#define MASK_IMMD		0x00000FFF



#define SHIFT_COND 		28
#define SHIFT_OPCODE	26
#define SHIFT_FUNCODE 	20
#define SHIFT_REGDEST 	16
#define SHIFT_REGSRC1	12
#define SHIFT_REGSRC2	8
	



typedef struct RegFile RegFile;
typedef struct CPU CPU;
typedef struct Decoder Decoder;

uint8_t mem[MEM_SIZE];

struct Decoder
{
	uint8_t cond;
	uint8_t opcode;
	uint8_t func;
};

struct RegFile
{
	uint8_t dest_index;
	uint8_t src1_index;
	uint8_t src2_index;
	uint32_t immediate;
	uint32_t r[16];
};

struct CPU
{
	uint32_t pc;
	uint32_t ir;
	Decoder decoder;
	RegFile rfile;
	ALU alu;
};


CPU cpu;

void cpu_init()
{

}

void cpu_loop()
{
	while(1)
	{
		fetch();
		decode();
		execute(); 
		memory();
		writeback();
	}
}

void fetch()
{
	cpu.pc = cpu.rfile.r[15];
	cpu.rfile.r[15] += 4;
	cpu.ir = mem[cpu.pc]; 
}

void decode()
{
	cpu.decoder.cond = (cpu.ir & MASK_COND) >> SHIFT_COND;
	cpu.decoder.opcode = (cpu.ir & MASK_OPCODE) >> SHIFT_OPCODE;
	cpu.decoder.func = (cpu.ir & MASK_FUNCODE) >> SHIFT_FUNCODE;

	cpu.rfile.dest_index = (cpu.ir & MASK_REGDEST) >> SHIFT_REGDEST;
	cpu.rfile.src1_index = (cpu.ir & MASK_REGSRC1) >> SHIFT_REGSRC1;
	cpu.rfile.src2_index = (cpu.ir & MASK_REGSRC2) >> SHIFT_REGSRC2;
	cpu.rfile.immediate = (cpu.ir & MASK_IMMD);

}


void execute()
{
	cpu.alu.src1 = cpu.rfile.r[cpu.rfile.src1_index];
	cpu.alu.src2 = cpu.rfile.r[cpu.rfile.src2_index];
	if(cpu.decoder.func >= 8) cpu.alu.src2 = cpu.rfile.immediate;

	cpu.alu.opcode = cpu.decoder.func & 0x8;  

	if(cpu.decoder.opcode == 0)
	{
		cpu.alu.res = cpu.alu.op[cpu.alu.opcode](cpu.alu.src1, cpu.alu.src2);
		return;
	}
	if(cpu.decoder.opcode == 1)
	{
		
		return;
	}

	if(cpu.decoder.opcode == 3)
	{

		return;
	}
	
}


void memory()
{

}

void writeback()
{

}


