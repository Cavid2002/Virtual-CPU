#include "../include/Common.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Label ltable[MAX_LABEL];
uint32_t lcount = 0;

int check_label_exists(char* name)
{
    for(int i = 0; i < lcount; i++)
    {
        if(strncmp(ltable[i].name, name, strlen(name)) == 0)
        {
            return 1;
        }
        
    }
    return 0;
}

void create_label(char** tokens)
{
    Label new_label;
    new_label.addr = lcount;
    new_label.name = calloc(sizeof(char), strlen(tokens[0]));
    strncpy(new_label.name, tokens[0], strlen(tokens[0]));
    ltable[lcount++] = new_label; 
}

void parse_immd(char* line, uint32_t* immd)
{
    char** tokens = split_str(line, ' ');
    
    bool flag = 1;
    if(check_label_exists(tokens[0]) == 1) check_synerr("overwriting label", &flag);
    
    create_label(tokens);
    *immd = atoi(tokens[1]);

    free_tokens(tokens);
}