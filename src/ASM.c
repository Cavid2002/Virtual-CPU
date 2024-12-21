#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/ASM.h"

uint32_t line_count = 0;
uint32_t instr_count = 0;


void fatal_error(const char* msg)
{
    fprintf(stderr, "[ERROR] %s at line %lu!\n", msg, line_count);
    exit(EXIT_FAILURE);
}

void check_synerr(const char* msg, bool* flag)
{
    if(*flag == 0)
    {
        *flag = 1;
        return;
    }
    fatal_error(msg);
}

char* remove_space(char* line)
{
    for(int i = 0; i < strlen(line); i++)
    {
        if(line[i] != ' ')
        {
            return line + i;
        }
    }
    return NULL;
}


char** split_str(char* str, char* delim)
{
    char** tokens = calloc(20, sizeof(char*));
    
    char* temp = strtok(str, delim);
    int c = 0;
    while(temp != NULL)
    {
        tokens[c] = calloc(strlen(temp) + 1, sizeof(char));
        strncpy(tokens[c], temp, strlen(temp) + 1);
        temp = strtok(NULL, delim);
        c++;
    }
    
    tokens[c] = NULL;
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


void generate_binary(FILE* source_file, FILE* dest_file)
{
    uint32_t foo = 0;
    char* line = malloc(MAX_LINE);
    char* ptr, ptr2;

    uint32_t segment_type = 0;
    fwrite(&foo, sizeof(uint32_t), 1, dest_file);

    while(fgets(line, MAX_LINE, source_file))
    {
        line_count++;

        line[strlen(line) - 1] = '\0';
        ptr = remove_space(line);
        if(ptr == NULL) 
        {
            continue;
        }
        
        
        if(strncmp(ptr, ".const", strlen(".const")) == 0)
        {
            segment_type = CONST_SEGMENT;
            continue;
        }
        else if(strncmp(ptr, ".text", strlen(".text")) == 0)
        {
            segment_type = CODE_SEGMENT;
            continue;
        }

        
        if(segment_type == CODE_SEGMENT) parse_instruction(ptr, dest_file);
        else if(segment_type == CONST_SEGMENT) parse_immd(ptr, dest_file);

        instr_count++; 
    }

    fseek(dest_file, 0, SEEK_SET);
    uint32_t offset = uncoditional_brch(laddr - 1);
    fwrite(&offset, sizeof(uint32_t), 1, dest_file);
    
}