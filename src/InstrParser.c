#include "../include/Decoder.h"
#include "../include/Common.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char* regs[16] = {"r0", "r1", "r2", "r3",
                  "r4", "r5", "r6", "r7",
                   "r8", "r9", "r10", "r11",
                   "r12", "13", "r14", "r15"};


char* opcode[4] = {"", "ldr", "str", "brc"};

char* aluop[8] = {"add", "sub", "orr", 
                "and", "xor", "lsh", "rsh"};

                
char* cond_prefix[16] = {"eq", "ne", "cs", "cc", 
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


void parse_load(char** tokens, uint32_t* instr)
{
    uint32_t temp;
    
    temp = 0;
    temp |= 2 << SHIFT_OPCODE;
    temp |= 0 << SHIFT_FUNCODE;
    
    bool synerr_flag = 1;

    for(int i = 0; i < 16; i++)
    {
        if(compare_string(tokens[1], regs[i]) == 1)
        {   
            temp |= i << SHIFT_REGDEST;
            synerr_flag = 0;
            break;
        }
    }
    
    check_synerr(tokens[1], &synerr_flag);

    for(int i = 0; i < 16; i++)
    {
        if(compare_string(tokens[2], regs[i]) == 1)
        {   
            temp |= i << SHIFT_REGSRC1;
            synerr_flag = 0;
            break;
        }
    }

    check_synerr(tokens[2], &synerr_flag);

    char* ptr = strchr(tokens[3], 'r');
    if(ptr != NULL)
    {
        for(int i = 0; i < 16; i++)
        {
            if(compare_string(tokens[3], regs[i]) == 1)
            {   
                temp |= i << SHIFT_REGSRC2;
                synerr_flag = 1;
                break;
            }
        }
        check_synerr(tokens[3], &synerr_flag);
        *instr |= temp;
        return;
    }


    ptr = strchr(tokens[3], '#');
    if(ptr != NULL)
    {
        temp |= 1 << SHIFT_IMMD;
        temp |= atoi(ptr + 1);
        *instr |= temp;
    }
    
    
    return;
}



void parse_store(char** tokens, uint32_t* instr)
{
    uint32_t temp;
    
    temp = 0;
    temp |= 1 << SHIFT_OPCODE;
    temp |= 0 << SHIFT_FUNCODE;    

    bool synerr_flag = 1;

    for(int i = 0; i < 16; i++)
    {
        if(compare_string(tokens[1], regs[i]) == 1)
        {   
            temp |= i << SHIFT_REGDEST;
            synerr_flag = 0;
            break;
        }
    }
    
    check_synerr(tokens[1], &synerr_flag);

    for(int i = 0; i < 16; i++)
    {
        if(compare_string(tokens[2], regs[i]) == 1)
        {   
            temp |= i << SHIFT_REGSRC1;
            synerr_flag = 0;
            break;
        }
    }

    check_synerr(tokens[2], &synerr_flag);

    char* ptr = strchr(tokens[3], 'r');
    if(ptr != NULL)
    {
        for(int i = 0; i < 16; i++)
        {
            if(compare_string(tokens[3], regs[i]) == 1)
            {   
                temp |= i << SHIFT_REGSRC2;
                synerr_flag = 1;
                break;
            }
        }
        check_synerr(tokens[3], &synerr_flag);
        *instr |= temp;
        return;
    }


    ptr = strchr(tokens[3], '#');
    if(ptr != NULL)
    {
        temp |= 1 << SHIFT_IMMD;
        temp |= atoi(ptr + 1);
        *instr |= temp;
    }
    
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
    char* ptr;
    temp |= 0 << SHIFT_OPCODE;
    
    bool synerr_flag = 1;
    for(int i = 0; i < 8; i++)
    {
        if(strncmp(tokens[0], aluop[i], 3) == 0)
        {
            temp |= i << SHIFT_FUNCODE;
            synerr_flag = 0;
            break;
        }
    }

    check_synerr(tokens[0], &synerr_flag);

    for(int i = 0; i < 16; i++)
    {
        if(compare_string(tokens[1], regs[i]) == 1)
        {
            temp |= i << SHIFT_REGDEST;
            synerr_flag = 0;
            break;
        }
    }

    check_synerr(tokens[1], &synerr_flag);

    for(int i = 0; i < 16; i++)
    {
        if(compare_string(tokens[2], regs[i]) == 1)
        {
            temp |= i << SHIFT_REGSRC1;
            synerr_flag = 0;
            break;
        }
    }
    
    check_synerr(tokens[2], &synerr_flag);

    ptr = strchr(tokens[3], 'r');
    if(ptr != NULL)
    {
        for(int i = 0; i < 16; i++)
        {
            if(compare_string(tokens[3], regs[i]) == 1)
            {
                temp |= i << SHIFT_REGSRC2;
                synerr_flag = 0;
                break;
            }
        }

        check_synerr(tokens[3], &synerr_flag);
        *instr |= temp;
        return;
    }

    ptr = strchr(tokens[3], '#');
    uint32_t immd;
    if(ptr != NULL)
    {
        temp |= 1 << SHIFT_IMMD;
        immd = atoi(ptr + 1);
        if(immd > 4095) fatal_error("Const value cannot exceed 12 bits immd");
        temp |= immd;
    }

    *instr |= temp;

}

void parse_instruction(char* line, uint32_t* instr)
{
    printf("%s\n", line);
    char** tokens = split_str(line, ' ');
    *instr = 0;


    //pseudo instruction
    if(strncmp(tokens[0], "out", strlen("out")) == 0)
    {
        *instr = 0xFFFFFFFF;
        return;
    }

    
    parse_flag(tokens, instr);
    
    if(strncmp(tokens[0], "ldr", 3) == 0)
    {
        parse_load(tokens, instr);
        return;
    }
    if(strncmp(tokens[0], "str", 3) == 0)
    {
        parse_store(tokens, instr);
        return;
    }
    if(strncmp(tokens[0], "brc", 3) == 0)
    {
        parse_branch(tokens, instr);
        return;
    }
    

    parse_dataproc(tokens, instr);



    free_tokens(tokens);
}
