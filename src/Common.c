#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/Common.h"

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
