#include "../include/Decoder.h"
#include "../include/ASM.h"

#include <string.h>
#include <stdlib.h>


char* regs[16] = {"r0", "r1", "r2", "r3",
                  "r4", "r5", "r6", "r7",
                   "r8", "r9", "r10", "r11",
                   "r12", "13", "r14", "r15"};


char* opcode[4] = {"", "ldr", "str", "brc"};

char* aluop[8] = {"add", "sub", "orr", 
                "and", "xor", "lsh", "rsh"};

                
char* cond_prefix[16] = {"eq", "ne", "hs", "lo", 
                        "mi", "pl", "vs", "vc", 
                        "hi", "ls", "ge", "lt",
                        "gt", "le", "al", NULL};


uint32_t uncoditional_brch(uint32_t offset)
{
    uint32_t instr = 0;
    instr |= 14 << SHIFT_COND;
    instr |= 3 << SHIFT_OPCODE;
    instr |= offset;
    return instr;
}


int compare_string(char* str1, char* str2)
{
    int size1 = strlen(str1);
    int size2 = strlen(str2);
    if(size1 != size2) return 0;
    
    if(strncmp(str1, str2, size1) == 0)
    {
        return 1;
    }

    return 0;
}


void parse_flag(char** tokens, int* instr)
{
    int temp = 0;
    int offset = 0;
    int cond = 0;
    int isize = strlen(tokens[0]);

    switch (isize)
    {
    case 3:
        cond = 14;
        temp |= cond << SHIFT_COND;
        *instr |= temp;
        return;
        break;
    case 4:
        cond = 14;
        temp |= cond << SHIFT_COND;

        if(tokens[0][3] == 's') temp |= 1 << SHIFT_FLAGS;
        else fatal_error(tokens[0]);
        *instr |= temp;
        
        return;
        break;
    case 5:
        offset = 3;
        break;
    case 6:
        offset = 4;
        if(tokens[0][3] == 's') temp |= 1 << SHIFT_FLAGS;
        else fatal_error("syntax error");
        
        break;
    }
    

    for(int i = 0; i < 14; i++)
    {
        if(strncmp(tokens[0] + offset, cond_prefix[i], 2) == 0)
        {
            cond = i;
            temp |= cond << SHIFT_COND;
            *instr |= temp;
            return;
        }
    }
    

    fatal_error("syntrax error");

}


int get_regs(char* reg)
{
    for(int i = 0; i < 16; i++)
    {
        if(compare_string(reg, regs[i]) == 1)
        {   
            return i;
        }
    }
    fatal_error(reg);
    return -1;
}

int get_immd(char* immd)
{
    uint32_t res = atoi(immd + 1) & 0x00000FFF;
    return res;
}

void parse_load(char** tokens, uint32_t* instr)
{
    uint32_t temp;
    uint32_t dst, src1, src2, immd;    
    temp = 0;
    temp |= 2 << SHIFT_OPCODE;
    temp |= 0 << SHIFT_FUNCODE;
    
    dst = get_regs(tokens[1]);
    src1 = get_regs(tokens[2]);
    
    temp |= dst << SHIFT_REGDEST;
    temp |= src1 << SHIFT_REGSRC1;

    
    if(strchr(tokens[3], 'r') != NULL)
    {
        src2 = get_regs(tokens[3]);
        temp |= src2 << SHIFT_REGSRC2;
        *instr |= temp;
        return;
    }
    
    if(strchr(tokens[3], '#') != NULL)
    {
        temp |= 1 << SHIFT_IMMD;
        temp |= get_immd(tokens[3]); 
        *instr |= temp;  
        return;
    }

    fatal_error(tokens[3]);

    return;
}



void parse_store(char** tokens, uint32_t* instr)
{
    uint32_t temp;
    uint32_t dst, src1, src2, immd;   

    temp = 0;
    temp |= 1 << SHIFT_OPCODE;
    temp |= 0 << SHIFT_FUNCODE;    

    dst = get_regs(tokens[1]);
    src1 = get_regs(tokens[2]);
    
    temp |= dst << SHIFT_REGDEST;
    temp |= src1 << SHIFT_REGSRC1;

    
    if(strchr(tokens[3], 'r') != NULL)
    {
        src2 = get_regs(tokens[3]);
        temp |= src2 << SHIFT_REGSRC2;
        *instr |= temp;
        return;
    }
    
    if(strchr(tokens[3], '#') != NULL)
    {
        temp |= 1 << SHIFT_IMMD;
        temp |= get_immd(tokens[3]);
        *instr |= temp;  
        return;
    }

    fatal_error(tokens[3]);

    return;
}



