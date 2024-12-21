#include "../include/ALU.h"

ALU alu_init()
{
	ALU alu;
	alu.src1 = 0; alu.src2 = 0;
	alu.opcode = 0;
	alu.flags = 0;
	alu.op[0] = add;
	alu.op[1] = sub;
	alu.op[2] = andd;
	alu.op[3] = orr;
	alu.op[4] = xorr;
	alu.op[5] = rshitf;
	alu.op[6] = lshitf;

	return alu;
}


uint8_t set_flag_register(uint32_t src1, uint32_t src2, uint32_t res) 
{
    uint8_t new_flag = 0;
    uint8_t msb_src1 = src1 >> 31; // MSB of src1
    uint8_t msb_src2 = src2 >> 31; // MSB of src2
    uint8_t msb_res = res >> 31;   // MSB of res

    new_flag |= msb_res; // N

    new_flag |= ((res == 0) << 1); // Z

	new_flag |= (((uint64_t)src1 + (uint64_t)src2) >> 32) << 2; // C

    new_flag |= ((msb_res ^ msb_src1) & (msb_res ^ msb_src2)) << 3; // V

    return new_flag;
}


uint32_t add(uint32_t src1, uint32_t src2)
{
	return src1 + src2;
}

uint32_t sub(uint32_t src1, uint32_t src2)
{
	return src1 + (-src2);
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

