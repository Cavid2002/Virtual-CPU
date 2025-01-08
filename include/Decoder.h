#ifndef DECODER_H
#define DECODER_H
#include <stdint.h>


#define MASK_COND 		0xF0000000
#define MASK_OPCODE 	0x0C000000
#define MASK_FUNCODE 	0x03F00000
#define MASK_REGDEST	0x000F0000
#define MASK_REGSRC1	0x0000F000
#define MASK_REGSRC2	0x00000F00
#define MASK_IMMD		0x00000FFF
#define MASK_BRCH_IMMD  0x03FFFFFF


#define SHIFT_COND 		28
#define SHIFT_OPCODE	26
#define SHIFT_IMMD		25
#define SHIFT_FLAGS		24
#define SHIFT_FUNCODE 	20
#define SHIFT_REGDEST 	16
#define SHIFT_REGSRC1	12
#define SHIFT_REGSRC2	8


#define FUNC_IMMD_MASK 32
#define FUNC_FLAG_MASK 16
#define FUNC_ALUC_MASK 7



#define OP_DATA 0
#define OP_STR	1
#define OP_LDR 	2
#define OP_BRC	3


typedef struct Decoder Decoder;
typedef struct RegFile RegFile;




struct Decoder
{
	uint8_t cond;
	uint8_t opcode;
	uint8_t func;
	uint8_t (*op[16])(uint8_t);
	uint8_t flag_res;
};


struct RegFile
{
	uint8_t dest_index;
	uint8_t src1_index;
	uint8_t src2_index;
	uint32_t immediate;
	uint32_t r[16];
};


Decoder decoder_init();
RegFile registers_init();

uint8_t equal(uint8_t flag);
uint8_t not_equal(uint8_t flag);
uint8_t carry_set(uint8_t flag);
uint8_t carry_clear(uint8_t flag);
uint8_t minus(uint8_t flag);
uint8_t plus(uint8_t flag);
uint8_t overflow_set(uint8_t flag);
uint8_t overflow_clear(uint8_t flag);
uint8_t higher(uint8_t flag);
uint8_t lower_same(uint8_t flag);
uint8_t greater_equal(uint8_t flag);
uint8_t less(uint8_t flag);
uint8_t greater(uint8_t flag);
uint8_t less_equal(uint8_t flag);
uint8_t always(uint8_t flag);

uint32_t convert_26_to_32(uint32_t instr);
uint32_t convert_12_to_32(uint32_t instr);

#endif