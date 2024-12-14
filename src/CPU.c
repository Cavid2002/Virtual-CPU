#include <stdio.h>
#include "../include/CPU.h"

uint32_t mem[MEM_SIZE];

CPU cpu;

void fetch()
{
	cpu.pc = cpu.rfile.r[15];
	cpu.rfile.r[15] += 4;
	cpu.ir = mem[cpu.pc]; 
}

void decode()
{
	if(cpu.ir == 0xFFFFFFFF)
	{
		display_regs();
		cpu.decoder.flag_res = 0;
	}


	cpu.decoder.cond = (cpu.ir & MASK_COND) >> SHIFT_COND;
	cpu.decoder.opcode = (cpu.ir & MASK_OPCODE) >> SHIFT_OPCODE;
	cpu.decoder.func = (cpu.ir & MASK_FUNCODE) >> SHIFT_FUNCODE;

	cpu.decoder.flag_res = cpu.decoder.op[cpu.decoder.cond](cpu.alu.flags);

	if(cpu.decoder.opcode == OP_BRC)
	{
		cpu.rfile.src1_index = 15;
		cpu.rfile.immediate = cpu.ir & MASK_BRCH_IMMD;
		cpu.rfile.dest_index = 15;
	}		
	else
	{
		cpu.rfile.dest_index = (cpu.ir & MASK_REGDEST) >> SHIFT_REGDEST;
		cpu.rfile.src1_index = (cpu.ir & MASK_REGSRC1) >> SHIFT_REGSRC1;
		cpu.rfile.src2_index = (cpu.ir & MASK_REGSRC2) >> SHIFT_REGSRC2;
		cpu.rfile.immediate = (cpu.ir & MASK_IMMD);
	}
	
}


void execute()
{
	if(cpu.decoder.flag_res == 0) return;


	if(cpu.decoder.opcode == OP_BRC)
	{
		cpu.alu.src1 = cpu.rfile.r[cpu.rfile.src1_index];
		cpu.alu.src2 = cpu.rfile.immediate;
		cpu.alu.res = cpu.alu.op[0](cpu.alu.src1, cpu.alu.src2);
		return;
	}

	/// If it is not brach instruction
	cpu.alu.src1 = cpu.rfile.r[cpu.rfile.src1_index];

	if(cpu.decoder.func & (uint8_t)FUNC_IMMD_MASK)
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
		cpu.alu.flags = set_flag_register(cpu.alu.src1, cpu.alu.src2, cpu.alu.res);
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
		return;
	}
}

void writeback()
{
	if(cpu.decoder.flag_res == 0) return;
	
	if(cpu.decoder.opcode == OP_DATA || cpu.decoder.opcode == OP_BRC)
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
	cpu.pc = 4096;
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


void display_regs()
{
	for(int i = 0; i < 16; i++)
	{
		fprintf(stdout, "r%d = %d\n", i, cpu.rfile.r[i]);
	}
	fprintf(stdout, "psw = %d\n", cpu.alu.flags);
}