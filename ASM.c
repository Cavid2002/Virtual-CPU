#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "include/Common.h"


void generate_binary(FILE* source_file, FILE* dest_file)
{
    uint32_t instr = 0;
    char* line = malloc(MAX_LINE);
    uint32_t segment_type = 0;
    fwrite(dest_file, sizeof(uint32_t), 1, dest_file);
    
    while(fgets(line, MAX_LINE, source_file))
    {
        instr = 0;
        line_count++;
        if(strcmp(line, "\n") == 0)
        {
            continue;
        }
        line[strlen(line) - 1] = '\0';
        
        if(strncmp(line, ".const", strlen(".const")) == 0)
        {
            segment_type = CONST_SEGMENT;
        }
        else if(strncmp(line, ".text", strlen(".text")) == 0)
        {
            segment_type = CODE_SEGMENT;
        }

        
        if(segment_type == CODE_SEGMENT) parse_instruction(line, &instr);
        else if(segment_type == CONST_SEGMENT) parse_immd(line, &instr);

        fwrite(&instr, sizeof(uint32_t), 1, dest_file);
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

    
    FILE* source_file = fopen(argv[1], "r");
    
    FILE* dest_file = fopen(argv[2], "wb");

    generate_binary(source_file, dest_file);

    fseek(dest_file, 0, SEEK_SET);
    uint32_t offset = uncoditional_brch(lcount - 1);
    fwrite(&offset, sizeof(uint32_t), 1, dest_file);

    fclose(source_file);
    fclose(dest_file);
    return 0;
}