#include "../include/Common.h"
#include <string.h>
#include <stdlib.h>

Label ltable[MAX_LABEL];
uint32_t lcount = 0;

uint32_t find_label(char* name)
{
    for(int i = 0; i < lcount; i++)
    {
        if(strncmp(ltable[i].name, name, strlen(name)) == 0)
        {
            return i;
        }
    }
    return -1;
}

void create_label(char** tokens)
{
    Label new_label;
    new_label.addr = lcount;
    new_label.name = calloc(sizeof(char), strlen(tokens[0]));
    strncpy(new_label.name, tokens[0], strlen(tokens[0]));
    ltable[lcount++] = new_label; 
}

void parse_immd(char* line, FILE* dest_file)
{
    char** tokens = split_str(line, ' ');
    uint32_t immd = 0;
    
    bool flag = 1;
    if(find_label(tokens[0]) != -1) check_synerr("overwriting label", &flag);
    
    create_label(tokens);
    for(int i = 1; tokens[i] != NULL; i++)
    {
        immd = atoi(tokens[i] + 1);
        fwrite(&immd, sizeof(uint32_t), 1, dest_file);
    }

    free_tokens(tokens);

    return;
}