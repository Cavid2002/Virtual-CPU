#include <stdint.h>
#include <stdbool.h>

#define CODE_SEGMENT 1
#define CONST_SEGMENT 2
#define DATA_SEGMENT 3

#define MAX_LINE 100
#define MAX_LABEL 1000
typedef struct Label Label;

struct Label{
    char* name;
    uint32_t addr;
};

extern Label ltables[MAX_LABEL];
extern uint32_t lcount;
extern uint32_t line_count;
extern uint32_t instr_count;



char** split_str(char* str, char c);
void free_tokens(char** tokens);

void fatal_error(const char* msg);
void check_synerr(const char* msg, bool* flag);
void parse_instruction(char* line, uint32_t* instr);
void parse_immd(char* line, uint32_t* instr);
uint32_t uncoditional_brch(uint32_t offset);