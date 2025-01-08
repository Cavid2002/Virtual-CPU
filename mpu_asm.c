#include <stdio.h>
#include "include/ASM.h"



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

    fclose(source_file);
    fclose(dest_file);
    return 0;
}