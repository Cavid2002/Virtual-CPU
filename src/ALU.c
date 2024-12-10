#include "../include/ALU.h"

ALU alu_init()
{
	ALU alu;
	alu.src1 = 0; alu.src2 = 0;
	alu.opcode = 0;
	alu.op[0] = add;
	alu.op[1] = sub;
	alu.op[2] = andd;
	alu.op[3] = orr;
	alu.op[4] = xorr;
	alu.op[5] = rshitf;
	alu.op[6] = lshitf;

	return alu;
}


uint32_t add(uint32_t src1, uint32_t src2)
{
	return src1 + src2;
}

uint32_t sub(uint32_t src1, uint32_t src2)
{
	return src1 - src2;
}


uint32_t orr(uint32_t src1, uint32_t src2)
{
	return src1 | src2;
}


uint32_t andd(uint32_t src1, uint32_t src2)
{
	return src1 & src2;
}

uint32_t xorr(uint32_t src1, uint32_t src2)
{
	return src1 ^ src2;
}

uint32_t rshitf(uint32_t src1, uint32_t src2)
{
	return src1 >> src2;
}

uint32_t lshitf(uint32_t src1, uint32_t src2)
{
	return src1 << src2;
}

