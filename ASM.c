#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#include "include/Decoder.h"

#define MAX_LINE 100

uint32_t instr_count = 0;
bool error_occured = 0;

char* regs[16] = {"r0", "r1", "r2", "r3",
                  "r4", "r5", "r6", "r7",
                   "r8", "r9", "r10", "r11",
                   "r12", "13", "r14", "r15"};
char* opcode[4] = {"", "ldr", "str", "brc"};
char* aluop[8] = {"add", "sub", "orr", "and", "xor", "lsh", "rsh"};
char* cond_prefix[16] = {"eq", "ne", "cs", "cc", 
                        "mi", "pl", "vs", "vc", 
                        "hi", "ls", "ge", "lt",
                        "gt", "le", "al", NULL};

char dest_filename[MAX_LINE];
char line[MAX_LINE];
uint32_t instr;

void fatal_error(const char* msg)
{
    fprintf(stderr, "[ERROR]%s\n", msg);
    exit(EXIT_FAILURE);
}

void syntrax_error(const char* msg)
{
    fprintf(stderr, "[SYNTRAX-ERROR]line %d\n", instr_count);
    exit(EXIT_FAILURE);
}


char** split_str(char* str, char c)
{
    char* p1 = str;
    char* p2 = str;
    int token_count = 0;
    while(p1 != NULL)
    {
        p1 = strchr(p1 + 1, c);
        token_count++;
    } 
    char** tokens = (char**)malloc(sizeof(char*) * (token_count + 1));
    if(token_count == 1)
    {
        tokens[0] = (char*)malloc(strlen(str) + 1);
        strcpy(tokens[0], str);
        tokens[1] = NULL;
        return tokens;
    }

    for(int i = 0; 1; i++)
    {
        p1 = strchr(p2, c);
        if(p1 == NULL)
        {
            tokens[i] = (char*)malloc(strlen(p2) + 1);
            strcpy(tokens[i], p2);
            break;
        }
        *p1 = '\0';
        tokens[i] = (char*)malloc(strlen(p2) + 1);
        strcpy(tokens[i], p2);
        *p1 = c;
        p2 = p1 + 1;

    }
    tokens[token_count] = NULL;
    return tokens;
}


void free_tokens(char** tokens)
{
    for(int i = 0; tokens[i] != NULL; i++)
    {
        free(tokens[i]);
    }
    free(tokens);
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
        else fatal_error("syntax error");
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
    
    for(int i = 0; i < 16; i++)
    {
        if(strncmp(tokens[1], regs[i], strlen(regs[i])) == 0)
        {   
            temp |= i << SHIFT_REGDEST;
            break;
        }
    }

    for(int i = 0; i < 16; i++)
    {
        if(strncmp(tokens[2], regs[i], strlen(regs[i])) == 0)
        {   
            temp |= i << SHIFT_REGSRC1;
            break;
        }
    }

    if(tokens[3][0] == '-')
    {
        temp |= 1 << SHIFT_FUNCODE; //substraction
    }
    else
    {
        temp |= 0 << SHIFT_FUNCODE; //addition
    }

    char* ptr = strchr(tokens[0], 'r');
    if(ptr != NULL)
    {
        for(int i = 0; i < 16; i++)
        {
            if(strncmp(ptr, regs[i], strlen(regs[i])) == 0)
            {   
                temp |= i << SHIFT_REGSRC2;
                break;
            }
        }
    }
    ptr = strchr(tokens[3], '#');
    if(ptr != NULL)
    {
        temp |= 1 << SHIFT_IMMD;
        temp |= atoi(ptr + 1);
    }
    
    *instr |= temp;
    return;
}

void parse_store(char** tokens, uint32_t* instr)
{
    uint32_t temp;
    
    temp = 0;
    temp |= 1 << SHIFT_OPCODE;
    
    for(int i = 0; i < 16; i++)
    {
        if(strncmp(tokens[1], regs[i], strlen(regs[i])) == 0)
        {   
            temp |= i << SHIFT_REGDEST;
            break;
        }
    }

    for(int i = 0; i < 16; i++)
    {
        if(strncmp(tokens[2], regs[i], strlen(regs[i])) == 0)
        {   
            temp |= i << SHIFT_REGSRC1;
            break;
        }
    }

    if(tokens[3][0] == '-')
    {
        temp |= 1 << SHIFT_FUNCODE; //substraction
    }
    else
    {
        temp |= 0 << SHIFT_FUNCODE; //addition
    }

    char* ptr = strchr(tokens[0], 'r');
    if(ptr != NULL)
    {
        for(int i = 0; i < 16; i++)
        {
            if(strncmp(ptr, regs[i], strlen(regs[i])) == 0)
            {   
                temp |= i << SHIFT_REGSRC2;
                break;
            }
        }
    }
    ptr = strchr(tokens[3], '#');
    if(ptr != NULL)
    {
        temp |= 1 << SHIFT_IMMD;
        temp |= atoi(ptr + 1);
    }
    
    *instr |= temp;
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
    
    for(int i = 0; i < 8; i++)
    {
        if(strncmp(tokens[0], aluop[i], 3) == 0)
        {
            temp |= i << SHIFT_FUNCODE;
            break;
        }
    }

    for(int i = 0; i < 16; i++)
    {
        if(strncmp(tokens[1], regs[i], strlen(regs[i])) == 0)
        {
            temp |= i << SHIFT_REGDEST;
            break;
        }
    }

    for(int i = 0; i < 16; i++)
    {
        if(strncmp(tokens[2], regs[i], strlen(regs[i])) == 0)
        {
            temp |= i << SHIFT_REGSRC1;
            break;
        }
    }

    ptr = strchr(tokens[3], 'r');
    if(ptr != NULL)
    {
        for(int i = 0; i < 16; i++)
        {
            if(strncmp(ptr, regs[i], strlen(regs[i])) == 0)
            {
                temp |= i << SHIFT_REGSRC2;
                break;
            }
        }
    }

    ptr = strchr(tokens[3], '#');
    if(ptr != NULL)
    {
        temp |= 1 << SHIFT_IMMD;
        temp |= atoi(ptr + 1);
    }

    *instr |= temp;

}

void parse_instruction(char* line, uint32_t* instr)
{
    printf("%s\n", line);
    char** tokens = split_str(line, ' ');
    *instr = 0;

    if(strcmp(tokens[0], "out") == 0)
    {
        *instr = 0xFFFFFFFF;

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


void generate_binary(FILE* input, FILE* output)
{
    uint32_t instr;
    while(fgets(line, MAX_LINE, input))
    {
        instr = 0;
        if(strcmp(line, "\n") == 0) continue;
        line[strlen(line) - 1] = '\0';
        parse_instruction(line, &instr);
        fwrite(&instr, sizeof(uint32_t), 1, output);
        instr_count++; 
    }
}


int main(int argc, char** argv)
{
    if(argc != 3)
    {
        printf("%d\n", argc);
        fatal_error("Usage: ./ASM [input_file] [output_file]");
    }

    
    FILE* source = fopen(argv[1], "r");
    
    FILE* destination = fopen(argv[2], "wb");

    generate_binary(source, destination);

    fclose(source);
    fclose(destination);
    
}