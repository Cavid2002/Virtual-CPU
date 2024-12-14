#include "ALU.h"
#include "Decoder.h"

#define MEM_SIZE 1024 * 1024

extern uint32_t mem[MEM_SIZE];


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


extern CPU cpu;

void cpu_init();
void cpu_loop();
void display_regs();

