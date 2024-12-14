#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/Decoder.h"

#define MAX_LINE 100

uint32_t instr_count = 0;

typedef enum COND_FIELD{
    EQ,
    NOT_EQ,
    CARRY_SET,
    CARRY_CLEAR,
    MINUS,
    PLUS,
    OVERFLOW_SET,
    OVERFLOW_CLEAR,
    HIGHER,
    LESS_SAME,
    GREATER_EQ,
    LESS_THAN,
    GREATER_THAN,
    LESS_EQ,
    ALWAYS,
    NEVER,
};

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
    int temp = *instr;
    int offset = 0;
    int cond = 0;
    int isize = strlen(tokens[0]);
    if(isize == 3) 
    {
        cond = 14;
        temp = cond << SHIFT_COND;
        *instr |= temp;
        return;
    }
    else if(isize == 5)
    {
        offset = 3;
    }
    else if(isize == 6)
    {
        offset = 4;
    }

    for(int i = 0; i < 14; i++)
    {
        if(strncmp(tokens[0] + offset, cond_prefix[i], 2) == 0)
        {
            cond = i;
            temp = cond << SHIFT_COND;
            *instr |= temp;
            return;
        }
    }
    

    fatal_error("syntrax error");

}



void parse_instruction(char* line, uint32_t* instr)
{
    char** tokens = split_str(line, ' ');

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
    while(fgets(line, MAX_LINE, input))
    {
        line[strlen(line) - 1] = '\0';
        parse_instruction(line, instr, instr_count);
        instr_count++;
    }
}


int main(int argc, char** argv)
{
    if(argc == 2)
    {
        fprintf(stderr, "[ERROR] provide input file!\n");
        exit(EXIT_FAILURE);
    }

    char* ptr = strchr(argv[1], '.');
    char* str = &argv[1];
    strncpy(dest_filename, argv[1], str - ptr);
    strcat(dest_filename, ".bin");

    FILE* source = fopen(argv[1], "r");
    
    FILE* destination = fopen(dest_filename, "wb");

    
}