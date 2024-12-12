#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_LINE 100

char dest_filename[MAX_LINE];
char line[MAX_LINE];
uint32_t instr;

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
        parse_instruction(line, instr);
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