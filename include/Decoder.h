#include <stdint.h>


#define MASK_COND 		0xF0000000
#define MASK_OPCODE 	0x0C000000
#define MASK_FUNCODE 	0x03F00000
#define MASK_REGDEST	0x000F0000
#define MASK_REGSRC1	0x0000F000
#define MASK_REGSRC2	0x00000F00
#define MASK_IMMD		0x00000FFF
#define MASK_BRCH_IMMD  0x0FFFFFFF


#define SHIFT_COND 		28
#define SHIFT_OPCODE	26
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