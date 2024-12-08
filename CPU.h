#include <stdint.h>
#include "ALU.h"
#include "Decoder.h"

#define MEM_SIZE 16 * 1024

uint32_t mem[MEM_SIZE];


typedef struct CPU CPU;


struct CPU
{
	uint32_t pc;
	uint32_t ir;
	uint32_t mar;
	uint32_t mbr;
	Decoder decoder;
	RegFile rfile;
	ALU alu;
};


CPU cpu;


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
	cpu.decoder.flag_res = cpu.decoder.cond & cpu.alu.flags;

	cpu.rfile.dest_index = (cpu.ir & MASK_REGDEST) >> SHIFT_REGDEST;
	cpu.rfile.src1_index = (cpu.ir & MASK_REGSRC1) >> SHIFT_REGSRC1;
	cpu.rfile.src2_index = (cpu.ir & MASK_REGSRC2) >> SHIFT_REGSRC2;
	cpu.rfile.immediate = (cpu.ir & MASK_IMMD);
}


void execute()
{
	if(cpu.decoder.flag_res == 0) return;

	cpu.alu.src1 = cpu.rfile.r[cpu.rfile.src1_index];
	
	if(cpu.decoder.func & 32)
	{
		cpu.alu.src2 = cpu.rfile.immediate;
	}
	else
	{
		cpu.alu.src2 = cpu.rfile.r[cpu.rfile.src2_index];
	}


	cpu.alu.opcode = cpu.decoder.func & FUNC_ALUC_MASK;
	cpu.alu.res = cpu.alu.op[cpu.alu.opcode](cpu.alu.src1, cpu.alu.src2);

	if(cpu.decoder.func & (uint8_t)FUNC_FLAG_MASK)
	{
		cpu.alu.flags = 0;
		cpu.alu.flags |= (uint8_t)((cpu.alu.res & 0x80000000) >> 31); // N
		cpu.alu.flags |= (uint8_t)(~(cpu.alu.res & cpu.alu.res) >> 30); // Z
		cpu.alu.flags |= (uint8_t)((((cpu.alu.src1 >> 31) | 
									(cpu.alu.src2 >> 31)) & ~(cpu.alu.res >> 31)) << 2); // C
		cpu.alu.flags |= (uint8_t)(~((cpu.alu.src1 >> 31) ^ 
						(cpu.alu.src2 >> 31)) & (cpu.alu.res >> 31) << 3); // V
	}
	
}


void memory()
{
	if(cpu.decoder.flag_res == 0) return;

	if(cpu.decoder.opcode == OP_LDR)
	{
		cpu.mar = cpu.alu.res;
		cpu.mbr = mem[cpu.mar];
		return;
	}

	if(cpu.decoder.opcode == OP_STR)
	{
		cpu.mar = cpu.alu.res;
		cpu.mbr = cpu.rfile.r[cpu.rfile.dest_index];
		mem[cpu.mar] = cpu.mbr;
	}
}

void writeback()
{
	if(cpu.decoder.flag_res == 0) return;
	
	if(cpu.decoder.opcode == OP_DATA)
	{
		cpu.rfile.r[cpu.rfile.dest_index] = cpu.alu.res;
		return;
	}

	if(cpu.decoder.opcode == OP_LDR)
	{
		cpu.rfile.r[cpu.rfile.dest_index] = cpu.mbr;
		return;
	}
	
}


void cpu_init()
{
	cpu.alu = alu_init();
	cpu.decoder = decoder_init();
	cpu.rfile = registers_init();
	cpu.pc = 0;
	cpu.mar = 0;
	cpu.ir = 0;
	cpu.mbr = 0; 
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