void parse_branch(char** tokens, uint32_t* instr)
{
    uint32_t temp;
    
    temp = 0;
    temp |= 3 << SHIFT_OPCODE;
    
    uint32_t offset = atoi(tokens[1] + 1) & 0x03FFFFFF;
    temp |= offset;
    *instr |= temp;
    return;
}


void parse_dataproc(char** tokens, uint32_t* instr)
{
    uint32_t temp = 0;
    uint32_t dst, src1, src2, immd;   

    temp |= 0 << SHIFT_OPCODE;
    
    bool synerr_flag = 1;
    for(int i = 0; i < 8; i++)
    {
        if(strncmp(tokens[0], aluop[i], 3) == 0)
        {
            temp |= i << SHIFT_FUNCODE;
            break;
        }
    }

    dst = get_regs(tokens[1]);
    src1 = get_regs(tokens[2]);
    
    temp |= dst << SHIFT_REGDEST;
    temp |= src1 << SHIFT_REGSRC1;

    
    if(tokens[3][0] == 'r')
    {
        src2 = get_regs(tokens[3]);
        temp |= src2 << SHIFT_REGSRC2;
        *instr |= temp;
        return;
    }
    else if(tokens[3][0] == '#')
    {
        temp |= 1 << SHIFT_IMMD;
        temp |= get_immd(tokens[3]);
        *instr |= temp;  
        return;
    }
    else if(tokens[3][0] == '=')
    {
        uint32_t id = find_label(tokens[3] + 1);
        if(id == -1) fatal_error("label doesn't exists");
        temp |= 1 << SHIFT_IMMD;
        temp |= ltable[id].addr;
        *instr |= temp;
        return;
    }

    fatal_error(tokens[3]);
    
    return;
    
}

void parse_mov(uint32_t* step1, uint32_t* step2, char** tokens)
{
    uint32_t src = 0;
    uint32_t dst = get_regs(tokens[1]);
    *step1 |= 0 << SHIFT_OPCODE;
    *step1 |= dst << SHIFT_REGDEST;
    *step1 |= dst << SHIFT_REGSRC1;
    *step1 |= dst << SHIFT_REGSRC2;
    *step1 |= 4 << SHIFT_FUNCODE;


    *step2 |= 0 << SHIFT_OPCODE;
    *step2 |= dst << SHIFT_REGDEST;
    *step2 |= dst << SHIFT_REGSRC1;
    *step2 |= 0 << SHIFT_FUNCODE;

    if(strchr(tokens[2], '#') != NULL)
    {
        src = get_immd(tokens[2]);
        *step2 |= 1 << SHIFT_IMMD;
        *step2 |= src;
        return;
    }

    if(strchr(tokens[2], 'r') != NULL)
    {
        src = get_regs(tokens[2]);
        *step2 |= src << SHIFT_REGSRC2;
        return;
    }

    fatal_error(tokens[3]);
    return;
}


void parse_instruction(char* line, FILE* dest_file)
{
    char** tokens = split_str(line, " ");
    uint32_t instr = 0;


    //debug instruction
    if(strncmp(tokens[0], "out", strlen("out")) == 0)
    {
        instr = 0xFFFFFFFF;
        fwrite(&instr, sizeof(instr), 1, dest_file);
        return;
    }

    
    parse_flag(tokens, &instr);
    if(strncmp(tokens[0], "mov", 3) == 0)
    {
        uint32_t step1 = 0, step2 = 0;
        step1 |= instr;
        step2 |= instr;

        parse_mov(&step1, &step2, tokens);

        fwrite(&step1, sizeof(uint32_t), 1, dest_file);
        fwrite(&step2, sizeof(uint32_t), 1, dest_file);
        return;
    }

    if(strncmp(tokens[0], "ldr", 3) == 0)
    {
        parse_load(tokens, &instr);
    }
    else if(strncmp(tokens[0], "str", 3) == 0)
    {
        parse_store(tokens, &instr);
    }
    else if(strncmp(tokens[0], "brc", 3) == 0)
    {
        parse_branch(tokens, &instr);
    }
    else
    {
        parse_dataproc(tokens, &instr);
    }


    fwrite(&instr, sizeof(uint32_t), 1, dest_file);

    free_tokens(tokens);
    

}
