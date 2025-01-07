#include "../include/ASM.h"
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
    new_label.addr = word_count;
    new_label.name = calloc(strlen(tokens[0] - 1), sizeof(char));
    strncpy(new_label.name, tokens[0], strlen(tokens[0] - 1));
    ltable[lcount++] = new_label; 
}


void fill_ltable(char* line)
{
    if(strchr(line, ':') == NULL)
    {  
        word_count++;
        return;
    }  
    char** tokens = split_str(line, " ");
    if(find_label(tokens[0]) == -1)
    {
        create_label(tokens);       
    }
    
    for(int i = 1; tokens[i] != NULL; i++)
    {
        word_count++;
    }

    free_tokens(tokens);
}


void parse_label(char* line, FILE* dest_file)
{
    char** tokens = split_str(line, " ");

    int immd;
    for(int i = 1; tokens[i] != NULL; i++)
    {
        immd = atoi(tokens[i] + 1);
        fwrite(&immd, sizeof(int), 1, dest_file);
        word_count++;
    }

    free_tokens(tokens);
}