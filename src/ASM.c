#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/ASM.h"

extern uint32_t lcount;
uint32_t line_count = 0;
uint32_t word_count = 0;


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
    char** tokens = calloc(10, sizeof(char*));
    
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


void parse_into_binary(char* line, FILE* dest_file)
{
    if(strchr(line, ':'))
    {
        parse_label(line, dest_file);
    }
    else
    {
        parse_instruction(line, dest_file);
    }
}


void generate_binary(FILE* source_file, FILE* dest_file)
{
    char* line = malloc(MAX_LINE);
    char* ptr;
    
    while(fgets(line, MAX_LINE, source_file))
    {
        line_count++;
        ptr = remove_space(line);
        if(strncmp(ptr, "\n", 1) == 0) 
        {
            continue;
        }
        fill_ltable(ptr);
    }   

    for(int i = 0; i < lcount; i++)
    {
        printf("%lu === %s\n", ltable[i].addr, ltable[i].name);
    }

    word_count = 0;
    line_count = 0;
    fseek(source_file, 0, SEEK_SET);

    
    while(fgets(line, MAX_LINE, source_file))
    {
        line_count++;
        ptr = remove_space(line);
        if(strncmp(ptr, "\n", 1) == 0) 
        {
            continue;
        }

        parse_into_binary(ptr, dest_file);
    }
    
}