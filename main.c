#include "./include/CPU.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "[ERROR] usage [main] [binary_file]");
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");

    int c = fread(mem, sizeof(uint32_t), MEM_SIZE, file);
    
    fclose(file);

    cpu_init();
    cpu_loop();
    return 0;
}