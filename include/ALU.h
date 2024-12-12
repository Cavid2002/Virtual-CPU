#include <stdint.h>

typedef struct ALU ALU;

uint32_t add(uint32_t src1, uint32_t src2);
uint32_t sub(uint32_t src1, uint32_t src2);
uint32_t orr(uint32_t src1, uint32_t src2);
uint32_t andd(uint32_t src1, uint32_t src2);
uint32_t lshitf(uint32_t src1, uint32_t src2);
uint32_t rshitf(uint32_t src1, uint32_t src2);
uint32_t xorr(uint32_t src1, uint32_t src2);


struct ALU
{
	uint32_t src1;
	uint32_t src2;
	uint32_t res;
	uint8_t opcode;
	uint8_t flags;
	uint32_t (*op[8])(uint32_t, uint32_t);

};

ALU alu_init();
uint8_t set_flag_register(uint32_t src1, uint32_t src2, uint32_t res);

