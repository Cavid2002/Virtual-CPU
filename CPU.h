
#include <stdint.h>
#include "ALU.h"

#define MEM_SIZE 1024 * 1024

#define MASK_COND 		0xF0000000
#define MASK_OPCODE 	0x0C000000
#define MASK_FUNCODE 	0x03C00000
#define MASK_REGDEST	0x003C0000
#define MASK_REGSRC1	0x0003C000
#define MASK_REGSRC2	0x00003C00
#define MASK_IMMD		0x00003FFF



typedef struct RegFile RegFile;
typedef struct CPU CPU
typedef struct Decoder Decoder;

uint8_t mem[CPU];

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
		read();
		execute();
		writeback();
	}
}

void fetch()
{
	cpu.pc = cpu.regFile[15];
	cpu.regFile[15] += 4;
	cpu.ir = mem[pc]; 
}

void decode()
{
	cpu.decoder.cond = (cpu.ir & MASK_COND) >> 28;
	cpu.decoder.opcode = (cpu.ir & MASK_OPCODE) >> 26;
	cpu.decoder.func = (cpu.ir & MASK_FUNCODE) >> 22;

}


void read(uint32_t instruction)
{
	cpu.rfile.dst_index = (cpu.ir & MASK_REGDEST) >> 18;
	cpu.rfile.src1_index = (cpu.ir & MASK_REGSRC1) >> 14;
	cpu.rfile.src2_index = (cpu.ir & MASK_REGSRC2) >> 10;
	cpu.rfile.immediate = (cpu.ir & MASK_IMMD);
	cpu.rfile.r[cpu.rfile.src1_index];

	
}

void execute()
{
	if(cpu.decoder.opcode == 0)
	{

		return;
	}

	
}

void writeback()
{

}


